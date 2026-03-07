/**
 * @file Object.cpp
 * @brief 游戏对象基类的实现文件
 * 
 * 【主要内容】
 * 实现对象树的事件处理、更新、渲染和清理的递归传播机制
 */

#include "Object.h"

/**
 * @brief 处理 SDL 事件（递归传播到子对象）
 * 
 * @param event SDL 事件引用
 * 
 * 【执行流程】
 * 1. 遍历 children_ 列表中的所有子对象
 * 2. 检查子对象的激活状态（getActive()）
 * 3. 如果激活，调用子对象的 handleEvents(event)
 * 
 * 【设计意图】
 * 事件从父对象向子对象递归传播，形成事件冒泡机制
 */
void Object::handleEvents(SDL_Event &event)
{
    // 遍历所有子对象
    for(auto& child : children_){
        // 只处理活跃的子对象
        if(child->getActive()){
            // 递归调用子对象的事件处理函数
            child->handleEvents(event);
        }
    }
}

/**
 * @brief 更新对象逻辑（递归传播到子对象）
 * 
 * @param dt 时间增量（秒）
 * 
 * 【执行流程】
 * 1. 遍历 children_ 列表中的所有子对象
 * 2. 检查子对象的激活状态
 * 3. 如果激活，调用子对象的 update(dt)
 * 
 * 【设计意图】
 * 确保所有子对象都能随父对象一起更新
 */
void Object::update(float dt)
{
    // 遍历所有子对象
    for(auto& child : children_){
        // 只更新活跃的子对象
        if(child->getActive()){
            // 递归调用子对象的更新函数
            child->update(dt);
        }
    }
}

/**
 * @brief 渲染对象（递归传播到子对象）
 * 
 * 【执行流程】
 * 1. 遍历 children_ 列表中的所有子对象
 * 2. 检查子对象的激活状态
 * 3. 如果激活，调用子对象的 render()
 * 
 * 【设计意图】
 * 确保所有子对象都能随父对象一起被渲染
 */
void Object::render()
{
    // 遍历所有子对象
    for(auto& child : children_){
        // 只渲染活跃的子对象
        if(child->getActive()){
            // 递归调用子对象的渲染函数
            child->render();
        }        
    }
}

/**
 * @brief 清理对象资源（递归清理子对象）
 * 
 * 【执行流程】
 * 1. 遍历 children_ 列表中的所有子对象
 * 2. 调用每个子对象的 clean() 方法
 * 3. 清空 children_ 容器
 * 
 * 【设计意图】
 * 确保释放对象树时，所有子对象都被正确清理
 */
void Object::clean()
{
    // 清理所有子对象
    for(auto& child : children_){
        // 递归调用子对象的清理函数
        child->clean();
    }
    
    // 清空子对象列表
    children_.clear();
}
