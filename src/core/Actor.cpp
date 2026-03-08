/**
 * @file Actor.cpp
 * @brief 演员类的实现文件
 * 
 * 【主要内容】
 * 实现基于速度的移动和边界限制功能
 */

#include "Actor.h"
#include "Scene.h"

/**
 * @brief 根据速度更新位置（带边界限制）
 * 
 * @param dt 时间增量（秒）
 * 
 * 【运动学原理】
 * 使用简单的欧拉积分：
 * position(t+dt) = position(t) + velocity × dt
 * 
 * 【执行步骤】
 * 1. 计算位移向量：
 *    displacement = velocity × dt
 *    - velocity.x * dt: X 方向位移
 *    - velocity.y * dt: Y 方向位移
 * 
 * 2. 更新世界坐标：
 *    position_ = position_ + displacement
 * 
 * 3. 调用父类 setPosition()：
 *    - 自动转换为屏幕坐标
 *    - 更新 render_position_
 * 
 * 4. 边界限制（glm::clamp）：
 *    position_.x = clamp(position_.x, 0, world_size.x)
 *    position_.y = clamp(position_.y, 0, world_size.y)
 * 
 * 【边界处理效果】
 * - 对象到达世界边缘时会被阻挡
 * - 无法移出世界范围（[0, world_size]）
 * - 产生"撞墙"效果
 * 
 * 【坐标系说明】
 * - 输入：velocity 是世界坐标单位（像素/秒）
 * - 输出：position_ 是世界坐标
 * - setPosition() 内部会转换为屏幕坐标
 * 
 * 【性能优化】
 * - glm::clamp 是高效的逐分量操作
 * - 无需额外的条件判断
 * 
 * 【使用示例】
 * ```cpp
 * // 在 Actor 子类的 update() 中
 * void Player::update(float dt) {
 *     // 设置速度（例如通过输入）
 *     velocity_ = glm::vec2(100.0f, 50.0f);
 *     
 *     // 应用移动
 *     move(dt);  // 自动处理边界
 * }
 * ```
 * 
 * 【注意事项】
 * - dt 应为正数，通常由游戏主循环提供
 * - 速度过大可能导致穿墙（隧道效应），需要额外处理
 * - 边界值 (0, 0) 和 (world_size.x, world_size.y) 是硬限制
 */
void Actor::move(float dt)
{
    // 根据速度和 dt 计算位移，更新位置
    // 公式：新位置 = 旧位置 + 速度 × 时间
    setPosition(position_ + velocity_ * dt);

    // 使用 glm::clamp 将位置限制在世界边界内
    // 最小值：glm::vec2(0) - 世界左上角
    // 最大值：getWorldSize() - 世界右下角
    position_ = glm::clamp(position_,
                           glm::vec2(0),  // 最小边界
                           game_.getCurrentScene()->getWorldSize());  // 最大边界
}
