/**
 * @file Scene.h
 * @brief 场景基类 - 管理游戏场景中的对象和摄像机
 * 
 * 【核心职责】
 * 1. 管理世界对象和屏幕对象两个容器
 * 2. 控制摄像机位置和视野
 * 3. 提供坐标转换功能（世界坐标↔屏幕坐标）
 * 4. 按类型分类管理子对象
 */

#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <vector>
#include "ObjectWorld.h"
#include "Object.h"

class Scene : public Object
{

protected:
    glm::vec2 camera_position_ = glm::vec2(0);// 场景的摄像机位置
    glm::vec2 world_size_ = glm::vec2(0); //世界地图大小
    std::vector<ObjectWorld*> children__world_;// 场景下的世界对象
    std::vector<ObjectScreen*> children__screen_;// 场景下的屏幕对象


public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void init(){};
    virtual void handleEvents(SDL_Event& event);
    virtual void update(float dt);
    virtual void render();
    virtual void clean();

    virtual  void addChild(Object* child) override;
    virtual void removeChild(Object* child) override;

    glm::vec2 worldToScreenPosition(const glm::vec2 &world_position) const {return world_position - camera_position_;}
    glm::vec2 screenToWorldPosition(const glm::vec2 &screen_position) const{return screen_position + camera_position_;}

    //get and set
    glm::vec2 GetCameraPosition() const {return camera_position_;}
    void setCameraPosition(const glm::vec2 &camera_position);

    glm::vec2 getWorldSize() const {return world_size_;}
    void setWorldSize(const glm::vec2 &world_size) {world_size_ = world_size;}
};

#endif // SCENE_H