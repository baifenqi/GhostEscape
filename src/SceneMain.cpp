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
#include "Enemy.h"

/**
 * @brief 初始化主场景
 * 
 * 【初始化流程】
 * 
 * 1. **设置世界大小**
 *    ```cpp
 *    world_size_ = game_.getScreenSize() * 3.0f;
 *    ```
 *    - 创建一个比屏幕大 3 倍的游戏世界
 *    - 提供探索空间，支持摄像机移动
 * 
 * 2. **设置摄像机初始位置**
 *    ```cpp
 *    camera_position_ = world_size_ / 2.0f - game_.getScreenSize() / 2.0f;
 *    ```
 *    - 位置 = 世界中心 - 屏幕中心
 *    - 确保玩家初始时在屏幕中央
 * 
 * 3. **创建并初始化玩家**
 *    ```cpp
 *    player_ = new Player();
 *    player_->init();
 *    player_->setPosition(world_size_ / 2.0f);
 *    addChild(player_);
 *    ```
 *    - 在世界中心创建玩家
 *    - 调用完整初始化流程
 *    - 添加为场景子对象（自动分类到 children__world_）
 * 
 * 4. **创建并初始化敌人**
 *    ```cpp
 *    enemy_ = new Enemy();
 *    enemy_->init();
 *    enemy_->setTarget(player_);
 *    enemy_->setPosition(world_size_ / 2.0f + glm::vec2(200.0f, 0.0f));
 *    addChild(enemy_);
 *    ```
 *    - 在玩家右侧 200 像素处创建敌人
 *    - 设置追踪目标为玩家
 *    - 调用完整初始化流程
 *    - 添加为场景子对象
 * 
 * 【初始布局】
 * ```
 * 世界中心：player_ at (world_size/2)
 * 敌人位置：enemy_ at (world_size/2 + (200, 0))
 * 摄像机：对准玩家
 * ```
 */
void SceneMain::init()
{
    // 世界大小 = 屏幕大小 × 3（更大的游戏世界）
    world_size_ = game_.getScreenSize() * 3.0f;
    
    // 摄像机初始位置 = 世界中心 - 屏幕中心
    // 这样玩家会出现在屏幕中央
    camera_position_ = world_size_ / 2.0f - game_.getScreenSize() / 2.0f;

    // ==================== 创建玩家 ====================
    
    // 创建玩家对象
    player_ = new Player();
    
    // 初始化玩家（加载精灵、创建碰撞体、设置属性）
    player_->init();
    
    // 设置玩家位置在世界中心
    player_->setPosition(world_size_ / 2.0f);
    
    // 将玩家添加为场景的子对象（自动分类到 children__world_）
    addChild(player_);

    // ==================== 创建敌人 ====================
    
    // 创建敌人对象
    enemy_ = new Enemy();
    
    // 初始化敌人（加载三种状态动画、创建碰撞体）
    enemy_->init();
    
    // 设置敌人的追踪目标为玩家
    enemy_->setTarget(player_);
    
    // 设置敌人初始位置（玩家右侧 200 像素）
    enemy_->setPosition(world_size_ / 2.0f + glm::vec2(200.0f, 0.0f));
    
    // 将敌人添加为场景的子对象
    addChild(enemy_);
}

/**
 * @brief 处理 SDL 事件
 * 
 * @param event SDL 事件引用
 * 
 * 【事件处理】
 * 1. 调用Scene::handleEvents(event) 让子对象处理事件
 * 2. 监听键盘事件：
 *    - SDL_EVENT_KEY_DOWN: 按键按下，设置 keys_[key] = true
 *    - SDL_EVENT_KEY_UP: 按键释放，设置 keys_[key] = false
 * 3. 按键索引限制在 0-255 范围内
 * 
 * 【用途】
 * keys_ 数组用于 Player::keyboardControl() 中的平滑移动控制
 * 
 * 【注意】
 * 玩家实际使用 SDL_GetKeyboardState() 轮询而非此处的 keys_数组
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
 * 调用Scene::update(dt) 更新所有子对象
 * 
 * 【实际效果】
 * - Player::update(dt) 会被调用
 *   - 处理玩家输入
 *   - 更新移动和动画
 *   - 同步摄像机
 * - Enemy::update(dt) 会被调用
 *   - AI 追踪玩家
 *   - 移动和攻击检测
 * 
 * 【扩展建议】
 * 可以在此添加其他游戏逻辑更新，如：
 * - 检查敌人死亡并移除
 * - 生成新敌人
 * - 关卡逻辑
 */
void SceneMain::update(float dt)
{
    // 调用父类更新函数，更新所有子对象
    Scene::update(dt);
    
    // TODO: 可以在这里添加场景特定的逻辑
    // 例如：检查敌人是否需要移除
    // if(enemy_ && enemy_->needMove_){
    //     removeChild(enemy_);
    //     delete enemy_;
    //     enemy_ = nullptr;
    // }
}

/**
 * @brief 渲染场景
 * 
 * 【渲染顺序】
 * 1. renderBackground() - 绘制背景网格和边界
 * 2. Scene::render() - 渲染所有子对象（包括玩家、敌人）
 * 
 * 【设计意图】
 * 先绘制背景，再绘制对象，确保正确的图层顺序
 * 
 * 【图层结构】
 * 1. 背景层：网格线、边界框
 * 2. 世界对象层：玩家、敌人（受摄像机影响）
 * 3. 屏幕对象层：UI 元素（不受摄像机影响）
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
 * 
 * 2. 绘制主网格：
 *    - 使用 Game::drawGrid()
 *    - 间距 GRID_SIZE (100px)
 *    - 颜色 GRID_COLOR (灰色半透明)
 * 
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
 * 调用Scene::clean() 清理所有子对象
 * 
 * 【自动清理】
 * - player_ 会被 Scene::clean() 自动清理
 * - enemy_ 会被 Scene::clean() 自动清理
 * - 无需手动删除
 * 
 * 【清理顺序】
 * 1. 清理所有 children__world_（玩家、敌人）
 * 2. 清理所有 children__screen_（如果有）
 * 3. 清空容器
 */
void SceneMain::clean()
{
    // 调用父类清理函数
    Scene::clean();
}
