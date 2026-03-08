/**
 * @file main.cpp
 * @brief 程序入口文件
 * 
 * 【功能说明】
 * 程序的启动入口，负责初始化和运行游戏主循环
 */

#include "core/Game.h"

/**
 * @brief 程序主函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return int 程序退出码
 * 
 * 【执行流程】
 * 1. 获取 Game 单例引用
 * 2. 初始化游戏窗口（标题：GhostEscape，分辨率：1280x720）
 * 3. 启动游戏主循环（事件处理→逻辑更新→画面渲染）
 * 
 * 【程序生命周期】
 * init() → run() → 主循环 → 清理资源 → 退出
 */
int main(int, char**)
{
    // 获取游戏引擎单例实例
    auto& game = Game::getInstance();
    
    // 初始化 SDL、创建窗口和渲染器、加载资源
    game.init("GhostEscape", 1280, 720);
    
    // 启动游戏主循环（60 FPS）
    game.run();
}
