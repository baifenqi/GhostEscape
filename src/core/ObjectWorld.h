/**
 * @file ObjectWorld.h
 * @brief 世界对象基类 - 在世界坐标系中的游戏对象
 * 
 * 【核心特点】
 * 1. 使用世界坐标系（相对于世界原点）
 * 2. 受摄像机位置影响
 * 3. 每帧自动转换世界坐标→屏幕坐标
 * 4. 适合游戏实体（玩家、敌人等）
 */

#ifndef OBJECT_WORLD_H
#define OBJECT_WORLD_H

#include "ObjectSCreen.h"
#include "../affiliate/Collider.h"

class ObjectWorld : public ObjectScreen{
protected:
    glm::vec2 position_ = glm::vec2(0,0);//世界位置
    Collider* collider_ = nullptr;

public:
    virtual void init() override {type_ = ObjectType::OBJECT_WORLD;};
    virtual void update(float dt) override;

    //get and set
    virtual glm::vec2 getPosition() const override {return position_;}
    void setPosition(const glm::vec2 &position);
    virtual void setRenderPosition(const glm::vec2 &render_position) override;

    Collider* getCollider() const {return collider_;}
    void setCollider(Collider *collider) {collider_ = collider;}


};

#endif //OBJECT_WORLD_H