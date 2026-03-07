/**
 * @file ObjectScreen.h
 * @brief 屏幕对象基类 - 相对于屏幕渲染的对象
 * 
 * 【核心职责】
 * 1. 定义屏幕坐标系统中的渲染位置
 * 2. 作为 UI 元素和世界对象的中间基类
 * 
 * 【与 ObjectWorld 的区别】
 * - ObjectScreen: 使用屏幕坐标，不受摄像机影响
 * - ObjectWorld: 使用世界坐标，受摄像机影响
 * 
 * 【继承体系】
 * Object -> ObjectScreen -> ObjectWorld/Actor
 *           |-> ObjectAffiliate -> Sprite/SpriteAnim
 */

#ifndef OBJECT_SCREEN_H
#define OBJECT_SCREEN_H

#include "Object.h"

/**
 * @class ObjectScreen
 * @brief 屏幕坐标系中的对象基类
 * 
 * 【特点】
 * - 渲染位置相对于屏幕左上角（0, 0）
 * - 不受摄像机位置影响
 * - 适合用于 UI 元素、HUD 等固定于屏幕的元素
 * 
 * 【关键属性】
 * - render_position_: 渲染位置（屏幕坐标）
 */
class ObjectScreen : public Object{

protected:
    glm::vec2 render_position_ = glm::vec2(0,0); ///< 渲染位置：屏幕坐标系中的位置（像素）
    
    /**
     * @brief 初始化对象类型
     * 
     * 【功能说明】
     * 将 type_ 设置为 ObjectType::OBJECT_SCREEN
     * 标识这是一个屏幕对象
     */
    virtual void init() override{type_ = ObjectType::OBJECT_SCREEN;}

public:
    // ==================== 获取器和设置器 ====================

    /**
     * @brief 获取渲染位置
     * @return glm::vec2 屏幕坐标位置 (x, y)
     */
    glm::vec2 getRenderPosition() const {return render_position_;}

    /**
     * @brief 设置渲染位置
     * @param render_position 新的屏幕坐标位置
     * 
     * 【用途】
     * 设置对象在屏幕上的显示位置
     * 对于 ObjectWorld 子类，这会同时更新世界坐标
     */
    virtual void setRenderPosition(const glm::vec2 &render_position){
        render_position_ = render_position;
    }
};

#endif //OBJECT_SCREEN_H
