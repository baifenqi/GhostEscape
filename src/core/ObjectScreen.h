/**
 * @file ObjectScreen.h
 * @brief 屏幕对象基类 - 相对于屏幕渲染的对象
 * 
 * 【核心特点】
 * 1. 使用屏幕坐标系（相对于屏幕左上角）
 * 2. 不受摄像机位置影响
 * 3. 适合 UI、HUD 等固定屏幕元素
 */

#ifndef OBJECT_SCREEN_H
#define OBJECT_SCREEN_H

#include "Object.h"

class ObjectScreen : public Object{

protected:
    glm::vec2 render_position_ = glm::vec2(0,0); //渲染位置(屏幕位置)
    virtual void init() override{type_ = ObjectType::OBJECT_SCREEN;}
public:
    //get and set
    glm::vec2 getRenderPosition() const {return render_position_;}
    virtual void setRenderPosition(const glm::vec2 &render_position){render_position_ = render_position;}

    virtual glm::vec2 getPosition() const {return glm::vec2(0,0);}
};

#endif //OBJECT_SCREEN_H