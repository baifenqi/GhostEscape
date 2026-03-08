/**
 * @file defs.h
 * @brief 类型定义和枚举常量
 * 
 * 【主要内容】
 * 定义项目中使用的全局枚举类型和宏定义
 */

#ifndef DEFS_H
#define DEFS_H

/// 调试模式开关：定义后启用调试渲染（如碰撞体可视化）
#define DEBUG_MODE

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

/**
 * @enum Anchor
 * @brief 锚点位置枚举，用于确定对象的对齐基准点
 * 
 * 【用途】
 * 控制对象相对于其父对象的对齐方式
 * 通过 setOffsetByAnchor() 自动计算偏移量
 * 
 * 【9 个锚点位置】
 * ```
 * TOP_LEFT      TOP_CENTER      TOP_RIGHT
 * CENTER_LEFT   CENTER          CENTER_RIGHT
 * BOTTOM_LEFT   BOTTOM_CENTER   BOTTOM_RIGHT
 * ```
 * 
 * 【应用场景】
 * - 精灵渲染：确定显示中心点
 * - 碰撞体对齐：确保碰撞区域与视觉匹配
 * - UI 布局：快速定位界面元素
 */
enum class Anchor
{
    NONE,           ///< 无锚点（默认值）
    
    /// 顶部锚点组
    TOP_LEFT,       ///< 左上角
    TOP_CENTER,     ///< 顶部中心
    TOP_RIGHT,      ///< 右上角
    
    /// 中心锚点组
    CENTER_LEFT,    ///< 左侧中心
    CENTER,         ///< 正中心（最常用）
    CENTER_RIGHT,   ///< 右侧中心
    
    /// 底部锚点组
    BOTTOM_LEFT,    ///< 左下角
    BOTTOM_CENTER,  ///< 底部中心
    BOTTOM_RIGHT    ///< 右下角
};

#endif
