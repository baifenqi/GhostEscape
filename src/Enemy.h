/**
 * @file Enemy.h
 * @brief 敌人类 - AI 控制的游戏角色
 * 
 * 【核心职责】
 * 1. 实现简单的 AI 追踪逻辑
 * 2. 管理多状态动画（普通/受伤/死亡）
 * 3. 检测与玩家的碰撞
 * 4. 自动移除死亡对象
 * 
 * 【状态机设计】
 * - NORMAL: 正常状态，追踪玩家
 * - HURT: 受伤状态，播放受伤动画
 * - DEAD: 死亡状态，播放死亡动画后移除
 * 
 * 【继承体系】
 * Object → ObjectScreen → ObjectWorld → Actor → Enemy
 */

#ifndef ENEMY_H
#define ENEMY_H

#include "Player.h"

/**
 * @class Enemy
 * @brief 敌人角色类，继承自 Actor
 * 
 * 【主要功能】
 * 1. AI 行为：
 *    - 自动追踪玩家位置
 *    - 检测与玩家的碰撞
 *    - 攻击玩家
 * 
 * 2. 状态管理：
 *    - NORMAL: 正常追踪状态
 *    - HURT: 受击状态
 *    - DEAD: 死亡状态
 * 
 * 3. 动画系统：
 *    - animNormal_: 普通状态动画
 *    - animHurt_: 受伤状态动画
 *    - animDead_: 死亡动画（单次播放）
 * 
 * 4. 碰撞检测：
 *    - collider_: 圆形碰撞体
 *    - 检测与玩家的碰撞
 * 
 * 【关键属性】
 * - currentState_: 当前状态
 * - target_: 追踪目标（玩家指针）
 * - currentAnim_: 当前播放的动画
 * - collider_: 碰撞体组件
 * 
 * 【AI 逻辑】
 * 每帧更新：
 * 1. aimTarget(target_) - 计算朝向玩家的方向
 * 2. velocity_ = direction × max_speed_
 * 3. move(dt) - 应用移动
 * 4. attack() - 检测碰撞并攻击
 */
class Enemy : public Actor
{
private:
    /**
     * @enum State
     * @brief 敌人状态枚举
     */
    enum class State
    {
        NORMAL,     ///< 普通状态：正常追踪玩家
        HURT,       ///< 受伤状态：受到攻击后的硬直
        DEAD        ///< 死亡状态：播放死亡动画后移除
    };
    
    State currentState_ = State::NORMAL;  ///< 当前状态标志

    Player* target_ = nullptr;            ///< 追踪目标：玩家指针
    SpriteAnim* animNormal_ = nullptr;    ///< 普通状态动画精灵
    SpriteAnim* animHurt_ = nullptr;      ///< 受伤状态动画精灵
    SpriteAnim* animDead_ = nullptr;      ///< 死亡动画精灵（单次播放）

    SpriteAnim* currentAnim_ = nullptr;   ///< 当前激活的动画精灵

public:
    /**
     * @brief 初始化敌人对象
     * 
     * 【初始化内容】
     * 1. 调用 Actor::init() 设置类型和基础属性
     * 2. 创建三个状态的动画精灵（2 倍缩放）：
     *    - animNormal_: "assets/sprite/ghost-Sheet.png"
     *    - animHurt_: "assets/sprite/ghostHurt-Sheet.png"
     *    - animDead_: "assets/sprite/ghostDead-Sheet.png"
     * 3. 初始只显示普通动画，隐藏其他
     * 4. 设置 animDead_为单次循环（不重复播放）
     * 5. 设置 currentAnim_ = animNormal_
     * 6. 创建圆形碰撞体（基于动画尺寸）
     * 
     * 【调试日志】
     * 输出动画尺寸、锚点、纹理尺寸、碰撞体尺寸等信息
     * 
     * 【资源路径】
     * 需要三张雪碧图文件：
     * - assets/sprite/ghost-Sheet.png
     * - assets/sprite/ghostHurt-Sheet.png
     * - assets/sprite/ghostDead-Sheet.png
     */
    virtual void init() override;
    
