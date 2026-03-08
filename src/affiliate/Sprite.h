/**
 * @file Sprite.h
 * @brief 2D 精灵类 - 用于渲染静态图像
 * 
 * 【核心功能】
 * 1. 管理纹理资源和渲染属性
 * 2. 支持缩放、旋转、翻转
 * 3. 相对于父对象定位（通过 offset_）
 */

#ifndef SPRITE_H
#define SPRITE_H

#include "../core/ObjectAffiliate.h"
#include <string>

/**
 * @struct Texture
 * @brief 纹理数据结构，封装 SDL 纹理及其渲染属性
 * 
 * 【成员说明】
 * - texture: SDL_Texture 指针，实际的图形资源
 * - src_rect: 源矩形，定义从纹理中截取的部位
 * - angle: 旋转角度（度，逆时针为正）
 * - is_flip: 是否水平翻转
 */
struct Texture
{
    SDL_Texture* texture = nullptr;   ///< SDL 纹理指针
    SDL_FRect src_rect = {0,0,0,0};   ///< 源矩形：{x, y, width, height}
    float angle = 0.0f;               ///< 旋转角度：逆时针方向（度）
    bool is_flip = false;             ///< 水平翻转标志
    
    Texture() = default;              ///< 默认构造函数
    
    /**
     * @brief 从文件路径加载纹理
     * @param file_path 纹理文件路径
     * 
     * 【功能】自动通过 AssetStore 加载纹理并获取尺寸
     */
    Texture(const std::string& file_path);
};

/**
 * @class Sprite
 * @brief 2D 精灵类，继承自 ObjectAffiliate
 * 
 * 【主要用途】
 * 显示 2D 图像，作为子对象附加到父对象上
 * 支持位置偏移、缩放、旋转和翻转效果
 * 
 * 【关键属性】
 * - texture_: 纹理数据（包含 SDL_Texture 和渲染属性）
 * - size_: 渲染尺寸（继承自 ObjectAffiliate）
 * - offset_: 相对于父对象的偏移（继承自 ObjectAffiliate）
 */
class Sprite : public ObjectAffiliate
{
protected:
    Texture texture_;  ///< 纹理数据

public:
    /**
     * @brief 创建精灵并添加为父对象的子对象
     * @param parent 父对象指针
     * @param file_path 纹理文件路径
     * @param scale 缩放比例（默认 1.0）
     * @param anchor 锚点位置（默认 CENTER）
     * @return Sprite* 新创建的精灵指针
     * 
     * 【使用示例】
     * Sprite* sprite = Sprite::addSpriteChild(parent, "assets/image.png", 2.0f);
     */
    static Sprite* addSpriteChild(ObjectScreen* parent, const std::string& file_path, float scale = 1.0f, Anchor anchor = Anchor::CENTER);

    Sprite() = default;  ///< 默认构造函数
    
    /**
     * @brief 渲染精灵到屏幕
     * 
     * 【渲染流程】
     * 1. 检查纹理和父对象是否有效
     * 2. 计算实际位置 = parent_->getRenderPosition() + offset_
     * 3. 调用 Game::renderTexture() 绘制纹理
     */
    virtual void render() override;

    // ==================== 获取器和设置器 ====================

    /**
     * @brief 设置缩放比例
     * @param scale 缩放因子
     * 
     * 【注意】必须在 setTexture() 之后调用才有效
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
     * 【效果】自动更新 size_ 为纹理原始尺寸
     */
    virtual void setTexture(const Texture &texture);

    /**
     * @brief 设置水平翻转
     * @param flip true=翻转，false=不翻转
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
