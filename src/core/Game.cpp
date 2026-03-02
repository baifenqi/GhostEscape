#include "Game.h"
#include "../SceneMain.h"

/**
 * @brief 主游戏循环函数，控制游戏的运行流程。
 * 
 * 该函数通过一个无限循环来持续处理事件、更新游戏状态和渲染画面，
 * 直到游戏结束标志 [isRunning_](file://d:\codelearng\GhostEscape\src\Game.h#L12-L12) 被设置为 false。
 */
void Game::run()
{
    while (isRunning_)
    {
        Uint64 start_time = SDL_GetTicks();
        handleEvents();  // 处理用户输入和系统事件
        update(dt_);   // 更新游戏逻辑（当前未使用时间差参数）
        render();        // 渲染游戏画面
        Uint64 end_time = SDL_GetTicks();
        auto elapsed_time = end_time - start_time;
        if (elapsed_time < frame_delay_)
        {
            SDL_DelayNS((frame_delay_ - elapsed_time));  // 延迟以维持稳定的帧率
            dt_ = static_cast<float>(frame_delay_ / 1.0e9);  // 计算时间差（秒）
        }
        else
        {
            dt_ = static_cast<float>(elapsed_time / 1.0e9);  // 计算时间差（秒）
        }
        
    }
}

/**
 * @brief 初始化游戏环境，包括 SDL 子系统、窗口和渲染器。
 * 
 * @param title 窗口标题字符串
 * @param width 窗口宽度
 * @param height 窗口高度
 * 
 * 该函数负责初始化 SDL 视频、音频、字体等子系统，并创建游戏窗口和渲染器。
 * 同时设置音频通道数量和音量，以及渲染器的逻辑分辨率。
 */
void Game::init(std::string title, int width, int height)
{
    screen_size_ = glm::vec2(width, height);

    // 初始化 SDL 视频和音频子系统
    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL初始化失败: %s\n", SDL_GetError());
    }

    // 初始化 SDL_mixer 音频库
    if ((Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) & (MIX_INIT_MP3 | MIX_INIT_OGG)) != (MIX_INIT_MP3 | MIX_INIT_OGG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_mixer初始化失败: %s\n", SDL_GetError());
    }

    // 打开音频设备
    if (!Mix_OpenAudio(0, NULL))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_mixer打开音频设备失败: %s\n", SDL_GetError());
    }

    // 分配音频通道并设置音量
    Mix_AllocateChannels(16);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_Volume(-1, MIX_MAX_VOLUME / 4);

    // 初始化 SDL_ttf 字体库
    if (!TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_ttf初始化失败: %s\n", SDL_GetError());
    }

    // 创建窗口和渲染器
    SDL_CreateWindowAndRenderer(title.c_str(), width, height, SDL_WINDOW_RESIZABLE, &window_, &renderer_);
    if (!window_ || !renderer_)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "创建窗口或渲染器失败: %s\n", SDL_GetError());
    }

    // 设置渲染器的逻辑分辨率和显示模式
    SDL_SetRenderLogicalPresentation(renderer_, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    frame_delay_ = 1000000000 / FPS_;  // 计算每帧延迟时间（纳秒）

    // 创建场景
    current_scene_ = new SceneMain();
    current_scene_->init();
}

/**
 * @brief 处理 SDL 事件队列中的所有事件。
 * 
 * 当前仅处理退出事件（SDL_EVENT_QUIT），将游戏运行标志 [isRunning_](file://d:\codelearng\GhostEscape\src\Game.h#L12-L12) 设置为 false。
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
            current_scene_->handleEvents(event);
        }
    }
}

/**
 * @brief 更新游戏逻辑。
 * 
 * @param dt 时间差（秒），用于计算基于时间的更新逻辑（当前未使用）。
 * 
 * 此函数预留了时间差参数，可用于实现与帧率无关的游戏逻辑更新。
 */
void Game::update([[maybe_unused]] float dt)
{
    current_scene_->update(dt);
}

/**
 * @brief 渲染游戏画面。
 * 
 * 当前函数为空，需根据具体需求实现渲染逻辑。
 */
void Game::render()
{
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);  // 设置清屏颜色为黑色
    SDL_RenderClear(renderer_);  // 清除渲染器内容
    current_scene_->render();
    SDL_RenderPresent(renderer_);
}

/**
 * @brief 清理游戏资源。
 * 
 * 当前函数为空，需根据具体需求释放已分配的资源（如纹理、音频等）。
 */
void Game::clean()
{
    if(current_scene_)
    {
        current_scene_->clean();
        delete current_scene_;       
    }

    if (renderer_)
    {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }

    if (window_)
    {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }

    Mix_CloseAudio();    
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();    
}

void Game::drawGrid(const glm::vec2 &top_left, const glm::vec2 &botton_right, float grid_width, SDL_FColor fcolor)
{
    SDL_SetRenderDrawColorFloat(renderer_, fcolor.r, fcolor.g, fcolor.b, fcolor.a);
    for (float x = top_left.x; x <= botton_right.x; x += grid_width){
        SDL_RenderLine(renderer_, x, top_left.y, x, botton_right.y);
    }
    for (float y = top_left.y; y <= botton_right.y; y += grid_width){
        SDL_RenderLine(renderer_, top_left.x, y, botton_right.x, y);
    }
    SDL_SetRenderDrawColorFloat(renderer_, 0, 0, 0, 1);
}

void Game::drawBoundary(const glm::vec2 &top_left, const glm::vec2 &botton_right, float boundary_width, SDL_FColor fcolor)
{
    SDL_SetRenderDrawColorFloat(renderer_, fcolor.r, fcolor.g, fcolor.b, fcolor.a);
    for (float i = 0; i < boundary_width; i++){
        SDL_FRect rect = {
            top_left.x - i,
            top_left.y - i,
            botton_right.x - top_left.x + 2 * i,
            botton_right.y - top_left.y + 2 * i
        };
        SDL_RenderRect(renderer_, &rect);
    }
    SDL_SetRenderDrawColorFloat(renderer_, 0, 0, 0, 1);
}