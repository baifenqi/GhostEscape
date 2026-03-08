/**
 * @file Actor.h
 * @brief 演员类 - 具有物理移动能力的世界对象
 * 
 * 【核心职责】
 * 1. 管理速度向量（velocity_）
 * 2. 限制最大速度（max_speed_）
 * 3. 提供基础移动方法 move(dt)
 * 4. 作为可移动游戏实体的基类
 * 
 * 【与 ObjectWorld 的区别】
 * - ObjectWorld: 有世界坐标，但不一定有移动能力
 * - Actor: 专门用于可以主动移动的实体
 * 
 * 【继承体系】
 * Object → ObjectScreen → ObjectWorld → Actor → Player/Enemy
 */

#ifndef ACTOR_H
#define ACTOR_H

#include "ObjectWorld.h"

/**
 * @class Actor
 * @brief 具有物理运动属性的世界对象基类
 * 
 * 【主要功能】
 * 1. 速度管理：维护 velocity_ 向量
 * 2. 速度限制：通过 max_speed_ 控制最快速度
 * 3. 边界移动：move(dt) 方法自动处理世界边界
 * 
 * 【物理属性】
 * - velocity_: 当前速度向量（像素/秒）
 *   - x 分量：水平速度
 *   - y 分量：垂直速度
 * - max_speed_: 最大速度值（像素/秒）
 *   - 限制速度的模长不超过此值
 * 
 * 【典型用途】
 * - Player: 玩家控制的角色
 * - Enemy: AI 控制的敌人
 * - NPC: 非玩家角色
 * 
 * 【关键方法】
 * - move(dt): 根据速度更新位置并限制边界
 */
class Actor : public ObjectWorld{
protected:
    glm::vec2 velocity_ = glm::vec2(0,0);  ///< 速度向量：每秒移动的像素数 (vx, vy)
    float max_speed_ = 100.0f;             ///< 最大速度：速度的模长上限（像素/秒）

public:
    /**
     * @brief 根据速度更新位置（带边界限制）
     * @param dt 时间增量（秒）
     * 
     * 【运动方程】
     * position_new = position_old + velocity × dt
     * 
     * 【执行流程】
     * 1. 计算位移：displacement = velocity × dt
     * 2. 更新位置：position = position + displacement
     * 3. 调用 setPosition() 自动转换屏幕坐标
     * 4. 使用 glm::clamp() 限制在世界边界内
     * 
     * 【边界限制】
     * - 最小值：glm::vec2(0) - 世界左上角
     * - 最大值：getWorldSize() - 世界右下角
     * - 防止对象移出世界范围
     * 
     * 【使用示例】
     * ```cpp
     * // 在 update() 中调用
     * void Actor::update(float dt) {
     *     // ... 其他逻辑
     *     move(dt);  // 应用速度
     * }
     * ```
     * 
     * 【注意事项】
     * - 必须先设置 velocity_ 才能产生移动
     * - 边界限制是硬性的，到达边界后无法继续移动
     */
    void move(float dt);
    
    // ==================== 获取器和设置器 ====================

    /**
     * @brief 获取速度向量
     * @return glm::vec2 速度向量 (vx, vy)
     */
    glm::vec2 getVelocity() const {return velocity_;}

    /**
     * @brief 设置速度向量
     * @param velocity 新的速度向量
     * 
     * 【注意】
     * 直接设置速度，不进行限制
     * 应在调用 move() 前确保速度合理
     */
    void setVelocity(const glm::vec2 &velocity) {velocity_ = velocity;}

    /**
     * @brief 获取最大速度
     * @return float 最大速度值（像素/秒）
     */
    float getMaxSpeed() const {return max_speed_;}

    /**
     * @brief 设置最大速度
     * @param max_speed 新的最大速度值
     * 
     * 【用途】
     * - 调整对象的移动快慢
     * - 不同角色可以有不同的最大速度
     * 
     * 【示例】
     * ```cpp
     * player->setMaxSpeed(600.0f);   // 玩家快速移动
     * enemy->setMaxSpeed(300.0f);    // 敌人慢速移动
     * ```
     */
    void setMaxSpeed(float max_speed) {max_speed_ = max_speed;}
};

#endif //ACTOR_H
