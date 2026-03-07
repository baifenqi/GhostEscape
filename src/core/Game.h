/**
 * @file Game.h
 * @brief 游戏核心管理类 - 整个游戏的引擎和控制中心
 * 
 * 【核心职责】
 * 1. 管理游戏主循环（事件处理、逻辑更新、画面渲染）
 * 2. 管理全局资源（AssetStore）
 * 3. 管理当前场景（Scene）
 * 4. 提供渲染工具函数（纹理绘制、网格绘制等）
 * 
 * 【设计模式】
 * - 单例模式：通过 getInstance() 确保全局唯一实例
 * - 游戏主循环模式：run() 方法驱动游戏运行
 * 
 * 【使用方式】
 * Game::getInstance().init("标题", 宽度，高度);
 * Game::getInstance().run();
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

class Scene;      // 场景基类（前向声明）
struct Texture;   // 纹理结构体（前向声明）

/**
 * @class Game
 * @brief 游戏引擎核心类，控制整个游戏的运行流程
 * 
 * 【主要功能】
 * - 初始化 SDL 子系统、窗口、渲染器、音频设备
 * - 运行游戏主循环（60 FPS）
 * - 管理资源存储（AssetStore）
 * - 管理当前场景（Scene）
 * - 提供渲染工具函数
 * 
 * 【关键属性】
 * - screen_size_: 屏幕分辨率（glm::vec2）
 * - current_scene_: 当前运行的场景对象
 * - asset_store_: 资源管理器指针
 * - FPS_: 目标帧率（60 帧/秒）
 * - dt_: 帧间隔时间（秒）
 */
class Game
{
private:
    AssetStore* asset_store_ = nullptr;        ///< 资源管理器：加载/管理纹理、字体、音频
    glm::vec2 screen_size_ = glm::vec2(0);     ///< 屏幕尺寸：窗口宽度和高度（像素）

    bool isRunning_ = true;                     ///< 游戏运行标志：false 时退出主循环

    Scene* current_scene_ = nullptr;            ///< 当前场景：负责场景内的对象管理

    Uint64 FPS_ = 60;                           ///< 目标帧率：每秒 60 帧
    Uint64 frame_delay_ = 0;                    ///< 每帧延迟时间（纳秒）= 1 秒 / FPS
    float dt_ = 0.0f;                           ///< 时间增量：上一帧到当前帧的时间间隔（秒）

    SDL_Window* window_ = nullptr;              ///< SDL 窗口：显示游戏的窗口句柄
    SDL_Renderer* renderer_ = nullptr;          ///< SDL 渲染器：负责绘制图形
    
    /// 私有构造函数（单例模式）
    Game(){}
    Game(const Game&) = delete;                 ///< 禁用拷贝构造
    Game& operator=(const Game&) = delete;      ///< 禁用赋值操作

public:
    /**
     * @brief 获取游戏单例实例
     * @return Game& 全局唯一的游戏实例引用
     * 
     * 【使用说明】
     * - 任何时候都可以通过 Game::getInstance() 访问游戏管理器
     * - 返回的是静态局部变量的引用，线程安全（C++11 起）
     */
    static Game& getInstance()
    {
        static Game instance;
        return instance;
    }

    /**
     * @brief 运行游戏主循环
     * 
     * 【执行流程】
     * while(isRunning_) {
     *   1. handleEvents() - 处理输入事件
     *   2. update(dt) - 更新游戏逻辑
     *   3. render() - 渲染画面
     *   4. 延迟控制 - 维持 60 FPS
     * }
     */
    void run();

    /**
     * @brief 初始化游戏环境
     * @param title 窗口标题字符串
     * @param width 窗口宽度（像素）
     * @param height 窗口高度（像素）
     * 
     * 【初始化内容】
     * 1. SDL 视频/音频子系统
     * 2. SDL_image 图像库
     * 3. SDL_mixer 音频库（16 通道）
     * 4. SDL_ttf 字体库
     * 5. 创建窗口和渲染器
     * 6. 设置逻辑分辨率
     * 7. 创建资源管理器
     * 8. 创建并初始化初始场景
     */
    void init(std::string title, int width, int height);

    /**
     * @brief 处理 SDL 事件队列
     * 
     * 【处理内容】
     * - SDL_EVENT_QUIT: 关闭窗口事件
     * - 其他事件委托给当前场景处理
     */
    void handleEvents();

    /**
     * @brief 更新游戏逻辑
     * @param dt 时间增量（秒），用于帧率无关的运动计算
     * 
     * 【调用时机】
     * 每帧调用一次，委托给当前场景的 update() 方法
     */
    void update(float dt);

    /**
     * @brief 渲染游戏画面
     * 
     * 【渲染流程】
     * 1. 清屏（黑色背景）
     * 2. 委托给当前场景渲染
     * 3. 呈现到屏幕
     */
    void render();

    /**
     * @brief 清理游戏资源
     * 
     * 【清理内容】
     * 1. 清理并删除当前场景
     * 2. 清理并删除资源管理器
     * 3. 销毁渲染器和窗口
     * 4. 关闭音频设备
     * 5. 退出 SDL 各子系统
     */
    void clean();

    // ==================== 获取器 ====================

    /**
     * @brief 获取屏幕尺寸
     * @return glm::vec2 屏幕宽度 (x) 和高度 (y)
     */
    glm::vec2 getScreenSize() const { return screen_size_; }

    /**
     * @brief 获取当前场景
     * @return Scene* 当前运行的场景指针
     */
    Scene* getCurrentScene() const {return current_scene_;}

    /**
     * @brief 获取资源管理器
     * @return AssetStore* 资源管理器指针
     */
    AssetStore* getAssetStore() const { return asset_store_; }

    // ==================== 渲染函数 ====================

    /**
     * @brief 渲染纹理到指定位置
     * @param texture 纹理对象（包含纹理资源和源矩形）
     * @param position 渲染位置（屏幕坐标）
     * @param size 渲染尺寸（宽度和高度）
     * 
     * 【功能说明】
     * 将纹理绘制到屏幕上的指定位置和大小，支持旋转和翻转
     */
    void renderTexture(const Texture& texture, const glm::vec2& position, const glm::vec2& size);

    // ==================== 工具函数 ====================

    /**
     * @brief 绘制网格线
     * @param top_left 网格左上角坐标
     * @param bottom_right 网格右下角坐标
     * @param grid_width 网格间距
     * @param fcolor 网格颜色（RGBA）
     * 
     * 【用途】
     * 用于调试或背景装饰，绘制均匀的网格线
     */
    void drawGrid(const glm::vec2& top_left, const glm::vec2& bottom_right, float grid_width, SDL_FColor fcolor);

    /**
     * @brief 绘制边界矩形框
     * @param top_left 边界左上角坐标
     * @param bottom_right 边界右下角坐标
     * @param boundary_width 边界线宽度（像素）
     * @param fcolor 边界颜色（RGBA）
     * 
     * 【用途】
     * 绘制多层矩形边框，用于标记区域边界
     */
    void drawBoundary(const glm::vec2& top_left, const glm::vec2& bottom_right, float boundary_width, SDL_FColor fcolor);
};

#endif // GAME_H
