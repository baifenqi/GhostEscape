/**
 * @file Sprite.cpp
 * @brief 精灵类的实现文件
 * 
 * 【主要内容】
 * 实现 Texture 结构的构造函数和 Sprite 类的核心功能
 */

#include "Sprite.h"

/**
 * @brief Texture 结构体的构造函数（从文件加载）
 * 
 * @param file_path 纹理文件路径
 * 
 * 【构造流程】
 * 1. 通过 AssetStore 获取纹理资源
 *    - 如果未加载，会自动调用 loadTexture()
 * 2. 使用 SDL_GetTextureSize() 获取纹理尺寸
 * 3. 设置 src_rect 的宽度和高度
 *    - src_rect.x 和 src_rect.y 默认为 0
 *    - src_rect.w 和 src_rect.h 为纹理实际尺寸
 * 
 * 【依赖关系】
 * 需要 Game::getInstance().getAssetStore() 提供资源管理
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
 * @param parent 父对象指针（ObjectScreen 类型）
 * @param file_path 纹理文件路径
 * @param scale 缩放比例（默认 1.0）
 * @return Sprite* 新创建的精灵指针
 * 
 * 【创建流程】
 * 1. new Sprite() - 创建精灵实例
 * 2. sprite->init() - 初始化（虽然当前为空实现）
 * 3. sprite->setTexture(Texture(file_path)) - 加载并设置纹理
 *    - 调用 Texture 构造函数加载图片
 *    - 调用 setTexture() 设置尺寸
 * 4. sprite->setScale(scale) - 应用缩放
 * 5. parent->addChild(sprite) - 添加到父对象
 * 6. return sprite - 返回指针
 * 
 * 【使用示例】
 * Sprite* playerSprite = Sprite::addSpriteChild(player, "assets/player.png", 2.0f);
 */
Sprite *Sprite::addSpriteChild(ObjectScreen *parent, const std::string &file_path, float scale)
{
    // 创建新的精灵实例
    auto sprite = new Sprite();
    
    // 初始化（当前为空实现）
    sprite->init();
    
    // 加载纹理并设置（包含自动计算尺寸）
    sprite->setTexture(Texture(file_path));
    
    // 应用缩放比例
    sprite->setScale(scale);
    
    // 添加到父对象的子对象列表
    parent->addChild(sprite);
    
    // 返回创建的精灵指针
    return sprite;
}

/**
 * @brief 渲染精灵到屏幕
 * 
 * 【渲染条件检查】
 * 1. texture_.texture != nullptr - 纹理必须有效
 * 2. parent_ != nullptr - 必须有父对象
 * 任一条件不满足则直接返回，不进行渲染
 * 
 * 【位置计算】
 * pos = parent_->getRenderPosition() + offset_
 * - parent_->getRenderPosition(): 父对象的屏幕位置
 * - offset_: 相对于父对象的偏移量
 * 
 * 【渲染调用】
 * game_.renderTexture(texture_, pos, size_)
 * - texture_: 包含SDL 纹理、源矩形、旋转角度、翻转标志
 * - pos: 渲染位置（屏幕坐标）
 * - size_: 渲染尺寸（已考虑缩放）
 * 
 * 【最终效果】
 * 精灵跟随父对象移动，并保持相对偏移
 */
void Sprite::render()
{
    // 检查纹理是否有效
    if(texture_.texture == nullptr){
        return;  // 纹理无效，跳过渲染
    }
    
    // 检查是否有父对象
    if(parent_ == nullptr){
        return;  // 没有父对象，无法确定位置
    }
    
    // 计算实际渲染位置 = 父对象位置 + 偏移量
    auto pos = parent_->getRenderPosition() + offset_;
    
    // 调用游戏引擎的渲染函数
    game_.renderTexture(texture_, pos, size_);
}

/**
 * @brief 设置纹理并自动计算尺寸
 * 
 * @param texture 新的纹理对象
 * 
 * 【设置内容】
 * 1. 复制 texture 参数到 texture_ 成员
 * 2. 提取纹理尺寸：
 *    - width = texture_.src_rect.w
 *    - height = texture_.src_rect.h
 * 3. 设置 size_ = glm::vec2(width, height)
 * 
 * 【用途】
 * 确保渲染尺寸与纹理原始尺寸一致
 * 后续可通过 setScale() 调整显示大小
 */
void Sprite::setTexture(const Texture &texture)
{
    // 复制纹理数据
    texture_ = texture;
    
    // 根据纹理源矩形设置渲染尺寸
    size_ = glm::vec2(texture_.src_rect.w, texture_.src_rect.h);
}
