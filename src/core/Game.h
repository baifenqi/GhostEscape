/**
 * @file Game.h
 * @brief 游戏引擎核心类 - 负责游戏主循环、资源管理和渲染
 * 
 * 【核心职责】
 * 1. 管理游戏主循环（事件处理→逻辑更新→画面渲染）
 * 2. 管理 SDL 窗口和渲染器
 * 3. 管理资源管理器（AssetStore）
 * 4. 管理当前场景
 * 5. 提供基础渲染工具函数
 */

#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <glm/glm.hpp>

#include "Asset_store.h"

#include <string>

class Scene;
struct Texture;
class Game
{
private:
    AssetStore* asset_store_ = nullptr;//资源管理器
    glm::vec2 screen_size_ = glm::vec2(0);

    bool isRunning_ = true;

    Scene* current_scene_ = nullptr;

    Uint64 FPS_ = 60;
    Uint64 frame_delay_ = 0;
    float dt_ = 0.0f;

    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    Game(){}
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

public:
    static Game& getInstance()
    {
        static Game instance;
        return instance;
    }

    void run();
    void init(std::string title, int width, int height);
    void handleEvents();
    void update(float dt);
    void render();
    void clean();

    // 获取屏幕尺寸
    glm::vec2 getScreenSize() const { return screen_size_; }// 获取屏幕尺寸
    Scene* getCurrentScene() const {return current_scene_;}// 获取当前场景
    AssetStore* getAssetStore() const { return asset_store_; }//获取资源管理器

    //渲染函数
    void renderTexture(const Texture& texture, const glm::vec2& position, const glm::vec2& size);//渲染纹理

    void renderFillCircle(const glm::vec2& position, glm::vec2& size, float alpha);

    //工具函数：
    void drawGrid(const glm::vec2& top_left, const glm::vec2& bottom_right, float grid_width, SDL_FColor fcolor);
    void drawBoundary(const glm::vec2& top_left, const glm::vec2& bottom_right, float boundary_width, SDL_FColor fcolor);
};

#endif // GAME_H
