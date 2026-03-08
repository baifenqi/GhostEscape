/**
 * @file Object.h
 * @brief 游戏对象基类 - 所有游戏对象的根类
 * 
 * 【核心职责】
 * 1. 定义对象的生命周期接口（init/update/render/clean）
 * 2. 管理对象树结构（父子关系）
 * 3. 管理对象类型和激活状态
 * 
 * 【设计模式】
 * 组合模式：通过 children_容器管理子对象树
 */

#ifndef OBJECT_H
#define OBJECT_H

#include "Game.h"
#include "defs.h"
#include <vector>

class Object
{
protected:
    Game& game_ = Game::getInstance();  // 获取游戏实例的引用，方便对象访问游戏状态和功能
    std::vector<Object*> children_;
    ObjectType type_ = ObjectType::NONE;//初始物品类型
    bool is_active_ = true;
    bool needMove_ = false;
public:
    Object() = default;  // 默认构造函数
    virtual ~Object() = default;  // 虚析构函数，确保正确释放资源

    virtual void init(){};
    virtual void handleEvents(SDL_Event& event);
    virtual void update(float dt);
    virtual void render();
    virtual void clean();

    virtual void addChild(Object* child) {children_.push_back(child);}
    virtual void removeChild(Object* child){
        children_.erase(remove(children_.begin(),children_.end(),child),children_.end());
    }



    //get and set
    ObjectType getType() const {return type_;}
    void setType(ObjectType type) {type_ = type;}

    void setActive(bool active) {is_active_ = active;}
    bool getActive() const {return is_active_;}

    bool getNeedMove() const {return needMove_;}
    void setNeedMove(bool needMove) {needMove_ = needMove;}
};



#endif // OBJECT_H