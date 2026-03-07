/**
 * @file main.cpp
 * @brief 程序入口文件，启动游戏主循环
 * 
 * 这是游戏的启动文件，负责创建游戏实例并初始化运行。
 * 作为整个程序的入口点，它启动了游戏的核心引擎。
 */

#include "core/Game.h"

/**
 * @brief 程序主函数
 * @param argc 命令行参数数量（未使用）
 * @param argv 命令行参数数组（未使用）
 * @return int 程序退出状态码
 * 
 * 【功能说明】
 * 1. 获取游戏单例实例（Game::getInstance()）
 * 2. 初始化游戏窗口：标题"GhostEscape"，分辨率 1280x720
 * 3. 启动游戏主循环（game.run()）
 * 
 * 【执行流程】
 * main() → Game.init() → Game.run() → [handleEvents(), update(), render()] 循环
 */
int main(int, char**)
{
    // 获取游戏单例实例（全局唯一的游戏管理器）
    auto& game = Game::getInstance();
    
    // 初始化游戏：创建窗口、渲染器、音频设备等
    // 参数：窗口标题，宽度 (1280), 高度 (720)
    game.init("GhostEscape", 1280, 720);
    
    // 启动游戏主循环：持续处理事件、更新逻辑、渲染画面
    // 直到用户关闭窗口或游戏结束
    game.run();
}
