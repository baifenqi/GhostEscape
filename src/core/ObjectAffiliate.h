/**
 * @file ObjectAffiliate.h
 * @brief 附属对象基类 - 依附于父对象的组件
 * 
 * 【核心特点】
 * 1. 必须依附于父对象（ObjectScreen）
 * 2. 通过 offset_保持相对偏移
 * 3. 用于装饰性组件（如精灵、碰撞体等）
 */

#ifndef OBJECTAFFILIATE_H
#define OBJECTAFFILIATE_H

#include "ObjectScreen.h"

class ObjectAffiliate : public Object
{
protected:
    ObjectScreen *parent_ = nullptr;
    glm::vec2 offset_ = glm::vec2(0.0f);//偏移量
    glm::vec2 size_ = glm::vec2(0.0f);//大小
    Anchor anchor_ = Anchor::CENTER;

public:
    void setOffsetByAnchor(Anchor anchor);
    //get and set
    ObjectScreen *getParent() const {return parent_;}
    void setParent(ObjectScreen *parent) {parent_ = parent;}
    glm::vec2 getOffset() const {return offset_;}
    void setOffset(const glm::vec2 &offset) {offset_ = offset;}

    glm::vec2 getSize() const {return size_;}
    void setSize(const glm::vec2 &size);
    void setScale(float scale);
    Anchor getAnchor() const {return anchor_;}
    void setAnchor(Anchor anchor) {anchor_ = anchor;}





};

#endif // OBJECTAFFILIATE_H