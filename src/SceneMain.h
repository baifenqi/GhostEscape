/**
 * @file SceneMain.h
 * @brief 主场景类 - 游戏的主要场景实现
 * 
 * 【核心职责】
 * 1. 管理游戏主场景的所有对象
 * 2. 渲染背景网格和边界
 * 3. 处理玩家输入（键盘控制）
 * 4. 管理玩家对象
 * 
 * 【场景配置】
 * - 世界大小：屏幕大小的 3 倍
 * - 摄像机初始位置：世界中心附近
 * - 网格间距：100 像素
 * - 边界颜色：红色（R:1, G:8, B:8）
 * 
 * 【继承体系】
 * Object -> Scene -> SceneMain
 */

#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include <memory>
#include "core/Scene.h"

class Player;  // 前向声明：玩家类

/**
 * @class SceneMain
 * @brief 游戏主场景，包含玩家和背景网格
 * 
 * 【主要功能】
 * 1. 初始化世界大小和摄像机位置
 * 2. 创建并管理玩家对象
 * 3. 追踪键盘按键状态（用于平滑移动）
 * 4. 渲染背景网格和边界框
 * 
 * 【配置常量】
 * - GRID_SIZE: 网格线间距（100 像素）
 * - BOUNDARY_WIDTH: 边界线宽度（2 像素）
 * - GRID_COLOR: 灰色半透明网格线
 * - BOUNDARY_COLOR: 红色边界线
 * 
 * 【关键属性】
 * - player_: 玩家对象指针
 * - keys_[256]: 键盘按键状态数组
 */
class SceneMain : public Scene {
private:
    Player* player_ = nullptr;  ///< 玩家对象指针

    // ==================== 网格配置常量 ====================
    
    static constexpr float GRID_SIZE = 100.0f;       ///< 网格间距：100 像素
    static constexpr float BOUNDARY_WIDTH = 2.0f;    ///< 边界线宽度：2 像素

    // ==================== 颜色配置 ====================
    
    /// 网格颜色：灰色半透明 (RGBA)
    static constexpr SDL_FColor GRID_COLOR = {0.5f, 0.5f, 0.5f, 0.6f};
    
    /// 边界颜色：红色 (RGBA) - 注意：G 值 8.0 可能是笔误，应为 0.0-1.0 范围
    static constexpr SDL_FColor BOUNDARY_COLOR = {1.0f, 8.0f, 8.0f, 1.0f};

    // ==================== 输入控制 ====================
    
    bool keys_[256] = {false};  ///< 键盘按键状态数组：追踪每个键的按下状态

public:
    SceneMain() = default;      ///< 默认构造函数
    virtual ~SceneMain() = default;  ///< 虚析构函数

    /**
     * @brief 初始化主场景
     * 
     * 【初始化内容】
     * 1. 设置世界大小 = 屏幕大小 × 3
     * 2. 设置摄像机位置 = 世界中心 - 屏幕中心
     * 3. 创建玩家对象
     * 4. 初始化玩家
     * 5. 设置玩家位置在世界中心
     * 6. 将玩家添加为场景子对象
     */
    void init() override;

    /**
     * @brief 处理 SDL 事件
     * @param event SDL 事件引用
     * 
     * 【处理内容】
     * 1. 调用 Scene::handleEvents(event) 处理子对象事件
     * 2. 监听键盘按下事件（SDL_EVENT_KEY_DOWN）
     * 3. 监听键盘释放事件（SDL_EVENT_KEY_UP）
     * 4. 更新 keys_ 数组中的按键状态
     */
    void handleEvents(SDL_Event& event) override;

    /**
     * @brief 更新场景逻辑
     * @param dt 时间增量（秒）
     * 
     * 【当前实现】
     * 调用 Scene::update(dt) 更新所有子对象
     * 玩家移动逻辑在 Player::update() 中实现
     */
    void update(float dt) override;

    /**
     * @brief 渲染场景
     * 
     * 【渲染顺序】
     * 1. renderBackground() - 绘制背景网格和边界
     * 2. Scene::render() - 渲染所有子对象（包括玩家）
     */
    void render() override;

    /**
     * @brief 清理场景资源
     * 
     * 【清理内容】
     * 调用 Scene::clean() 清理所有子对象
     */
    void clean() override;

private:
    /**
     * @brief 渲染背景
     * 
     * 【渲染内容】
     * 1. 计算可见区域的起始和结束坐标
     * 2. 绘制主网格（GRID_SIZE 间距，灰色）
     * 3. 绘制边界矩形框（BOUNDARY_WIDTH 宽度，红色）
     */
    void renderBackground();
};

#endif // SCENEMAIN_H
