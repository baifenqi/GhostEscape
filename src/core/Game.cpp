/**
 * @file Game.cpp
 * @brief 游戏核心管理类的实现文件
 * 
 * 【主要内容】
 * 实现游戏主循环、初始化、事件处理、更新、渲染和资源清理功能
 */

#include "Game.h"
#include "../SceneMain.h"
#include "ObjectScreen.h"
#include "ObjectWorld.h"
#include "Actor.h"
#include "../affiliate/Sprite.h"
#include  <filesystem>
#include <iostream>

/**
 * @brief 主游戏循环函数，控制游戏的运行流程
 * 
 * 【执行流程】
 * 1. 记录帧开始时间
 * 2. 处理事件（handleEvents）
 * 3. 更新逻辑（update）
 * 4. 渲染画面（render）
 * 5. 计算帧时间差并延迟以维持 60 FPS
 * 
 * 【时间控制】
 * - 目标帧率：60 FPS
 * - 每帧时间：16.67ms (1000ms / 60)
 * - dt_ 变量：记录实际帧间隔时间（秒），用于平滑运动计算
 */
void Game::run()
{
    while (isRunning_)
    {
        // 记录帧开始时间（纳秒精度）
        Uint64 start_time = SDL_GetTicks();
        
        handleEvents();  // 处理用户输入和系统事件
        update(dt_);     // 更新游戏逻辑（使用时间增量参数）
        render();        // 渲染游戏画面
        
        // 计算帧耗时
        Uint64 end_time = SDL_GetTicks();
        auto elapsed_time = end_time - start_time;
        
        // 帧率控制：如果帧耗时小于目标时间，则延迟
        if (elapsed_time < frame_delay_)
        {
            SDL_DelayNS((frame_delay_ - elapsed_time));  // 延迟以维持稳定的帧率
            dt_ = static_cast<float>(frame_delay_ / 1.0e9);  // 使用目标帧时间作为 dt
        }
        else
        {
            dt_ = static_cast<float>(elapsed_time / 1.0e9);  // 使用实际帧时间作为 dt
        }
    }
}

/**
 * @brief 初始化游戏环境，包括 SDL 子系统、窗口和渲染器
 * 
 * @param title 窗口标题字符串
 * @param width 窗口宽度（像素）
 * @param height 窗口高度（像素）
 * 
 * 【初始化步骤】
 * 1. 设置屏幕尺寸
 * 2. 打印当前工作目录（调试用）
 * 3. 初始化 SDL 视频和音频子系统
 * 4. 初始化 SDL_mixer（支持 MP3 和 OGG 格式）
 * 5. 打开音频设备并分配 16 个通道
 * 6. 设置音乐和音效音量（最大音量的 1/4）
 * 7. 初始化 SDL_ttf 字体库
 * 8. 创建可调整大小的窗口和渲染器
 * 9. 设置渲染器逻辑分辨率（letterbox 模式）
 * 10. 计算帧延迟时间
 * 11. 创建资源管理器
 * 12. 创建并初始化初始场景（SceneMain）
 * 
 * 【错误处理】
 * 每个初始化步骤都包含错误日志输出，但不会中断程序
 */
