#ifndef ACTOR_H
#define ACTOR_H

#include "ObjectWorld.h"

class Actor : public ObjectWorld{
protected:
    glm::vec2 velocity_ = glm::vec2(0,0);//速度
    float max_speed_ = 100.0f; //最大速度值  
    
public:
    //get and set
    glm::vec2 getVelocity() const {return velocity_;}
    void setVelocity(const glm::vec2 &velocity) {velocity_ = velocity;}
    float getMaxSpeed() const {return max_speed_;}
    void setMaxSpeed(float max_speed) {max_speed_ = max_speed;} 
};

#endif //ACTOR_H