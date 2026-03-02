#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include <memory>
#include "core/Scene.h"

class Player;
class SceneMain : public Scene {
private:

    Player* player_ = nullptr;
    
    // 网格配置常量
    static constexpr float GRID_SIZE = 100.0f;      // 网格间距
    static constexpr float BOUNDARY_WIDTH = 2.0f;         // 边界线宽度
    
    // 颜色配置
    static constexpr SDL_FColor GRID_COLOR = {0.5f, 0.5f, 0.5f, 0.6f};
    static constexpr SDL_FColor BOUNDARY_COLOR = {1.0f, 8.0f, 8.0f, 1.0f};
   
    // 键盘状态追踪
    bool keys_[256] = {false};

public:
    SceneMain() = default;
    virtual ~SceneMain() = default;
    
    void init() override;
    void handleEvents(SDL_Event& event) override;
    void update(float dt) override;   
    void render() override;    
    void clean() override;

private:
    void renderBackground();
        
};

#endif // SCENEMAIN_H