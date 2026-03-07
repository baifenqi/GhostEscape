/**
 * @file ObjectWorld.cpp
 * @brief 世界对象基类的实现文件
 * 
 * 【主要内容】
 * 实现世界坐标与屏幕坐标的双向转换和自动同步机制
 */

#include "ObjectWorld.h"
#include "Scene.h"

/*
 * 【坐标转换系统说明】
 * 
 * 1. 双向坐标转换：
 *    - 当设置世界位置时，自动更新渲染位置
 *    - 当设置渲染位置时，自动反推世界位置
 * 
 * 2. 自动同步：
 *    - 在 update() 方法中每帧自动同步世界位置和渲染位置
 *    - 确保摄像机移动时，对象的屏幕位置正确更新
 * 
 * 3. 场景关联：
 *    - 通过 Game 类获取当前场景进行坐标转换
 *    - 使用 Scene::worldToScreenPosition() 和 screenToWorldPosition()
 * 
 * 【坐标关系公式】
 * screen_position = world_position - camera_position
 * world_position = screen_position + camera_position
 */

/**
 * @brief 更新对象逻辑（自动同步坐标）
 * 
 * @param dt 时间增量（秒）
 * 
 * 【核心功能】
 * 每帧自动将世界坐标转换为屏幕坐标
 * 
 * 【执行流程】
 * 1. 调用父类 ObjectScreen::update(dt)
 * 2. 获取当前场景
 * 3. 使用场景的 worldToScreenPosition() 转换坐标
 * 4. 更新 render_position_
 * 
 * 【设计意图】
 * 确保摄像机移动时，所有世界对象的屏幕位置都能实时更新
 */
void ObjectWorld::update(float dt)
{
    // 调用父类更新函数
    ObjectScreen::update(dt);
    
    // 世界坐标转屏幕坐标：减去摄像机位置
    // 这样当摄像机向右移动时，物体在屏幕上向左移动
    render_position_ = game_.getCurrentScene()->worldToScreenPosition(position_);
}

/**
 * @brief 设置世界位置
 * 
 * @param position 新的世界坐标位置
 * 
 * 【自动转换】
 * 设置世界位置后，立即计算对应的屏幕位置
 * 
 * 【计算公式】
 * render_position = position - camera_position
 * 
 * 【用途】
 * 移动游戏世界中的对象时使用此方法
 */
void ObjectWorld::setPosition(const glm::vec2 &position)
{
    // 设置世界坐标
    position_ = position;
    
    // 立即转换为屏幕坐标
    render_position_ = game_.getCurrentScene()->worldToScreenPosition(position_);
}

/**
 * @brief 设置渲染位置（反向推算世界位置）
 * 
 * @param render_position 新的屏幕坐标位置
 * 
 * 【自动转换】
 * 设置屏幕位置后，立即反推世界位置
 * 
 * 【计算公式】
 * position = render_position + camera_position
 * 
 * 【用途】
 * 需要固定对象在屏幕上的位置时使用此方法
 * （但会改变其世界位置）
 */
void ObjectWorld::setRenderPosition(const glm::vec2 &render_position)
{
    // 设置屏幕坐标
    render_position_ = render_position;
    
    // 立即反推世界坐标
    position_ = game_.getCurrentScene()->screenToWorldPosition(render_position_);
}
