#ifndef OBJECT_WORLD_H
#define OBJECT_WORLD_H

#include "ObjectSCreen.h"

class ObjectWorld : public ObjectScreen{
protected:
    glm::vec2 position_ = glm::vec2(0,0);//世界位置
public:
    virtual void update(float dt) override;

    //get and set
    glm::vec2 getPosition() const {return position_;}
    void setPosition(const glm::vec2 &position);
    virtual void setRenderPosition(const glm::vec2 &render_position) override;

};

#endif //OBJECT_WORLD_H