    /**
     * @brief 更新敌人逻辑
     * @param dt 时间增量（秒）
     * 
     * 【更新流程】
     * 1. Actor::update(dt) - 更新子对象
     * 2. aimTarget(target_) - 计算追踪方向
     *    - 如果 target_ 为空，跳过
     *    - 计算归一化方向向量
     *    - 设置 velocity_ = direction × max_speed_
     * 3. move(dt) - 应用移动并限制边界
     * 4. attack() - 检测与玩家的碰撞
     * 
     * 【AI 行为】
     * - 持续追踪玩家位置
     * - 以恒定速度移动
     * - 接触时触发攻击检测
     */
    virtual void update(float dt) override;
    
    /**
     * @brief 渲染敌人
     * 
     * 【渲染流程】
     * 1. Actor::render() - 渲染子对象
     * 2. 当前激活的动画精灵会被渲染
     * 3. 碰撞体在 DEBUG_MODE 下可见
     * 
     * 【图层顺序】
     * 背景 → 网格 → 敌人 → 玩家 → UI
     */
    virtual void render() override;

    /**
     * @brief 瞄准追踪目标
     * @param target 目标玩家指针
     * 
     * 【追踪算法】
     * 1. 计算方向向量：
     *    direction = target->getPosition() - this->getPosition()
     * 2. 归一化：
     *    direction = normalize(direction)
     * 3. 设置速度：
     *    velocity_ = direction × max_speed_
     * 
     * 【效果】
     * 敌人会以恒定速度朝玩家当前位置移动
     * 
     * 【空指针处理】
     * 如果 target == nullptr，直接返回，不做任何操作
     * 
     * 【数学原理】
     * 归一化确保方向向量长度为 1
     * 乘以 max_speed_得到实际速度向量
     */
    void aimTarget(Player* target);

    /**
     * @brief 检查并更新状态
     * 
     * 【当前实现】
     * 空函数，待扩展
     * 
     * 【预期功能】
     * - 检测是否受到伤害
     * - 检测是否死亡
     * - 触发状态切换
     * 
     * 【调用时机】
     * 应在 update() 中调用
     */
    void checkState();
    
    /**
     * @brief 切换状态
     * @param new_state 新的状态值
     * 
     * 【切换逻辑】
     * 1. 如果新状态等于当前状态，直接返回
     * 2. 停用当前动画：currentAnim_->setActive(false)
     * 3. 根据新状态选择动画：
     *    - NORMAL: currentAnim_ = animNormal_
     *    - HURT: currentAnim_ = animHurt_
     *    - DEAD: currentAnim_ = animDead_
     * 4. 激活新动画：currentAnim_->setActive(true)
     * 5. 更新 currentState_ = new_state
     * 
     * 【使用示例】
     * ```cpp
     * changeState(State::HURT);  // 切换到受伤状态
     * changeState(State::DEAD);  // 切换到死亡状态
     * ```
     */
    void changeState(State new_state);
    
    /**
     * @brief 执行攻击检测
     * 
     * 【检测逻辑】
     * 1. 检查自身碰撞体是否存在
     * 2. 检查目标碰撞体是否存在（target_->getCollider()）
     * 3. 调用 isColliding() 检测重叠
     * 4. 如果碰撞，输出日志"Circle vs Circle"
     * 
     * 【当前实现】
     * 仅输出调试日志，未实现实际伤害逻辑
     * 
     * 【扩展建议】
     * - 对玩家造成伤害
     * - 播放攻击特效
     * - 添加攻击冷却时间
     */
    void attack();

    /**
     * @brief 移除死亡的敌人
     * 
     * 【移除条件】
     * 当死亡动画播放完毕（animDead_->getFinish() == true）
     * 
     * 【当前实现】
     * 设置 needMove_ = true（标记需要移除）
     * 
     * 【预期用途】
     * 应在 SceneMain::update() 中检查此标志
     * 如果 needMove_为 true，从场景中删除该敌人
     * 
     * 【内存管理】
     * 被场景删除后会自动调用 clean() 释放资源
     */
    void remove();

    // ==================== 获取器和设置器 ====================

    /**
     * @brief 获取追踪目标
     * @return Player* 目标玩家指针
     */
    Player* getTarget() { return target_; }
    
    /**
     * @brief 设置追踪目标
     * @param target 新的目标玩家指针
     * 
     * 【用途】
     * 动态改变敌人的追踪对象
     */
    void setTarget(Player* target) {target_ = target;}
};

#endif