void Game::init(std::string title, int width, int height)
{
    // 设置屏幕尺寸
    screen_size_ = glm::vec2(width, height);
    
    // 获取并打印当前工作目录
    auto current_path = std::filesystem::current_path();
    std::cout << "Current working directory: " << current_path.string() << std::endl;

    // 初始化 SDL 视频和音频子系统
    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL 初始化失败：%s\n", SDL_GetError());
    }

    // 初始化 SDL_mixer 音频库（支持 MP3 和 OGG 格式）
    if ((Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) & (MIX_INIT_MP3 | MIX_INIT_OGG)) != (MIX_INIT_MP3 | MIX_INIT_OGG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_mixer 初始化失败：%s\n", SDL_GetError());
    }

    // 打开音频设备（默认频率和格式）
    if (!Mix_OpenAudio(0, NULL))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_mixer 打开音频设备失败：%s\n", SDL_GetError());
    }

    // 分配 16 个音频通道并设置音量
    Mix_AllocateChannels(16);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);  // 背景音乐音量
    Mix_Volume(-1, MIX_MAX_VOLUME / 4);   // 音效音量

    // 初始化 SDL_ttf 字体库
    if (!TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_ttf 初始化失败：%s\n", SDL_GetError());
    }

    // 创建可调整大小的窗口和渲染器
    SDL_CreateWindowAndRenderer(title.c_str(), width, height, SDL_WINDOW_RESIZABLE, &window_, &renderer_);
    if (!window_ || !renderer_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "创建窗口或渲染器失败：%s\n", SDL_GetError());
    }

    // 设置渲染器的逻辑分辨率（letterbox 模式保持宽高比）
    SDL_SetRenderLogicalPresentation(renderer_, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    // 计算每帧延迟时间（纳秒）= 1 秒 / 60 帧
    frame_delay_ = 1000000000 / FPS_;

    // 创建资源存储对象（管理纹理、字体、音频资源）
    asset_store_ = new AssetStore(renderer_);

    // 创建并初始化主场景
    current_scene_ = new SceneMain();
    current_scene_->init();
}

/**
 * @brief 处理 SDL 事件队列中的所有事件
 * 
 * 【事件处理】
 * - SDL_EVENT_QUIT: 用户关闭窗口时，设置 isRunning_ = false
 * - 其他事件：委托给当前场景的 handleEvents() 方法处理
 * 
 * 【轮询机制】
 * 使用 SDL_PollEvent() 持续从事件队列中取出事件，直到队列为空
 */
void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            isRunning_ = false;  // 用户请求关闭窗口，结束游戏循环
            break;
        default:
            // 其他事件委托给当前场景处理
            current_scene_->handleEvents(event);
        }
    }
}

/**
 * @brief 更新游戏逻辑
 * 
 * @param dt 时间增量（秒），用于计算基于时间的更新逻辑
 * 
 * 【功能说明】
 * 将更新操作委托给当前场景的 update() 方法
 * 参数 dt 可用于实现与帧率无关的平滑运动
 * 
 * 【当前状态】
 * 参数 dt 已传递给场景，但在场景内部可能未完全利用
 */
void Game::update([[maybe_unused]] float dt)
{
    // 委托给当前场景更新逻辑
    current_scene_->update(dt);
}

/**
 * @brief 渲染游戏画面
 * 
 * 【渲染流程】
 * 1. 设置清屏颜色为黑色（RGBA: 0, 0, 0, 255）
 * 2. 清除渲染器内容
 * 3. 委托给当前场景进行渲染
 * 4. 将渲染结果呈现到屏幕
 */
void Game::render()
{
    // 设置清屏颜色为黑色
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    
    // 清除渲染器内容
    SDL_RenderClear(renderer_);
    
    // 委托给当前场景渲染
    current_scene_->render();
    
    // 将渲染结果呈现到屏幕
    SDL_RenderPresent(renderer_);
}

/**
 * @brief 清理游戏资源
 * 
 * 【清理顺序】（反向清理原则）
 * 1. 清理并删除当前场景
 * 2. 清理并删除资源管理器
 * 3. 销毁渲染器
 * 4. 销毁窗口
 * 5. 关闭音频设备
 * 6. 退出 SDL_mixer
 * 7. 退出 SDL_ttf
 * 8. 退出 SDL 子系统
 * 
 * 【内存安全】
 * 所有指针在释放后都设置为 nullptr，防止悬空指针
 */
void Game::clean()
{
    // 清理场景
    if(current_scene_)
    {
        current_scene_->clean();
        delete current_scene_;
    }
    
    // 清理资源管理器
    if (asset_store_)
    {
        delete asset_store_;
        asset_store_ = nullptr;
    }

    // 销毁渲染器
    if (renderer_)
    {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }

    // 销毁窗口
    if (window_)
    {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }

    // 关闭音频设备和 SDL 子系统
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}

