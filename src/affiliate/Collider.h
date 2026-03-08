/**
 * @file Collider.h
 * @brief 碰撞检测组件 - 用于游戏对象的物理碰撞
 * 
 * 【核心职责】
 * 1. 定义碰撞体形状（圆形/矩形）
 * 2. 实现碰撞检测逻辑
 * 3. 作为附属组件挂载到游戏对象
 * 4. 支持调试可视化渲染
 * 
 * 【设计模式】
 * - 组合模式：Collider 作为组件组合到 ObjectAffiliate
 * - 工厂模式：通过静态方法 addColliderChild() 创建
 * 
 * 【使用方式】
 * Collider::addColliderChild(parent, size, type, anchor);
 */

#ifndef COLLIDER_H
#define COLLIDER_H

#include "../core/ObjectAffiliate.h"

/**
 * @class Collider
 * @brief 碰撞检测器类，继承自 ObjectAffiliate
 * 
 * 【主要功能】
 * 1. 支持两种碰撞体类型：
 *    - CIRCLE: 圆形碰撞体（size_.x 为直径）
 *    - RECTANGLE: 矩形碰撞体（待实现）
 * 2. 检测与其他碰撞体的重叠
 * 3. 可选的调试渲染（DEBUG_MODE 宏控制）
 * 
 * 【坐标系统】
 * - 位置：parent_->getPosition() + offset_
 * - 尺寸：size_（世界坐标单位）
 * 
 * 【关键属性】
 * - type_: 碰撞体类型（圆形/矩形）
 * - 继承自 ObjectAffiliate：
 *   - parent_: 父对象指针
 *   - offset_: 相对偏移
 *   - size_: 碰撞体尺寸
 */
class Collider : public ObjectAffiliate
{
public:  // 从 protected 改为 public，便于外部访问
    
    /**
     * @enum Type
     * @brief 碰撞体形状类型
     */
    enum class Type{
        CIRCLE,     ///< 圆形碰撞体：size_的 x 轴为直径，默认 y = x
        RECTANGLE   ///< 矩形碰撞体：size_.x 为宽度，size_.y 为高度
    };

protected:
    Type type_ = Type::CIRCLE;  ///< 碰撞体类型

public:
    /**
     * @brief 渲染碰撞体（仅调试模式）
     * 
     * 【渲染条件】
     * 仅在定义 DEBUG_MODE 宏时才会渲染
     * 
     * 【渲染效果】
     * 使用半透明圆形（alpha=0.3）覆盖在碰撞区域
     * 
     * 【调用时机】
     * 由场景的 render() 自动调用
     * 
     * 【性能影响】
     * 调试模式下会增加绘制开销，生产环境应关闭 DEBUG_MODE
     */
    virtual void render() override;
    
    /**
     * @brief 创建碰撞体并添加为子对象
     * @param parent 父对象指针（ObjectScreen 类型）
     * @param size 碰撞体尺寸（世界坐标单位）
     * @param type 碰撞体类型（默认圆形）
     * @param anchor 锚点位置（默认中心）
     * @return Collider* 新创建的碰撞体指针
     * 
     * 【创建流程】
     * 1. new Collider() - 创建实例
     * 2. setParent(parent) - 设置父对象
     * 3. setAnchor(anchor) - 设置锚点
     * 4. setSize(size) - 设置尺寸（会自动调用 setOffsetByAnchor）
     * 5. setType(type) - 设置碰撞体类型
     * 6. parent->addChild(collider) - 添加到父对象
     * 7. return collider - 返回指针
     * 
     * 【使用示例】
     * ```cpp
     * // 为玩家创建圆形碰撞体
     * Collider* playerCollider = Collider::addColliderChild(
     *     player, 
     *     glm::vec2(50.0f, 50.0f),  // 直径 50
     *     Collider::Type::CIRCLE,
     *     Anchor::CENTER
     * );
     * ```
     * 
     * 【注意事项】
     * - 尺寸单位是世界坐标（像素）
     * - 锚点决定碰撞体相对于父对象的对齐方式
     * - 圆形碰撞体的 size_.y 会被忽略，始终等于 size_.x
     */
    static Collider* addColliderChild(ObjectScreen* parent, glm::vec2 size, Type type = Type::CIRCLE, Anchor anchor = Anchor::CENTER);
    
    /**
     * @brief 检测是否与其他碰撞体重叠
     * @param other 另一个碰撞体指针
     * @return bool true=发生碰撞，false=未碰撞
     * 
     * 【检测逻辑】
     * 当前仅支持圆形 vs 圆形检测：
     * 1. 计算两个圆心的世界坐标
     *    - center1 = this->parent->position + offset + size/2
     *    - center2 = other->parent->position + other->offset + other->size/2
     * 2. 计算圆心距离
     * 3. 判断：distance < (radius1 + radius2)
     * 
     * 【返回值说明】
     * - true: 两个碰撞体有重叠区域
     * - false: 无碰撞（包括 other==nullptr 或类型不支持）
     * 
     * 【性能优化】
     * 可先进行快速排斥测试（如 AABB 包围盒）
     * 
     * 【TODO】
     * - 矩形 vs 矩形检测
     * - 圆形 vs 矩形检测
     */
    bool isColliding(Collider* other);

    // ==================== 获取器和设置器 ====================

    /**
     * @brief 获取碰撞体类型
     * @return Type 碰撞体类型枚举值
     */
    Type getType() const{ return type_; }
    
    /**
     * @brief 设置碰撞体类型
     * @param type 新的碰撞体类型
     */
    void setType(Type type) {type_ = type;}
};

#endif // COLLIDER_H
