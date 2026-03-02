#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <glm/glm.hpp>

#include <string>

class Scene;

class Game
{
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
    
    //工具函数：
    void drawGrid(const glm::vec2& top_left, const glm::vec2& bottom_right, float grid_width, SDL_FColor fcolor);
    void drawBoundary(const glm::vec2& top_left, const glm::vec2& bottom_right, float boundary_width, SDL_FColor fcolor);
};

#endif // GAME_H
