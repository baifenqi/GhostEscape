/**
 * @file ObjectWorld.h
 * @brief 世界对象基类 - 游戏世界中的对象
 * 
 * 【核心职责】
 * 1. 管理世界坐标系中的位置
 * 2. 实现世界坐标与屏幕坐标的双向转换
 * 3. 受摄像机位置影响
 * 
 * 【与 ObjectScreen 的区别】
 * - ObjectWorld: 使用世界坐标，受摄像机影响（如玩家、敌人）
 * - ObjectScreen: 使用屏幕坐标，不受摄像机影响（如 UI）
 * 
 * 【继承体系】
 * Object -> ObjectScreen -> ObjectWorld -> Actor -> Player
 */

#ifndef OBJECT_WORLD_H
#define OBJECT_WORLD_H

#include "ObjectSCreen.h"

/**
 * @class ObjectWorld
 * @brief 世界坐标系中的对象基类
 * 
 * 【特点】
 * - 位置在世界坐标系中（可以远大于屏幕）
 * - 渲染时会根据摄像机位置转换为屏幕坐标
 * - 适合用于游戏世界中的实体（玩家、NPC、道具等）
 * 
 * 【坐标转换】
 * 屏幕坐标 = 世界坐标 - 摄像机位置
 * 世界坐标 = 屏幕坐标 + 摄像机位置
 * 
 * 【关键属性】
 * - position_: 世界坐标位置
 */
class ObjectWorld : public ObjectScreen{
protected:
    glm::vec2 position_ = glm::vec2(0,0); ///< 世界位置：在游戏世界坐标系中的位置

public:
    /**
     * @brief 初始化对象类型
     * 
     * 【功能说明】
     * 将 type_ 设置为 ObjectType::OBJECT_WORLD
     * 标识这是一个世界对象
     */
    virtual void init() override {type_ = ObjectType::OBJECT_WORLD;}

    /**
     * @brief 更新对象逻辑（自动同步坐标）
     * @param dt 时间增量（秒）
     * 
     * 【核心功能】
     * 每帧自动将世界坐标转换为屏幕坐标
     * render_position_ = worldToScreen(position_)
     * 
     * 【调用时机】
     * 在 Scene::update() 中被调用
     */
    virtual void update(float dt) override;

    // ==================== 获取器和设置器 ====================

    /**
     * @brief 获取世界位置
     * @return glm::vec2 世界坐标位置 (x, y)
     */
    glm::vec2 getPosition() const {return position_;}

    /**
     * @brief 设置世界位置
     * @param position 新的世界坐标位置
     * 
     * 【自动转换】
     * 设置世界位置后，会自动计算对应的屏幕位置
     * screen_position = world_position - camera_position
     */
    void setPosition(const glm::vec2 &position);

    /**
     * @brief 设置渲染位置（反向推算世界位置）
     * @param render_position 屏幕坐标位置
     * 
     * 【自动转换】
     * 设置屏幕位置后，会自动反推出世界位置
     * world_position = screen_position + camera_position
     */
    virtual void setRenderPosition(const glm::vec2 &render_position) override;

};

#endif //OBJECT_WORLD_H
