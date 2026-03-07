/**
 * @file Object.h
 * @brief 游戏对象基类 - 所有游戏对象的抽象父类
 * 
 * 【核心职责】
 * 1. 定义游戏对象的基本接口（init, handleEvents, update, render, clean）
 * 2. 管理对象树结构（父子关系）
 * 3. 提供对象类型标识
 * 4. 提供激活/停用控制
 * 
 * 【设计模式】
 * - 组合模式：通过 children_ 容器管理子对象
 * - 模板方法模式：定义生命周期虚函数，子类重写实现具体逻辑
 * 
 * 【继承体系】
 * Object -> ObjectScreen -> ObjectWorld -> Actor -> Player
 * Object -> ObjectAffiliate -> Sprite -> SpriteAnim
 */

#ifndef OBJECT_H
#define OBJECT_H

#include "Game.h"
#include "defs.h"
#include <vector>

/**
 * @class Object
 * @brief 所有游戏对象的基类，定义了对象的生命周期和树形结构
 * 
 * 【生命周期方法】
 * 1. init(): 初始化对象
 * 2. handleEvents(): 处理事件
 * 3. update(dt): 更新逻辑
 * 4. render(): 渲染对象
 * 5. clean(): 清理资源
 * 
 * 【树形结构】
 * 每个对象可以有多个子对象，形成对象树
 * 父对象的操作会自动传播到子对象
 * 
 * 【关键属性】
 * - game_: 游戏实例引用，方便访问全局游戏状态
 * - children_: 子对象列表
 * - type_: 对象类型标识
 * - is_active_: 激活状态标志
 */
class Object
{
protected:
    Game& game_ = Game::getInstance();  ///< 游戏实例引用：访问全局游戏管理器
    std::vector<Object*> children_;     ///< 子对象列表：管理对象的层级结构
    ObjectType type_ = ObjectType::NONE;///< 对象类型：用于区分不同种类的对象
    bool is_active_ = true;             ///< 激活状态：false 时不处理事件、不更新、不渲染

public:
    Object() = default;   ///< 默认构造函数
    virtual ~Object() = default;  ///< 虚析构函数：确保派生类正确释放资源

    /**
     * @brief 初始化对象
     * 
     * 【调用时机】
     * 对象创建后立即调用，用于设置初始状态
     * 
     * 【默认实现】
     * 空函数，子类根据需要重写
     */
    virtual void init(){};

    /**
     * @brief 处理 SDL 事件
     * @param event SDL 事件引用
     * 
     * 【默认实现】
     * 遍历并调用所有活跃子对象的 handleEvents()
     * 
     * 【重写建议】
     * 子类应先调用 Object::handleEvents(event) 以保证子对象能接收事件
     */
    virtual void handleEvents(SDL_Event& event);

    /**
     * @brief 更新对象逻辑
     * @param dt 时间增量（秒）
     * 
     * 【默认实现】
     * 遍历并调用所有活跃子对象的 update()
     * 
     * 【重写建议】
     * 子类应先调用 Object::update(dt) 以保证子对象能正常更新
     */
    virtual void update(float dt);

    /**
     * @brief 渲染对象
     * 
     * 【默认实现】
     * 遍历并调用所有活跃子对象的 render()
     * 
     * 【重写建议】
     * 子类应先调用 Object::render() 以保证子对象能被渲染
     */
    virtual void render();

    /**
     * @brief 清理对象资源
     * 
     * 【默认实现】
     * 调用所有子对象的 clean() 并清空 children_ 容器
     * 
     * 【重写建议】
     * 子类应先调用 Object::clean() 以保证子对象被正确清理
     */
    virtual void clean();

    /**
     * @brief 添加子对象
     * @param child 子对象指针
     * 
     * 【功能说明】
     * 将子对象添加到 children_ 列表中
     * 父对象的操作会自动传播到子对象
     */
    virtual void addChild(Object* child) {children_.push_back(child);}

    /**
     * @brief 移除子对象
     * @param child 要移除的子对象指针
     * 
     * 【功能说明】
     * 从 children_ 列表中删除指定子对象
     * 使用 erase-remove 惯用法安全删除
     */
    virtual void removeChild(Object* child){
        children_.erase(remove(children_.begin(),children_.end(),child),children_.end());
    }

    // ==================== 获取器和设置器 ====================

    /**
     * @brief 获取对象类型
     * @return ObjectType 对象类型枚举值
     */
    ObjectType getType() const {return type_;}

    /**
     * @brief 设置对象类型
     * @param type 要设置的类型
     */
    void setType(ObjectType type) {type_ = type;}

    /**
     * @brief 设置激活状态
     * @param active true=激活，false=停用
     * 
     * 【效果】
     * 停用的对象不会处理事件、不会更新、不会被渲染
     */
    void setActive(bool active) {is_active_ = active;}

    /**
     * @brief 获取激活状态
     * @return bool 当前激活状态
     */
    bool getActive() const {return is_active_;}
};

#endif // OBJECT_H
