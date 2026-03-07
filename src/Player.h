/**
 * @file Player.h
 * @brief 玩家类 - 用户控制的游戏角色
 * 
 * 【核心职责】
 * 1. 处理键盘输入控制移动
 * 2. 管理两种精灵动画（idle/move）
 * 3. 根据速度自动切换动画状态
 * 4. 控制摄像机跟随玩家
 * 
 * 【物理属性】
 * - 最大速度：600 像素/秒
 * - 加速度：最大速度的 80% 每秒
 * - 阻尼系数：0.9（每帧速度衰减 10%）
 * 
 * 【动画系统】
 * - idle 动画：静止时播放
 * - move 动画：移动时播放
 * - 自动根据速度切换
 * 
 * 【继承体系】
 * Object -> ObjectScreen -> ObjectWorld -> Actor -> Player
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "core/Actor.h"
#include "Affiliate/SpriteAnim.h"

/**
 * @class Player
 * @brief 玩家控制的角色类
 * 
 * 【主要功能】
 * 1. 键盘控制移动（WASD/方向键）
 * 2. 速度累积和衰减（惯性效果）
 * 3. 精灵动画自动切换（idle/move）
 * 4. 摄像机自动跟随
 * 5. 边界限制（不超出世界范围）
 * 
 * 【动画组件】
 * - sprite_idle_: 静止状态的精灵动画
 * - sprite_move_: 移动状态的精灵动画
 * - is_moving_: 当前是否在移动的标记
 * 
 * 【关键方法】
 * - keyboardControl(): 读取键盘输入并施加加速度
 * - move(dt): 根据速度更新位置
 * - syncamera(): 让摄像机跟随玩家
 * - checkState(): 检查状态并切换动画
 */
class Player : public Actor{
private:
    SpriteAnim* sprite_idle_ = nullptr;  ///< 静止动画精灵
    SpriteAnim* sprite_move_ = nullptr;  ///< 移动动画精灵
    bool is_moving_ = false;             ///< 移动状态标志

public:
    /**
     * @brief 初始化玩家对象
     * 
     * 【初始化内容】
     * 1. 调用 Actor::init() 设置类型为 OBJECT_WORLD
     * 2. 设置最大速度为 600 像素/秒
     * 3. 创建 idle 动画精灵（ghost-idle.png，2 倍缩放）
     * 4. 创建 move 动画精灵（ghost-move.png，2 倍缩放）
     * 5. 初始隐藏 move 动画（只显示 idle）
     */
    virtual void init() override;

    /**
     * @brief 处理 SDL 事件
     * @param event SDL 事件引用
     * 
     * 【当前实现】
     * 调用 Actor::handleEvents(event)
     * 实际输入处理在 keyboardControl() 中通过轮询实现
     */
    virtual void handleEvents(SDL_Event& event) override;

    /**
     * @brief 更新玩家逻辑
     * @param dt 时间增量（秒）
     * 
     * 【更新流程】
     * 1. Actor::update(dt) - 更新子对象
     * 2. velocity_ *= 0.9 - 速度衰减（阻尼效果）
     * 3. checkState() - 检查移动状态并切换动画
     * 4. keyboardControl() - 读取键盘输入并加速
     * 5. move(dt) - 根据速度更新位置
     * 6. syncamera() - 更新摄像机位置
     */
    virtual void update(float dt) override;

    /**
     * @brief 渲染玩家
     * 
     * 【当前实现】
     * 调用 Actor::render()
     * 实际渲染由活跃的精灵动画完成
     */
    virtual void render() override;

    /**
     * @brief 清理玩家资源
     * 
     * 【清理内容】
     * 调用 Actor::clean() 清理子对象（精灵动画）
     */
    virtual void clean() override;

    /**
     * @brief 键盘控制移动
     * 
     * 【控制方式】
     * - WASD 键或方向键控制八个方向
     * - 每个方向施加加速度（max_speed_ × 0.8）
     * - 速度有上限（max_speed_）
     * 
     * 【物理特性】
     * 加速度驱动，非直接速度控制
     * 产生平滑的启动和停止效果
     */
    void keyboardControl();

    /**
     * @brief 根据速度更新位置
     * @param dt 时间增量（秒）
     * 
     * 【运动公式】
     * position = position + velocity × dt
     * 
     * 【边界限制】
     * 使用 glm::clamp 将位置限制在 [0, world_size] 范围内
     * 防止玩家移出世界边界
     */
    void move(float dt);

    /**
     * @brief 同步摄像机位置
     * 
     * 【跟随逻辑】
     * 摄像机位置 = 玩家位置 - 屏幕中心
     * 确保玩家始终在屏幕中央
     * 
     * 【调用时机】
     * 每帧更新后调用，实现实时跟随
     */
    void syncamera();

    /**
     * @brief 检查并更新移动状态
     * 
     * 【检查内容】
     * 1. 根据 velocity_.x 设置精灵翻转方向
     * 2. 计算速度模长，判断是否在移动（阈值 0.1）
     * 3. 如果移动状态改变，调用 changeState() 切换动画
     * 
     * 【状态切换】
     * - 速度 > 0.1: 切换到 move 动画
     * - 速度 <= 0.1: 切换到 idle 动画
     */
    void checkState();

    /**
     * @brief 切换动画状态
     * @param is_moving true=移动状态，false=静止状态
     * 
     * 【切换逻辑】
     * - 移动时：
     *   - 激活 move 动画，停用 idle 动画
     *   - 同步帧索引和计时器（保持动画连续性）
     * - 静止时：
     *   - 激活 idle 动画，停用 move 动画
     *   - 同步帧索引和计时器
     * 
     * 【设计意图】
     * 动画切换时保持帧同步，避免视觉跳跃
     */
    void changeState(bool is_moving);
};

#endif //PLAYER_H
