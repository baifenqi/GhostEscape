#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <vector>

#include "Object.h"

class Scene : public Object
{

protected:
    glm::vec2 camera_position_ = glm::vec2(0);// 场景的摄像机位置
    glm::vec2 world_size_ = glm::vec2(0); //世界地图大小 
    std::vector<Object*> objects_;  // 场景中的对象列表
     

public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void init(){};
    virtual void update(float dt){};
    virtual void render(){};
    virtual void handleEvents(SDL_Event& event){};
    virtual void clean(){};  

    glm::vec2 worldToScreenPosition(const glm::vec2 &world_position) const {return world_position - camera_position_;}
    glm::vec2 screenToWorldPosition(const glm::vec2 &screen_position) const{return screen_position + camera_position_;}

    //get and set
    glm::vec2 GetCameraPosition() const {return camera_position_;}
    void setCameraPosition(const glm::vec2 &camera_position);

    glm::vec2 getWorldSize() const {return world_size_;}
    void setWorldSize(const glm::vec2 &world_size) {world_size_ = world_size;} 
};

#endif // SCENE_H