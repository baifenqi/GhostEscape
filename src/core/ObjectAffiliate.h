/**
 * @file ObjectAffiliate.h
 * @brief 附属对象类 - 依附于其他对象的装饰性对象
 * 
 * 【核心职责】
 * 1. 管理相对于父对象的偏移位置
 * 2. 管理自身尺寸
 * 3. 作为 Sprite 和 SpriteAnim 的基类
 * 
 * 【使用场景】
 * - 角色的装备、装饰品
 * - 物体的附件（如血条、名称标签）
 * - 复合对象的组成部分
 * 
 * 【继承体系】
 * Object -> ObjectAffiliate -> Sprite -> SpriteAnim
 */

#ifndef OBJECTAFFILIATE_H
#define OBJECTAFFILIATE_H

#include "ObjectScreen.h"

/**
 * @class ObjectAffiliate
 * @brief 附属对象基类，依附于父对象的装饰性或功能性组件
 * 
 * 【特点】
 * - 位置相对于父对象（通过 offset_ 偏移）
 * - 可以有自己的尺寸
 * - 通常用于视觉效果或 UI 元素
 * 
 * 【坐标系统】
 * 实际渲染位置 = parent_->getRenderPosition() + offset_
 * 
 * 【关键属性】
 * - parent_: 父对象指针（ObjectScreen 类型）
 * - offset_: 相对于父对象的偏移量
 * - size_: 对象的尺寸（宽度和高度）
 */
class ObjectAffiliate : public Object
{
protected:
    ObjectScreen *parent_ = nullptr;        ///< 父对象指针：被依附的对象
    glm::vec2 offset_ = glm::vec2(0.0f);    ///< 偏移量：相对于父对象的位置偏移
    glm::vec2 size_ = glm::vec2(0.0f);      ///< 大小：对象的宽度和高度（像素）

public:
    // ==================== 获取器和设置器 ====================

    /**
     * @brief 获取父对象
     * @return ObjectScreen* 父对象指针
     */
    ObjectScreen *getParent() const {return parent_;}

    /**
     * @brief 设置父对象
     * @param parent 新的父对象指针
     * 
     * 【用途】
     * 建立附属关系，使此对象跟随父对象移动
     */
    void setParent(ObjectScreen *parent) {parent_ = parent;}

    /**
     * @brief 获取偏移量
     * @return glm::vec2 相对于父对象的偏移 (x, y)
     */
    glm::vec2 getOffset() const {return offset_;}

    /**
     * @brief 设置偏移量
     * @param offset 新的偏移量
     * 
     * 【用途】
     * 调整附属对象相对于父对象的位置
     */
    void setOffset(const glm::vec2 &offset) {offset_ = offset;}

    /**
     * @brief 获取对象尺寸
     * @return glm::vec2 对象的宽度 (x) 和高度 (y)
     */
    glm::vec2 getSize() const {return size_;}

    /**
     * @brief 设置对象尺寸
     * @param size 新的尺寸（宽度和高度）
     * 
     * 【用途】
     * 设置对象的显示大小
     */
    void setSize(const glm::vec2 &size) {size_ = size;}
};

#endif // OBJECTAFFILIATE_H
