/**
 * @file defs.h
 * @brief 类型定义和枚举常量
 * 
 * 【主要内容】
 * 定义对象类型枚举，用于区分不同种类的游戏对象
 */

#ifndef DEFS_H
#define DEFS_H

/**
 * @enum ObjectType
 * @brief 游戏对象类型枚举
 * 
 * 【用途】
 * 用于标识对象的类别，便于场景管理和类型判断
 * 
 * 【类型说明】
 * - NONE: 无类型或默认类型
 * - OBJECT_SCREEN: 屏幕对象（UI 元素，固定于屏幕坐标）
 * - OBJECT_WORLD: 世界对象（游戏世界中的对象，受摄像机影响）
 * 
 * 【使用场景】
 * Scene::addChild() 根据对象类型将对象添加到不同的容器中
 */
enum class ObjectType
{
    NONE,              ///< 无类型或默认类型
    OBJECT_SCREEN,     ///< 屏幕对象：相对于屏幕渲染，不受摄像机影响（如 UI）
    OBJECT_WORLD       ///< 世界对象：在世界坐标系中，受摄像机影响（如玩家、敌人）
};

#endif
