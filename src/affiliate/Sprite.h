/**
 * @file Sprite.h
 * @brief 精灵类 - 2D 图像渲染对象
 * 
 * 【核心职责】
 * 1. 管理纹理资源（Texture 结构体）
 * 2. 管理渲染尺寸和缩放
 * 3. 管理旋转角度和翻转状态
 * 4. 作为父对象的附属组件进行渲染
 * 
 * 【设计模式】
 * - 附属模式：Sprite 总是依附于 ObjectScreen 对象
 * - 位置相对于父对象（通过 offset_）
 * 
 * 【继承体系】
 * Object -> ObjectAffiliate -> Sprite -> SpriteAnim
 */

#ifndef SPRITE_H
#define SPRITE_H

#include "../core/ObjectAffiliate.h"
#include <string>

/**
 * @struct Texture
 * @brief 纹理数据结构，封装 SDL 纹理及其属性
 * 
 * 【成员说明】
 * - texture: SDL_Texture 指针，实际的图形资源
 * - src_rect: 源矩形，定义从纹理中截取的部位
 * - angle: 旋转角度（度）
 * - is_flip: 是否水平翻转
 * 
 * 【构造方式】
 * 可以通过文件路径自动加载纹理并计算尺寸
 */
struct Texture
{
    SDL_Texture* texture = nullptr;   ///< SDL 纹理指针
    SDL_FRect src_rect = {0,0,0,0};   ///< 源矩形：{x, y, width, height}
    float angle = 0.0f;               ///< 旋转角度：逆时针方向（度）
    bool is_flip = false;             ///< 水平翻转标志
    
    Texture() = default;              ///< 默认构造函数
    
    /**
     * @brief 从文件加载纹理
     * @param file_path 纹理文件路径
     * 
     * 【功能说明】
     * 1. 通过 AssetStore 加载纹理
     * 2. 自动获取纹理尺寸并设置 src_rect
     */
    Texture(const std::string& file_path);
};

/**
 * @class Sprite
 * @brief 2D 精灵类，用于渲染静态图像
 * 
 * 【主要功能】
 * 1. 加载和显示 2D 图像
 * 2. 支持缩放、旋转、翻转
 * 3. 相对于父对象定位（offset_）
 * 4. 自动跟随父对象移动
 * 
 * 【渲染原理】
 * 实际渲染位置 = parent_->getRenderPosition() + offset_
 * 渲染尺寸 = size_（可通过 scale 调整）
 * 
 * 【关键属性】
 * - texture_: 纹理数据（包含SDL_Texture、源矩形等）
 * - size_: 渲染尺寸（继承自 ObjectAffiliate）
 * - offset_: 相对于父对象的偏移（继承自 ObjectAffiliate）
 */
class Sprite : public ObjectAffiliate
{
protected:
    Texture texture_;  ///< 纹理数据：包含SDL 纹理和渲染属性

public:
    /**
     * @brief 创建精灵并添加为子对象
     * @param parent 父对象指针（ObjectScreen 类型）
     * @param file_path 纹理文件路径
     * @param scale 缩放比例（默认 1.0）
     * @return Sprite* 新创建的精灵指针
     * 
     * 【创建流程】
     * 1. 创建 Sprite 实例
     * 2. 调用 init() 初始化
     * 3. 加载纹理（通过 Texture 构造函数）
     * 4. 应用缩放
     * 5. 添加到父对象的子对象列表
     * 6. 返回精灵指针
     * 
     * 【使用示例】
     * Sprite* sprite = Sprite::addSpriteChild(parent, "assets/image.png", 2.0f);
     */
    static Sprite* addSpriteChild(ObjectScreen* parent, const std::string& file_path, float scale = 1.0f);
    
    Sprite() = default;  ///< 默认构造函数
    
    /**
     * @brief 渲染精灵
     * 
     * 【渲染流程】
     * 1. 检查纹理是否有效
     * 2. 检查父对象是否存在
     * 3. 计算实际位置 = parent_->getRenderPosition() + offset_
     * 4. 调用 Game::renderTexture() 绘制纹理
     * 
     * 【渲染参数】
     * - 位置：父对象位置 + 偏移量
     * - 尺寸：size_
     * - 旋转：texture_.angle
     * - 翻转：texture_.is_flip
     */
    virtual void render() override;

    // ==================== 获取器和设置器 ====================

    /**
     * @brief 设置缩放比例
     * @param scale 缩放因子
     * 
     * 【计算公式】
     * size_ = original_size × scale
     * 
     * 【注意】
     * 必须在 setTexture() 之后调用才有效
     * 因为需要先有原始尺寸
     */
    virtual void setScale(float scale) {size_ *= scale;}

    /**
     * @brief 获取纹理数据
     * @return Texture 纹理结构体副本
     */
    Texture getTexture() const {return texture_;}

    /**
     * @brief 设置纹理
     * @param texture 新的纹理对象
     * 
     * 【自动操作】
     * 设置纹理后会自动更新 size_ 为纹理原始尺寸
     */
    virtual void setTexture(const Texture &texture);

    /**
     * @brief 设置水平翻转
     * @param flip true=翻转，false=不翻转
     * 
     * 【效果】
     * 渲染时纹理会沿垂直轴翻转
     * 常用于角色左右转向
     */
    void setFlip(bool flip) {texture_.is_flip = flip;}

    /**
     * @brief 获取翻转状态
     * @return bool 当前翻转标志
     */
    bool getFlip() const {return texture_.is_flip;}

    /**
     * @brief 设置旋转角度
     * @param angle 旋转角度（度，逆时针为正）
     */
    void setAngle(float angle) {texture_.angle = angle;}

    /**
     * @brief 获取旋转角度
     * @return float 当前旋转角度
     */
    float getAngle() const {return texture_.angle;}
};

#endif // SPRITE_H
