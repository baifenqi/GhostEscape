/**
 * @file SceneMain.cpp
 * @brief 主场景类的实现文件
 * 
 * 【主要内容】
 * 实现游戏主场景的初始化、事件处理、更新、渲染和清理功能
 */

#include <algorithm>
#include <chrono>
#include <cstring>
#include <cmath>

#include "SceneMain.h"
#include "Player.h"

/**
 * @brief 初始化主场景
 * 
 * 【初始化流程】
 * 1. 设置世界大小为屏幕大小的 3 倍
 *    - 创建一个比屏幕大的游戏世界，提供探索空间
 * 2. 设置摄像机初始位置
 *    - 位置 = 世界中心 - 屏幕中心
 *    - 确保玩家初始时在屏幕中央
 * 3. 创建玩家对象
 * 4. 初始化玩家（加载精灵动画）
 * 5. 设置玩家位置在世界中心
 * 6. 将玩家添加为场景的子对象
 */
void SceneMain::init()
{
    // 世界大小 = 屏幕大小 × 3（更大的游戏世界）
    world_size_ = game_.getScreenSize() * 3.0f;
    
    // 摄像机初始位置 = 世界中心 - 屏幕中心
    // 这样玩家会出现在屏幕中央
    camera_position_ = world_size_ / 2.0f - game_.getScreenSize() / 2.0f;
    
    // 创建玩家对象
    player_ = new Player();
    
    // 初始化玩家（加载精灵、设置属性）
    player_->init();
    
    // 设置玩家位置在世界中心
    player_->setPosition(world_size_ / 2.0f);
    
    // 将玩家添加为场景的子对象（自动分类到 children__world_）
    addChild(player_);
}

/**
 * @brief 处理 SDL 事件
 * 
 * @param event SDL 事件引用
 * 
 * 【事件处理】
 * 1. 调用 Scene::handleEvents(event) 让子对象处理事件
 * 2. 监听键盘事件：
 *    - SDL_EVENT_KEY_DOWN: 按键按下，设置 keys_[key] = true
 *    - SDL_EVENT_KEY_UP: 按键释放，设置 keys_[key] = false
 * 3. 按键索引限制在 0-255 范围内
 * 
 * 【用途】
 * keys_ 数组用于 Player::keyboardControl() 中的平滑移动控制
 */
void SceneMain::handleEvents(SDL_Event& event)
{
    // 让场景中的所有对象处理事件
    Scene::handleEvents(event);
    
    // 处理键盘事件
    switch(event.type){
        case SDL_EVENT_KEY_DOWN:
            // 按键按下：记录按键状态
            if (event.key.key < 256) {
                keys_[event.key.key] = true;
            }
            break;
        case SDL_EVENT_KEY_UP:
            // 按键释放：清除按键状态
            if (event.key.key < 256) {
                keys_[event.key.key] = false;
            }
            break;
    }
}

/**
 * @brief 更新场景逻辑
 * 
 * @param dt 时间增量（秒）
 * 
 * 【当前实现】
 * 调用 Scene::update(dt) 更新所有子对象
 * 
 * 【实际效果】
 * - 玩家对象的 update() 会被调用
 * - 玩家在 Player::update() 中处理移动、动画切换等逻辑
 * 
 * 【扩展建议】
 * 可以在此添加其他游戏逻辑更新，如：
 * - NPC AI
 * - 物理模拟
 * - 碰撞检测
 */
void SceneMain::update(float dt)
{
    // 调用父类更新函数，更新所有子对象
    Scene::update(dt);
}

/**
 * @brief 渲染场景
 * 
 * 【渲染顺序】
 * 1. renderBackground() - 绘制背景网格和边界
 * 2. Scene::render() - 渲染所有子对象（包括玩家）
 * 
 * 【设计意图】
 * 先绘制背景，再绘制对象，确保正确的图层顺序
 */
void SceneMain::render()
{
   // 首先渲染背景（网格和边界）
   renderBackground();
   
   // 然后渲染场景中的所有对象
   Scene::render();
}

/**
 * @brief 渲染背景
 * 
 * 【渲染内容】
 * 1. 计算可见区域的坐标范围：
 *    - start = (0, 0) - camera_position_
 *      世界原点相对于摄像机的位置（可能是负值）
 *    - end = world_size_ - camera_position_
 *      世界右下角相对于摄像机的位置
 * 2. 绘制主网格：
 *    - 使用 Game::drawGrid()
 *    - 间距 GRID_SIZE (100px)
 *    - 颜色 GRID_COLOR (灰色半透明)
 * 3. 绘制边界矩形框：
 *    - 使用 Game::drawBoundary()
 *    - 宽度 BOUNDARY_WIDTH (2px)
 *    - 颜色 BOUNDARY_COLOR (红色)
 * 
 * 【视觉效果】
 * 网格帮助玩家感知空间和移动距离
 * 边界标记游戏世界的范围
 */
void SceneMain::renderBackground()
{
    // 计算可见区域的起始和结束坐标
    auto start = glm::vec2(0.0f, 0.0f) - camera_position_;
    auto end = world_size_ - camera_position_;

    // 绘制主网格（100px 间距，灰色半透明）
    game_.drawGrid(start, end, GRID_SIZE, GRID_COLOR);

    // 绘制边界矩形框（2px 宽度，红色）
    game_.drawBoundary(start, end, BOUNDARY_WIDTH, BOUNDARY_COLOR);
}

/**
 * @brief 清理场景资源
 * 
 * 【清理内容】
 * 调用 Scene::clean() 清理所有子对象
 * 
 * 【注意】
 * player_ 指针会被 Scene::clean() 自动清理
 * 无需手动删除
 */
void SceneMain::clean()
{
    // 调用父类清理函数
    Scene::clean();
}