/**
 * @brief 渲染纹理到指定位置和尺寸
 * 
 * @param texture 纹理对象（包含SDL 纹理、源矩形、旋转角度、翻转标志）
 * @param position 渲染位置（屏幕坐标 x, y）
 * @param size 渲染尺寸（宽度 x, 高度 y）
 * 
 * 【功能说明】
 * 使用 SDL_RenderTextureRotated() 绘制纹理，支持：
 * - 缩放：通过 dst_rect 的宽高控制
 * - 旋转：texture.angle 参数
 * - 翻转：texture.is_flip 控制水平翻转
 */
void Game::renderTexture(const Texture& texture, const glm::vec2& position, const glm::vec2& size)
{
    // 构建目标矩形（渲染位置和尺寸）
    SDL_FRect dst_rect = {
        position.x,
        position.y,
        size.x,
        size.y
    };
    
    // 使用 SDL_RenderTextureRotated 绘制纹理
    // 参数：渲染器，纹理，源矩形，目标矩形，旋转角度，旋转中心，翻转方式
    SDL_RenderTextureRotated(renderer_, texture.texture, &texture.src_rect, &dst_rect,
                             texture.angle, nullptr, 
                             texture.is_flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

/**
 * @brief 绘制网格线
 * 
 * @param top_left 网格左上角坐标（世界坐标或屏幕坐标）
 * @param botton_right 网格右下角坐标
 * @param grid_width 网格间距（像素）
 * @param fcolor 网格颜色（RGBA 浮点数）
 * 
 * 【绘制原理】
 * 1. 设置绘制颜色
 * 2. 沿 X 轴方向绘制垂直线
 * 3. 沿 Y 轴方向绘制水平线
 * 4. 恢复绘制颜色为黑色
 * 
 * 【用途】
 * 常用于调试坐标系或作为背景装饰
 */
void Game::drawGrid(const glm::vec2& top_left, const glm::vec2& botton_right, float grid_width, SDL_FColor fcolor)
{
    // 设置绘制颜色
    SDL_SetRenderDrawColorFloat(renderer_, fcolor.r, fcolor.g, fcolor.b, fcolor.a);
    
    // 绘制垂直线（沿 X 轴等间距）
    for (float x = top_left.x; x <= botton_right.x; x += grid_width){
        SDL_RenderLine(renderer_, x, top_left.y, x, botton_right.y);
    }
    
    // 绘制水平线（沿 Y 轴等间距）
    for (float y = top_left.y; y <= botton_right.y; y += grid_width){
        SDL_RenderLine(renderer_, top_left.x, y, botton_right.x, y);
    }
    
    // 恢复绘制颜色为黑色
    SDL_SetRenderDrawColorFloat(renderer_, 0, 0, 0, 1);
}

/**
 * @brief 绘制多层边界矩形框
 * 
 * @param top_left 边界左上角坐标
 * @param botton_right 边界右下角坐标
 * @param boundary_width 边界线宽度（层数）
 * @param fcolor 边界颜色（RGBA 浮点数）
 * 
 * 【绘制原理】
 * 从内向外逐层绘制矩形框，每层向外扩展 1 像素
 * 
 * 【用途】
 * 用于标记重要区域边界或创建视觉边框效果
 */
void Game::drawBoundary(const glm::vec2& top_left, const glm::vec2& botton_right, float boundary_width, SDL_FColor fcolor)
{
    // 设置绘制颜色
    SDL_SetRenderDrawColorFloat(renderer_, fcolor.r, fcolor.g, fcolor.b, fcolor.a);
    
    // 逐层绘制矩形边框
    for (float i = 0; i < boundary_width; i++){
        SDL_FRect rect = {
            top_left.x - i,                      // 左边界向左扩展
            top_left.y - i,                      // 上边界向上扩展
            botton_right.x - top_left.x + 2 * i, // 宽度增加 2*i
            botton_right.y - top_left.y + 2 * i  // 高度增加 2*i
        };
        SDL_RenderRect(renderer_, &rect);
    }
    
    // 恢复绘制颜色为黑色
    SDL_SetRenderDrawColorFloat(renderer_, 0, 0, 0, 1);
}
