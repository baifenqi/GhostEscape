/**
 * @file Asset_store.cpp
 * @brief 资源管理类的实现文件
 * 
 * 【主要内容】
 * 实现资源的加载、缓存和获取功能
 */

#include "Asset_store.h"

/**
 * @brief 清理所有资源
 * 
 * 【清理流程】
 * 1. 遍历 textures_ 容器，销毁所有 SDL_Texture
 * 2. 遍历 fonts_ 容器，关闭所有 TTF_Font
 * 3. 遍历 sounds_ 容器，释放所有 Mix_Chunk
 * 4. 遍历 musics_ 容器，释放所有 Mix_Music
 * 5. 清空所有容器
 * 
 * 【内存安全】
 * 使用正确的 SDL 函数释放各类资源，防止内存泄漏
 */
void AssetStore::clean()
{
    // 销毁所有纹理
    for(auto &texture : textures_){
        SDL_DestroyTexture(texture.second);
    }
    textures_.clear();
    
    // 关闭所有字体
    for(auto &font : fonts_){
        TTF_CloseFont(font.second);
    }
    fonts_.clear();
    
    // 释放所有音效
    for(auto &sound : sounds_){
        Mix_FreeChunk(sound.second);
    }
    sounds_.clear();
    
    // 释放所有音乐
    for(auto &music : musics_){
        Mix_FreeMusic(music.second);
    }
    musics_.clear();    
}

/**
 * @brief 加载纹理文件
 * 
 * @param file_path 纹理文件路径
 * 
 * 【实现细节】
 * 1. 使用 IMG_LoadTexture() 从文件加载纹理
 * 2. 检查返回指针是否为空
 * 3. 失败时记录错误日志（包含文件路径）
 * 4. 成功时将纹理存入 textures_ 容器
 * 
 * 【支持格式】
 * PNG, JPG, BMP, GIF 等 SDL_image 支持的格式
 */
void AssetStore::loadTexture(const std::string &file_path)
{
    // 使用 SDL_image 加载纹理
    SDL_Texture *texture = IMG_LoadTexture(renderer_, file_path.c_str());
    
    // 检查加载是否成功
    if(texture == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", file_path.c_str());
        return;
    }
    
    // 将纹理存入缓存容器
    textures_.emplace(file_path, texture);
}

/**
 * @brief 加载字体文件
 * 
 * @param file_path 字体文件路径
 * @param font_size 字体大小（像素）
 * 
 * 【实现细节】
 * 1. 使用 TTF_OpenFont() 打开字体文件
 * 2. 检查返回指针是否为空
 * 3. 失败时记录错误日志
 * 4. 成功时将字体存入 fonts_ 容器
 * 
 * 【注意事项】
 * 同一字体文件不同大小会作为不同条目存储
 */
void AssetStore::loadFont(const std::string &file_path, int font_size)
{
    // 使用 SDL_ttf 打开字体
    TTF_Font *font = TTF_OpenFont(file_path.c_str(), font_size);
    
    // 检查加载是否成功
    if(font == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load font: %s", file_path.c_str());
        return;
    }
    
    // 将字体存入缓存容器
    fonts_.emplace(file_path, font);
}

/**
 * @brief 加载音效文件
 * 
 * @param file_path 音效文件路径
 * 
 * 【实现细节】
 * 1. 使用 Mix_LoadWAV() 加载 WAV 格式音效
 * 2. 检查返回指针是否为空
 * 3. 失败时记录错误日志
 * 4. 成功时将音效存入 sounds_ 容器
 * 
 * 【支持格式】
 * 仅支持 WAV 格式（Mix_LoadWAV 的限制）
 */
void AssetStore::loadSound(const std::string &file_path)
{
    // 加载 WAV 格式音效
    Mix_Chunk *sound = Mix_LoadWAV(file_path.c_str());
    
    // 检查加载是否成功
    if(sound == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load sound: %s", file_path.c_str());
        return;
    }
    
    // 将音效存入缓存容器
    sounds_.emplace(file_path, sound);
}

/**
 * @brief 加载音乐文件
 * 
 * @param file_path 音乐文件路径
 * 
 * 【实现细节】
 * 1. 使用 Mix_LoadMUS() 加载音乐文件
 * 2. 检查返回指针是否为空
 * 3. 失败时记录错误日志
 * 4. 成功时将音乐存入 musics_ 容器
 * 
 * 【支持格式】
 * MP3, OGG, WAV, MOD 等 SDL_mixer 支持的格式
 */
