/**
 * @file Actor.h
 * @brief 演员类 - 具有物理属性的世界对象
 * 
 * 【核心职责】
 * 1. 管理速度向量（velocity_）
 * 2. 限制最大速度（max_speed_）
 * 3. 作为可移动游戏实体的基类
 * 
 * 【物理特性】
 * - 速度：二维向量（x, y 方向的速度分量）
 * - 最大速度：限制对象的运动速度上限
 * 
 * 【继承体系】
 * Object -> ObjectScreen -> ObjectWorld -> Actor -> Player
 */

#ifndef ACTOR_H
#define ACTOR_H

#include "ObjectWorld.h"

/**
 * @class Actor
 * @brief 具有物理运动属性的世界对象基类
 * 
 * 【特点】
 * - 可以移动（有速度向量）
 * - 速度有限制（不超过 max_speed_）
 * - 适合用于动态游戏实体（玩家、敌人、NPC 等）
 * 
 * 【物理属性】
 * - velocity_: 当前速度向量（像素/秒）
 * - max_speed_: 最大速度限制（像素/秒）
 * 
 * 【典型用途】
 * Player 类继承 Actor，实现玩家角色的移动控制
 */
class Actor : public ObjectWorld{
protected:
    glm::vec2 velocity_ = glm::vec2(0,0);  ///< 速度向量：每秒移动的像素数 (vx, vy)
    float max_speed_ = 100.0f;             ///< 最大速度：速度的模长上限（像素/秒）

public:
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
     * 应在 update() 中根据 max_speed_ 进行限幅
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
     * 调整对象的移动速度上限
     */
    void setMaxSpeed(float max_speed) {max_speed_ = max_speed;}
};

#endif //ACTOR_H
