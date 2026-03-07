/**
 * @file Scene.h
 * @brief 场景基类 - 管理游戏场景和摄像机
 * 
 * 【核心职责】
 * 1. 管理场景中的对象（世界对象和屏幕对象）
 * 2. 管理摄像机位置（视图控制）
 * 3. 管理世界地图大小
 * 4. 提供坐标转换功能
 * 
 * 【设计模式】
 * - 组合模式：包含多个 ObjectWorld 和 ObjectScreen 对象
 * - 模板方法模式：定义场景生命周期虚函数
 * 
 * 【继承体系】
 * Object -> Scene -> SceneMain
 */

#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <vector>
#include "ObjectWorld.h"
#include "Object.h"

/**
 * @class Scene
 * @brief 场景基类，管理游戏世界和其中的对象
 * 
 * 【主要功能】
 * 1. 管理两类对象：
 *    - children__world_: 世界对象列表（受摄像机影响）
 *    - children__screen_: 屏幕对象列表（不受摄像机影响）
 * 2. 管理摄像机位置（camera_position_）
 * 3. 管理世界地图大小（world_size_）
 * 4. 提供世界坐标与屏幕坐标的转换
 * 
 * 【坐标系说明】
 * - 世界坐标：游戏世界的绝对坐标，范围由 world_size_ 定义
 * - 屏幕坐标：相对于屏幕左上角的坐标
 * - 转换公式：screen = world - camera
 * 
 * 【关键属性】
 * - camera_position_: 摄像机在世界坐标中的位置
 * - world_size_: 世界地图的尺寸
 * - children__world_: 世界对象容器
 * - children__screen_: 屏幕对象容器
 */
class Scene : public Object
{

protected:
    glm::vec2 camera_position_ = glm::vec2(0);   ///< 摄像机位置：世界坐标中的观察点位置
    glm::vec2 world_size_ = glm::vec2(0);        ///< 世界地图大小：游戏世界的宽度和高度
    std::vector<ObjectWorld*> children__world_;  ///< 世界对象列表：受摄像机影响的对象
    std::vector<ObjectScreen*> children__screen_;///< 屏幕对象列表：不受摄像机影响的对象

public:
    Scene() = default;      ///< 默认构造函数
    virtual ~Scene() = default;  ///< 虚析构函数

    /**
     * @brief 初始化场景
     * 
     * 【调用时机】
     * 场景创建后立即调用
     * 
     * 【默认实现】
     * 空函数，子类需要重写以设置初始状态
     */
    virtual void init(){};

    /**
     * @brief 处理 SDL 事件
     * @param event SDL 事件引用
     * 
     * 【执行流程】
     * 1. 调用 Object::handleEvents(event) 处理普通子对象
     * 2. 遍历并处理所有活跃的屏幕对象
     * 3. 遍历并处理所有活跃的世界对象
     */
    virtual void handleEvents(SDL_Event& event);

    /**
     * @brief 更新场景逻辑
     * @param dt 时间增量（秒）
     * 
     * 【执行流程】
     * 1. 调用 Object::update(dt) 更新普通子对象
     * 2. 更新所有活跃的屏幕对象
     * 3. 更新所有活跃的世界对象
     */
    virtual void update(float dt);

    /**
     * @brief 渲染场景
     * 
     * 【执行流程】
     * 1. 调用 Object::render() 渲染普通子对象
     * 2. 渲染所有活跃的屏幕对象
     * 3. 渲染所有活跃的世界对象
     */
    virtual void render();

    /**
     * @brief 清理场景资源
     * 
     * 【清理内容】
     * 1. 调用 Object::clean() 清理普通子对象
     * 2. 清理所有屏幕对象并清空容器
     * 3. 清理所有世界对象并清空容器
     */
    virtual void clean();

    /**
     * @brief 添加子对象（按类型分类存储）
     * @param child 要添加的子对象指针
     * 
     * 【分类规则】
     * - OBJECT_SCREEN: 加入 children__screen_
     * - OBJECT_WORLD: 加入 children__world_
     * - 其他类型：加入 children_（父类的容器）
     */
    virtual  void addChild(Object* child) override;

    /**
     * @brief 移除子对象（从对应容器中删除）
     * @param child 要移除的子对象指针
     * 
     * 【删除规则】
     * 根据对象类型从对应的容器中删除
     * 使用 erase-remove 惯用法
     */
    virtual void removeChild(Object* child) override;

    // ==================== 坐标转换函数 ====================

    /**
     * @brief 世界坐标转屏幕坐标
     * @param world_position 世界坐标位置
     * @return glm::vec2 屏幕坐标位置
     * 
     * 【转换公式】
     * screen_position = world_position - camera_position
     * 
     * 【用途】
     * 将世界中的对象位置转换为屏幕上的渲染位置
     */
    glm::vec2 worldToScreenPosition(const glm::vec2 &world_position) const {
        return world_position - camera_position_;
    }

    /**
     * @brief 屏幕坐标转世界坐标
     * @param screen_position 屏幕坐标位置
     * @return glm::vec2 世界坐标位置
     * 
     * 【转换公式】
     * world_position = screen_position + camera_position
     * 
     * 【用途】
     * 将屏幕输入（如鼠标点击）转换为世界坐标
     */
    glm::vec2 screenToWorldPosition(const glm::vec2 &screen_position) const {
        return screen_position + camera_position_;
    }

    // ==================== 获取器和设置器 ====================

    /**
     * @brief 获取摄像机位置
     * @return glm::vec2 摄像机世界坐标
     */
    glm::vec2 GetCameraPosition() const {return camera_position_;}

    /**
     * @brief 设置摄像机位置
     * @param camera_position 新的摄像机世界坐标
     * 
     * 【边界限制】
     * 摄像机会被限制在合法范围内：
     * - 最小值：(-300, -300)
     * - 最大值：world_size_ - screen_size + (300, 300)
     * 防止摄像机移出世界边界
     */
    void setCameraPosition(const glm::vec2 &camera_position);

    /**
     * @brief 获取世界地图大小
     * @return glm::vec2 世界地图的宽度和高度
     */
    glm::vec2 getWorldSize() const {return world_size_;}

    /**
     * @brief 设置世界地图大小
     * @param world_size 新的世界地图尺寸
     */
    void setWorldSize(const glm::vec2 &world_size) {world_size_ = world_size;}
};

#endif // SCENE_H
