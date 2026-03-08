/**
 * @file Asset_store.h
 * @brief 资源管理器类 - 集中管理游戏资源（纹理、字体、音频）
 * 
 * 【核心职责】
 * 1. 加载和缓存纹理、字体、音频等资源
 * 2. 提供资源的统一访问接口
 * 3. 实现资源的懒加载（首次请求时加载）
 * 4. 自动管理资源生命周期
 * 
 * 【设计模式】
 * 单例模式：全局唯一的资源管理器
 */

#ifndef     ASSET_STORE_H
#define     ASSET_STORE_H

#include <unordered_map>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

class AssetStore
{
private:
    SDL_Renderer* renderer_ = nullptr;
    std::unordered_map<std::string,SDL_Texture*> textures_;
    std::unordered_map<std::string,TTF_Font*> fonts_;
    std::unordered_map<std::string,Mix_Chunk*> sounds_;
    std::unordered_map<std::string,Mix_Music*> musics_;

public:
    AssetStore(SDL_Renderer* renderer) {renderer_ = renderer;};
    ~AssetStore() = default;

    void clean();

    //载
    void loadTexture(const std::string& file_path);
    void loadFont(const std::string& file_path, int font_size);
    void loadSound(const std::string& file_path);
    void loadMusic(const std::string& file_path);

    //获取
    SDL_Texture* getTexture(const std::string& file_path);
    TTF_Font* getFont(const std::string& file_path, int font_size);
    Mix_Chunk* getSound(const std::string& file_path);
    Mix_Music* getMusic(const std::string& file_path);
};




#endif // ASSET_STORE_H