void AssetStore::loadMusic(const std::string &file_path)
{
    // 加载音乐文件
    Mix_Music *music = Mix_LoadMUS(file_path.c_str());
    
    // 检查加载是否成功
    if(music == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load music: %s", file_path.c_str());
        return;
    }
    
    // 将音乐存入缓存容器
    musics_.emplace(file_path, music);
}

/**
 * @brief 获取纹理资源（带懒加载）
 * 
 * @param file_path 纹理文件路径
 * @return SDL_Texture* 纹理指针，失败返回 nullptr
 * 
 * 【查找逻辑】
 * 1. 在 textures_ 容器中查找 file_path
 * 2. 如果未找到，调用 loadTexture() 加载
 * 3. 再次查找并返回结果
 * 4. 如果仍然未找到，记录错误并返回 nullptr
 * 
 * 【优势】
 * 首次请求时才加载资源，节省内存和启动时间
 */
SDL_Texture *AssetStore::getTexture(const std::string &file_path)
{
    // 在缓存中查找
    auto iter = textures_.find(file_path);
    
    // 如果未找到，尝试加载
    if(iter == textures_.end()){
        loadTexture(file_path);
        iter = textures_.find(file_path);
    }
    
    // 检查是否加载成功
    if(iter == textures_.end()){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Texture not found: %s", file_path.c_str());
        return nullptr;
    }
    
    // 返回纹理指针
    return iter->second;
}

/**
 * @brief 获取字体资源（带懒加载）
 * 
 * @param file_path 字体文件路径
 * @param font_size 字体大小
 * @return TTF_Font* 字体指针，失败返回 nullptr
 * 
 * 【查找逻辑】
 * 1. 在 fonts_ 容器中查找 file_path
 * 2. 如果未找到，调用 loadFont() 加载
 * 3. 再次查找并返回结果
 * 4. 如果仍然未找到，记录错误并返回 nullptr
 */
TTF_Font *AssetStore::getFont(const std::string &file_path, int font_size)
{
    // 在缓存中查找
    auto iter = fonts_.find(file_path);
    
    // 如果未找到，尝试加载
    if(iter == fonts_.end()){
        loadFont(file_path, font_size);
        iter = fonts_.find(file_path);
    }
    
    // 检查是否加载成功
    if(iter == fonts_.end()){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Font not found: %s", file_path.c_str());
        return nullptr;
    }
    
    // 返回字体指针
    return iter->second;
}

/**
 * @brief 获取音效资源（带懒加载）
 * 
 * @param file_path 音效文件路径
 * @return Mix_Chunk* 音效指针，失败返回 nullptr
 * 
 * 【查找逻辑】
 * 1. 在 sounds_ 容器中查找 file_path
 * 2. 如果未找到，调用 loadSound() 加载
 * 3. 再次查找并返回结果
 * 4. 如果仍然未找到，记录错误并返回 nullptr
 */
Mix_Chunk *AssetStore::getSound(const std::string &file_path)
{
    // 在缓存中查找
    auto iter = sounds_.find(file_path);
    
    // 如果未找到，尝试加载
    if(iter == sounds_.end()){
        loadSound(file_path);
        iter = sounds_.find(file_path);
    }
    
    // 检查是否加载成功
    if(iter == sounds_.end()){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Sound not found: %s", file_path.c_str());
        return nullptr;
    }
    
    // 返回音效指针
    return iter->second;
}

/**
 * @brief 获取音乐资源（带懒加载）
 * 
 * @param file_path 音乐文件路径
 * @return Mix_Music* 音乐指针，失败返回 nullptr
 * 
 * 【查找逻辑】
 * 1. 在 musics_ 容器中查找 file_path
 * 2. 如果未找到，调用 loadMusic() 加载
 * 3. 再次查找并返回结果
 * 4. 如果仍然未找到，记录错误并返回 nullptr
 */
Mix_Music *AssetStore::getMusic(const std::string &file_path)
{
    // 在缓存中查找
    auto iter = musics_.find(file_path);
    
    // 如果未找到，尝试加载
    if(iter == musics_.end()){
        loadMusic(file_path);
        iter = musics_.find(file_path);
    }
    
    // 检查是否加载成功
    if(iter == musics_.end()){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Music not found: %s", file_path.c_str());
        return nullptr;
    }
    
    // 返回音乐指针
    return iter->second;
}
