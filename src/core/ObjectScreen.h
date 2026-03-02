#ifndef OBJECT_SCREEN_H
#define OBJECT_SCREEN_H

#include "Object.h"

class ObjectScreen : public Object{

protected:
    glm::vec2 render_position_ = glm::vec2(0,0); //渲染位置(屏幕位置)

public:
    //get and set
    glm::vec2 getRenderPosition() const {return render_position_;}
    virtual void setRenderPosition(const glm::vec2 &render_position){render_position_ = render_position;} 
};

#endif //OBJECT_SCREEN_H