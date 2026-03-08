/**
 * @file Sprite.cpp
 * @brief 2D 精灵类的实现文件
 * 
 * 【主要内容】
 * 实现纹理加载、精灵创建和渲染功能
 */

#include "Sprite.h"

/**
 * @brief Texture 构造函数 - 从文件加载纹理
 * 
 * @param file_path 纹理文件路径
 * 
 * 【执行流程】
 * 1. 通过 AssetStore 获取纹理资源
 * 2. 使用 SDL_GetTextureSize() 获取纹理尺寸
 * 3. 设置 src_rect 的宽度和高度
 */
Texture::Texture(const std::string &file_path)
{
    // 通过资源管理器加载纹理
    texture = Game::getInstance().getAssetStore()->getTexture(file_path);
    
    // 获取纹理尺寸并设置源矩形
    SDL_GetTextureSize(texture, &src_rect.w, &src_rect.h);
}

/**
 * @brief 创建精灵并添加为子对象（静态工厂方法）
 * 
 * @param parent 父对象指针
 * @param file_path 纹理文件路径
 * @param scale 缩放比例
 * @param anchor 锚点位置（默认 CENTER）
 * @return Sprite* 新创建的精灵指针
 * 
 * 【创建步骤】
 * 1. 创建 Sprite 实例
 * 2. 初始化
 * 3. 加载并设置纹理
 * 4. 先设置锚点（确保后续缩放使用正确的锚点）
 * 5. 应用缩放
 * 6. 添加到父对象
 */
Sprite *Sprite::addSpriteChild(ObjectScreen *parent, const std::string &file_path, float scale, Anchor anchor)
{
    auto sprite = new Sprite();      // 创建实例
    sprite->init();                  // 初始化
    sprite->setTexture(Texture(file_path));  // 加载纹理
    sprite->setAnchor(anchor);       // 先设置锚点
    sprite->setScale(scale);         // 再应用缩放（会根据锚点计算 offset）
    parent->addChild(sprite);        // 添加到父对象
    return sprite;
}

/**
 * @brief 渲染精灵到屏幕
 * 
 * 【渲染条件】
 * - 纹理必须有效 (texture_.texture != nullptr)
 * - 必须有父对象 (parent_ != nullptr)
 * 
 * 【位置计算】
 * 实际位置 = parent_->getRenderPosition() + offset_
 */
void Sprite::render()
{
    if(texture_.texture == nullptr){
        return;  // 纹理无效，跳过渲染
    }
    
    if(parent_ == nullptr){
        return;  // 没有父对象，无法确定位置
    }
    
    auto pos = parent_->getRenderPosition() + offset_;
    game_.renderTexture(texture_, pos, size_);
}

/**
 * @brief 设置纹理并自动计算尺寸
 * 
 * @param texture 新的纹理对象
 * 
 * 【效果】更新 texture_ 并根据纹理尺寸设置 size_
 */
void Sprite::setTexture(const Texture &texture)
{
    texture_ = texture;
    size_ = glm::vec2(texture_.src_rect.w, texture_.src_rect.h);
}
