/**
 * @file Collider.cpp
 * @brief 碰撞检测组件的实现文件
 * 
 * 【主要内容】
 * 实现碰撞体的渲染、创建和碰撞检测功能
 */

#include "Collider.h"

/**
 * @brief 渲染碰撞体（仅调试模式）
 * 
 * 【渲染条件】
 * 仅在定义 DEBUG_MODE 宏时执行渲染逻辑
 * 
 * 【渲染流程】
 * 1. 调用 ObjectAffiliate::render() - 父类渲染（如果有）
 * 2. 计算实际位置 = parent_->getRenderPosition() + offset_
 * 3. 调用 Game::renderFillCircle() - 绘制半透明圆形
 *    - 位置：计算出的实际位置
 *    - 尺寸：size_（直径）
 *    - 透明度：alpha = 0.3（半透明）
 * 
 * 【视觉效果】
 * 在碰撞体位置绘制一个半透明的红色圆形
 * 用于可视化调试，帮助确认碰撞体位置和大小是否正确
 * 
 * 【性能考虑】
 * - 生产环境应关闭 DEBUG_MODE 避免性能浪费
 * - 调试模式下每帧都会绘制，可能影响帧率
 */
void Collider::render()
{
#ifdef DEBUG_MODE
    // 调用父类渲染方法
    ObjectAffiliate::render();
    
    // 计算实际渲染位置 = 父对象位置 + 偏移量
    auto pos = parent_->getRenderPosition() + offset_;
    
    // 使用游戏引擎的填充圆形函数渲染
    // alpha=0.3 表示半透明效果
    game_.renderFillCircle(pos, size_, 0.3f);
#endif //DEBUG_MODE
}

/**
 * @brief 创建碰撞体并添加为子对象（静态工厂方法）
 * 
 * @param parent 父对象指针
 * @param size 碰撞体尺寸
 * @param type 碰撞体类型
 * @param anchor 锚点位置
 * @return Collider* 新创建的碰撞体指针
 * 
 * 【详细创建步骤】
 * 1. new Collider() - 在堆上创建碰撞体实例
 * 2. setParent(parent) - 建立父子关系
 * 3. setAnchor(anchor) - 设置锚点（决定对齐方式）
 * 4. setSize(size) - 设置尺寸
 *    - 内部会调用 setOffsetByAnchor() 根据锚点计算偏移
 * 5. setType(type) - 设置碰撞体形状
 * 6. parent->addChild(collider) - 添加到父对象的子对象列表
 * 7. return collider - 返回创建的指针
 * 
 * 【双向绑定】
 * - 父对象通过 children_ 容器持有子对象
 * - 子对象通过 parent_ 指针反向引用父对象
 * 
 * 【使用场景】
 * ```cpp
 * // 玩家碰撞体
 * Collider* playerCol = Collider::addColliderChild(
 *     player, 
 *     glm::vec2(50, 50),  // 50x50 像素
 *     Collider::Type::CIRCLE,
 *     Anchor::CENTER      // 中心对齐
 * );
 * 
 * // 敌人碰撞体
 * Collider* enemyCol = Collider::addColliderChild(
 *     enemy,
 *     glm::vec2(40, 40),
 *     Collider::Type::CIRCLE,
 *     Anchor::CENTER
 * );
 * ```
 */
Collider *Collider::addColliderChild(ObjectScreen *parent, glm::vec2 size, Type type, Anchor anchor)
{
    // 创建新的碰撞体实例
    auto collider = new Collider();
    
    // 设置父对象（建立层级关系）
    collider->setParent(parent);
    
    // 设置锚点（决定对齐基准）
    collider->setAnchor(anchor);
    
    // 设置尺寸（会自动根据锚点计算偏移）
    collider->setSize(size);
    
    // 设置碰撞体类型
    collider->setType(type);
    
    // 添加到父对象的子对象列表
    parent->addChild(collider);
    
    // 返回创建的碰撞体指针
    return collider;
}

/**
 * @brief 检测是否与其他碰撞体重叠
 * 
 * @param other 另一个碰撞体指针
 * @return bool true=发生碰撞，false=未碰撞
 * 
 * 【空指针检查】
 * 如果 other == nullptr，直接返回 false
 * 
 * 【圆形 vs 圆形检测流程】
 * 1. 类型检查：确保两个碰撞体都是圆形
 * 2. 计算圆心 1 的世界坐标：
 *    position_1 = this->parent_->getPosition() + offset_ + size_ / 2.0f
 *    - parent_->getPosition(): 父对象世界坐标
 *    - offset_: 相对偏移
 *    - size_ / 2.0f: 半径（从左上角到圆心）
 * 
 * 3. 计算圆心 2 的世界坐标：
 *    position_2 = other->getParent()->getPosition() + other->offset_ + other->size_ / 2.0f
 * 
 * 4. 计算圆心距离：
 *    distance = glm::distance(position_1, position_2)
 * 
 * 5. 碰撞判断：
 *    distance < (radius1 + radius2)
 *    即：distance < size_.x / 2.0f + other->size_.x / 2.0f
 * 
 * 【数学原理】
 * 两个圆相交的充要条件：圆心距离 < 半径之和
 * 
 * 【返回值情况】
 * - true: 两个圆形碰撞体有重叠区域
 * - false: 
 *   - other 为空指针
 *   - 类型不匹配（当前仅支持圆形）
 *   - 圆心距离 >= 半径之和
 * 
 * 【TODO 扩展】
 * - Rectangle vs Rectangle: SAT（分离轴定理）或 AABB 检测
 * - Circle vs Rectangle: 点到矩形最近点距离检测
 */
bool Collider::isColliding(Collider *other)
{
    // 空指针检查
    if(other == nullptr) return false;
    
    // 圆形 vs 圆形检测
    if(type_ == Type::CIRCLE && other->getType() == Type::CIRCLE){
        // 计算第一个圆的圆心世界坐标
        auto position_1 = parent_->getPosition() + offset_ + size_ / 2.0f;
        
        // 计算第二个圆的圆心世界坐标
        auto position_2 = other->getParent()->getPosition() + other->offset_ + other->size_ / 2.0f;

        // 判断圆心距离是否小于半径之和
        return glm::distance(position_1, position_2) < size_.x / 2.0f + other->size_.x / 2.0f;
    }

    // TODO: 其他类型的碰撞检测
    // - Rectangle vs Rectangle
    // - Circle vs Rectangle
    
    return false;
};
