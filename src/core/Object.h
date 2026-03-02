#ifndef OBJECT_H
#define OBJECT_H

#include "Game.h"

class Object
{
protected:
    Game& game_ = Game::getInstance();  // 获取游戏实例的引用，方便对象访问游戏状态和功能

public:
    Object() = default;  // 默认构造函数
    virtual ~Object() = default;  // 虚析构函数，确保正确释放资源

    virtual void init(){};   
    virtual void handleEvents(SDL_Event& event){};  
    virtual void update(float dt){};  
    virtual void render(){};  
    virtual void clean(){};  
};



#endif // OBJECT_H