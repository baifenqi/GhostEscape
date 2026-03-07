/**
 * @file Asset_store.h
 * @brief 资源管理类 - 统一加载和管理游戏资源
 * 
 * 【核心职责】
 * 1. 加载纹理、字体、音效、音乐资源
 * 2. 缓存已加载的资源，避免重复加载
 * 3. 提供资源获取接口
 * 4. 统一管理资源释放
 * 
 * 【设计模式】
 * - 享元模式：共享资源实例，减少内存占用
 * - 懒加载：首次请求时才加载资源
 * 
 * 【使用方式】
 * AssetStore* store = Game::getInstance().getAssetStore();
 * SDL_Texture* tex = store->getTexture("assets/image.png");
 */

#ifndef     ASSET_STORE_H
#define     ASSET_STORE_H

#include <unordered_map>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

/**
 * @class AssetStore
 * @brief 游戏资源管理器，负责加载、缓存和提供各类资源
 * 
 * 【管理的资源类型】
 * - SDL_Texture*: 图像纹理（PNG、JPG 等）
 * - TTF_Font*: 字体文件（TTF 格式）
 * - Mix_Chunk*: 短音效（WAV 格式）
 * - Mix_Music*: 背景音乐（MP3、OGG 格式）
 * 
 * 【缓存机制】
 * 使用 std::unordered_map 以文件路径为键存储资源指针
 * 相同路径的资源只加载一次，后续直接返回缓存的指针
 * 
 * 【依赖关系】
 * 需要 SDL_Renderer 指针来加载纹理
 */
class AssetStore
{
private:
    SDL_Renderer* renderer_ = nullptr;                      ///< SDL 渲染器：用于加载纹理
    
    /// 资源缓存容器（路径 -> 资源指针）
    std::unordered_map<std::string,SDL_Texture*> textures_;   ///< 纹理缓存
    std::unordered_map<std::string,TTF_Font*> fonts_;         ///< 字体缓存
    std::unordered_map<std::string,Mix_Chunk*> sounds_;       ///< 音效缓存
    std::unordered_map<std::string,Mix_Music*> musics_;       ///< 音乐缓存

public:
    /**
     * @brief 构造函数
     * @param renderer SDL 渲染器指针（用于加载纹理）
     */
    AssetStore(SDL_Renderer* renderer) {renderer_ = renderer;}
    
    ~AssetStore() = default;  ///< 析构函数（需要手动调用 clean() 释放资源）

    /**
     * @brief 清理所有资源
     * 
     * 【清理内容】
     * 1. 销毁所有纹理
     * 2. 关闭所有字体
     * 3. 释放所有音效
     * 4. 释放所有音乐
     * 5. 清空所有缓存容器
     */
    void clean();

    // ==================== 加载资源 ====================

    /**
     * @brief 加载纹理文件
     * @param file_path 纹理文件路径（如 "assets/image.png"）
     * 
     * 【功能说明】
     * 使用 IMG_LoadTexture() 加载图像并创建 SDL_Texture
     * 失败时记录错误日志
     */
    void loadTexture(const std::string& file_path);

    /**
     * @brief 加载字体文件
     * @param file_path 字体文件路径（如 "assets/font.ttf"）
     * @param font_size 字体大小（像素）
     * 
     * 【功能说明】
     * 使用 TTF_OpenFont() 打开字体文件
     * 失败时记录错误日志
     */
    void loadFont(const std::string& file_path, int font_size);

    /**
     * @brief 加载音效文件
     * @param file_path 音效文件路径（如 "assets/sound.wav"）
     * 
     * 【功能说明】
     * 使用 Mix_LoadWAV() 加载 WAV 格式音效
     * 失败时记录错误日志
     */
    void loadSound(const std::string& file_path);

    /**
     * @brief 加载音乐文件
     * @param file_path 音乐文件路径（如 "assets/music.mp3"）
     * 
     * 【功能说明】
     * 使用 Mix_LoadMUS() 加载 MP3 或 OGG 格式音乐
     * 失败时记录错误日志
     */
    void loadMusic(const std::string& file_path);

    // ==================== 获取资源 ====================

    /**
     * @brief 获取纹理资源
     * @param file_path 纹理文件路径
     * @return SDL_Texture* 纹理指针，失败返回 nullptr
     * 
     * 【智能加载】
     * 如果纹理未加载，会自动调用 loadTexture() 加载
     * 然后返回缓存的纹理指针
     */
    SDL_Texture* getTexture(const std::string& file_path);

    /**
     * @brief 获取字体资源
     * @param file_path 字体文件路径
     * @param font_size 字体大小
     * @return TTF_Font* 字体指针，失败返回 nullptr
     * 
     * 【智能加载】
     * 如果字体未加载，会自动调用 loadFont() 加载
     */
    TTF_Font* getFont(const std::string& file_path, int font_size);

    /**
     * @brief 获取音效资源
     * @param file_path 音效文件路径
     * @return Mix_Chunk* 音效指针，失败返回 nullptr
     * 
     * 【智能加载】
     * 如果音效未加载，会自动调用 loadSound() 加载
     */
    Mix_Chunk* getSound(const std::string& file_path);

    /**
     * @brief 获取音乐资源
     * @param file_path 音乐文件路径
     * @return Mix_Music* 音乐指针，失败返回 nullptr
     * 
     * 【智能加载】
     * 如果音乐未加载，会自动调用 loadMusic() 加载
     */
    Mix_Music* getMusic(const std::string& file_path);
};

#endif // ASSET_STORE_H