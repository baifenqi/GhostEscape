/**
 * @file Scene.cpp
 * @brief 场景基类的实现文件
 * 
 * 【主要内容】
 * 实现场景的事件处理、更新、渲染、清理以及对象管理功能
 */

#include "Scene.h"

/**
 * @brief 处理 SDL 事件（分发到所有活跃子对象）
 * 
 * @param event SDL 事件引用
 * 
 * 【执行流程】
 * 1. 调用 Object::handleEvents(event) 处理普通子对象
 * 2. 遍历 children__screen_ 容器：
 *    - 检查每个屏幕对象的激活状态
 *    - 如果激活，调用其 handleEvents(event)
 * 3. 遍历 children__world_ 容器：
 *    - 检查每个世界对象的激活状态
 *    - 如果激活，调用其 handleEvents(event)
 * 
 * 【设计意图】
 * 确保场景中的所有活跃对象都能接收到事件
 */
void Scene::handleEvents(SDL_Event &event)
{
    // 处理父类中的普通子对象
    Object::handleEvents(event);
    
    // 处理所有活跃的屏幕对象
    for (auto child : children__screen_)
    {
        if(child->getActive())
            child->handleEvents(event);
    }
    
    // 处理所有活跃的世界对象
    for (auto child : children__world_)
    {
        if(child->getActive()){
            child->handleEvents(event);
        }
    }
}

/**
 * @brief 更新场景逻辑（更新所有活跃子对象）
 * 
 * @param dt 时间增量（秒）
 * 
 * 【执行流程】
 * 1. 调用 Object::update(dt) 更新普通子对象
 * 2. 遍历 children__screen_ 容器：
 *    - 检查每个屏幕对象的激活状态
 *    - 如果激活，调用其 update(dt)
 * 3. 遍历 children__world_ 容器：
 *    - 检查每个世界对象的激活状态
 *    - 如果激活，调用其 update(dt)
 * 
 * 【设计意图】
 * 确保场景中的所有活跃对象都能随时间更新状态
 */
void Scene::update(float dt)
{
    // 更新父类中的普通子对象
    Object::update(dt);
    
    // 更新所有活跃的屏幕对象
    for (auto child : children__screen_)
    {
        if(child->getActive()){
            child->update(dt);
        }
    }
    
    // 更新所有活跃的世界对象
    for (auto child : children__world_)
    {
        if(child->getActive()){
            child->update(dt);
        }
    }
}

/**
 * @brief 渲染场景（渲染所有活跃子对象）
 * 
 * 【执行流程】
 * 1. 调用 Object::render() 渲染普通子对象
 * 2. 遍历 children__screen_ 容器：
 *    - 检查每个屏幕对象的激活状态
 *    - 如果激活，调用其 render()
 * 3. 遍历 children__world_ 容器：
 *    - 检查每个世界对象的激活状态
 *    - 如果激活，调用其 render()
 * 
 * 【渲染顺序】
 * 普通子对象 -> 屏幕对象 -> 世界对象
 */
void Scene::render()
{
    // 渲染父类中的普通子对象
    Object::render();
    
    // 渲染所有活跃的屏幕对象
    for (auto child : children__screen_)
    {
        if(child->getActive())
        {
            child->render();
        }
    }
    
    // 渲染所有活跃的世界对象
    for (auto child : children__world_)
    {
        if(child->getActive())
        {
            child->render();
        }
    }
}

/**
 * @brief 清理场景资源（递归清理所有子对象）
 * 
 * 【执行流程】
 * 1. 调用 Object::clean() 清理普通子对象
 * 2. 遍历 children__screen_ 容器：
 *    - 调用每个屏幕对象的 clean()
 *    - 清空容器
 * 3. 遍历 children__world_ 容器：
 *    - 调用每个世界对象的 clean()
 *    - 清空容器
 * 
 * 【内存安全】
 * 确保所有动态分配的对象都被正确释放
 */
void Scene::clean()
{
    // 清理父类中的普通子对象
    Object::clean();
    
    // 清理所有屏幕对象
    for (auto child : children__screen_)
    {
        child->clean();
    }
    children__screen_.clear();
    
    // 清理所有世界对象
    for (auto child : children__world_)
    {
        child->clean();
    }
    children__world_.clear();
}

/**
 * @brief 添加子对象（按类型分类存储）
 * 
 * @param child 要添加的子对象指针
 * 
 * 【分类规则】
 * 根据对象的 type_ 属性将其添加到不同的容器中：
 * - OBJECT_SCREEN: 加入 children__screen_（强制类型转换）
 * - OBJECT_WORLD: 加入 children__world_（强制类型转换）
 * - 其他类型：加入 children_（父类的容器）
 * 
 * 【类型安全】
 * 使用 dynamic_cast 进行安全的向下转型
 */
void Scene::addChild(Object *child)
{
    switch (child->getType())
    {
    case ObjectType::OBJECT_SCREEN:
        // 屏幕对象添加到屏幕容器
        children__screen_.push_back(dynamic_cast<ObjectScreen*>(child));
        break;
    case ObjectType::OBJECT_WORLD:
        // 世界对象添加到世界容器
        children__world_.push_back(dynamic_cast<ObjectWorld*>(child));
        break;
    default:
        // 其他类型添加到父类容器
        children_.push_back(child);
        break;
    }
}

/**
 * @brief 移除子对象（从对应容器中删除）
 * 
 * @param child 要移除的子对象指针
 * 
 * 【删除规则】
 * 根据对象的 type_ 属性从对应的容器中删除：
 * - OBJECT_SCREEN: 从 children__screen_ 删除
 * - OBJECT_WORLD: 从 children__world_ 删除
 * - 其他类型：从 children_ 删除
 * 
 * 【实现技巧】
 * 使用 erase-remove 惯用法安全删除向量中的元素
 */
void Scene::removeChild(Object *child)
{
    switch (child->getType())
    {
    case ObjectType::OBJECT_SCREEN:
        // 从屏幕容器中删除
        children__screen_.erase(
            remove(children__screen_.begin(), children__screen_.end(), 
                   dynamic_cast<ObjectScreen*>(child)),
            children__screen_.end());
        break;
    case ObjectType::OBJECT_WORLD:
        // 从世界容器中删除
        children__world_.erase(
            remove(children__world_.begin(), children__world_.end(), 
                   dynamic_cast<ObjectWorld*>(child)),
            children__world_.end());
        break;
    default:
        // 从父类容器中删除
        children_.erase(remove(children_.begin(), children_.end(), child), 
                       children_.end());
        break;
    }
}

/**
 * @brief 设置摄像机位置（带边界限制）
 * 
 * @param camera_position 新的摄像机世界坐标
 * 
 * 【边界限制】
 * 使用 glm::clamp 将摄像机位置限制在合法范围内：
 * - 最小值：glm::vec2(-300)
 *   允许摄像机稍微移出世界左/上边界
 * - 最大值：world_size_ - game_.getScreenSize() + glm::vec2(300)
 *   允许摄像机稍微移出世界右/下边界
 * 
 * 【设计意图】
 * 防止摄像机完全移出世界范围，同时提供一定的观察余量
 */
void Scene::setCameraPosition(const glm::vec2 &camera_position)
{
    // 直接赋值
    camera_position_ = camera_position;
    
    // 限制摄像机在世界边界内（允许±300 像素的余量）
    camera_position_ = glm::clamp(camera_position_,
                                  glm::vec2(-300),
                                  world_size_ - game_.getScreenSize() + glm::vec2(300));
}
