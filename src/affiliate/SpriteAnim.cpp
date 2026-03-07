/**
 * @file SpriteAnim.cpp
 * @brief 动画精灵类的实现文件
 * 
 * 【主要内容】
 * 实现动画精灵的创建、播放和纹理设置功能
 */

#include "SpriteAnim.h"

/**
 * @brief 创建动画精灵并添加为子对象（静态工厂方法）
 * 
 * @param parent 父对象指针
 * @param file_path 雪碧图文件路径
 * @param scale 缩放比例（默认 1.0）
 * @return SpriteAnim* 新创建的动画精灵指针
 * 
 * 【创建流程】
 * 1. new SpriteAnim() - 创建实例
 * 2. spriteAnim->init() - 初始化（空实现）
 * 3. spriteAnim->setTexture(Texture(file_path)) - 加载雪碧图
 *    - 调用 Texture 构造函数加载图片
 *    - 调用 setTexture() 自动计算总帧数
 * 4. spriteAnim->setScale(scale) - 应用缩放
 * 5. spriteAnim->setParent(parent) - 设置父对象关系
 * 6. parent->addChild(spriteAnim) - 添加到父对象
 * 7. return spriteAnim - 返回指针
 * 
 * 【与 Sprite 的区别】
 * - 额外调用 setParent() 显式设置父对象
 * - setTexture() 会计算总帧数
 * 
 * 【使用示例】
 * SpriteAnim* moveAnim = SpriteAnim::addSpriteAnimChild(player, "assets/ghost-move.png", 2.0f);
 */
SpriteAnim *SpriteAnim::addSpriteAnimChild(ObjectScreen *parent, const std::string &file_path, float scale)
{
    // 创建动画精灵实例
    auto spriteAnim = new SpriteAnim();
    
    // 初始化
    spriteAnim->init();
    
    // 加载雪碧图并设置（自动计算帧数）
    spriteAnim->setTexture(Texture(file_path));
    
    // 应用缩放比例
    spriteAnim->setScale(scale);
    
    // 设置父对象关系
    spriteAnim->setParent(parent);
    
    // 添加到父对象的子对象列表
    parent->addChild(spriteAnim);
    
    // 返回创建的动画精灵指针
    return spriteAnim;
}

/**
 * @brief 更新动画逻辑（自动播放）
 * 
 * @param dt 时间增量（秒）
 * 
 * 【播放原理】
 * 1. 累加帧计时器：frame_timer_ += dt
 * 2. 检查是否达到帧间隔阈值：1.0f / fps_
 *    - 例如：fps_=10，阈值为 0.1 秒
 * 3. 如果达到阈值：
 *    - current_frame_++ - 切换到下一帧
 *    - 检查是否超出范围：current_frame_ >= total_frames_
 *    - 如果超出：current_frame_ = 0 - 循环到第一帧
 *    - 重置计时器：frame_timer_ = 0.0f
 * 4. 更新源矩形 X 偏移：
 *    texture_.src_rect.x = current_frame_ * texture_.src_rect.w
 *    - 每帧宽度 = src_rect.w
 *    - 第 N 帧的 X 偏移 = N × 帧宽度
 * 
 * 【视觉效果】
 * 通过不断修改 src_rect.x，从雪碧图中截取不同的帧
 * 在屏幕上连续显示，形成动画效果
 * 
 * 【性能优化】
 * 只在达到帧间隔时才更新，避免每帧都计算
 */
void SpriteAnim::update(float dt)
{
    // 累加帧计时器
    frame_timer_ += dt;
    
    // 检查是否达到帧切换阈值
    if(frame_timer_ >= 1.0f / fps_){
        // 切换到下一帧
        current_frame_ ++;
        
        // 检查是否需要循环
        if(current_frame_ >= total_frames_){
            current_frame_ = 0;  // 回到第一帧，循环播放
        }
        
        // 重置帧计时器
        frame_timer_ = 0.0f;
    }
    
    // 更新源矩形的 X 偏移，截取当前帧
    // 假设雪碧图是水平排列的：[帧 0][帧 1][帧 2]...
    texture_.src_rect.x = current_frame_ * texture_.src_rect.w;
}

/**
 * @brief 设置纹理并自动计算动画参数（重载版本）
 * 
 * @param texture 雪碧图纹理对象
 * 
 * 【设置流程】
 * 1. texture_ = texture - 复制纹理数据
 * 2. 计算总帧数：
 *    total_frames_ = texture_.src_rect.w / texture_.src_rect.h
 *    - 假设雪碧图宽度包含所有帧
 *    - 假设每帧是正方形（宽=高）
 *    - 例如：600×100 的雪碧图，total_frames_ = 6
 * 3. 修正单帧宽度：
 *    texture_.src_rect.w = texture_.src_rect.h
 *    - 确保 src_rect 是正方形
 *    - 这样每帧的宽高相等
 * 4. 设置渲染尺寸：
 *    size_ = glm::vec2(texture_.src_rect.w, texture_.src_rect.h)
 *    - 设置为单帧的尺寸
 * 
 * 【假设条件】
 * - 雪碧图是水平排列的帧序列
 * - 每帧都是正方形
 * - 帧与帧之间没有间隙
 * 
 * 【示例】
 * 一张 800×100 的雪碧图会被识别为 8 帧，每帧 100×100 像素
 */
void SpriteAnim::setTexture(const Texture &texture)
{
    // 复制纹理数据
    texture_ = texture;
    
    // 计算总帧数 = 图片总宽度 / 单帧高度（假设正方形）
    total_frames_ = texture_.src_rect.w / texture_.src_rect.h;
    
    // 修正单帧宽度为高度（强制正方形帧）
    texture_.src_rect.w = texture_.src_rect.h;
    
    // 设置渲染尺寸为单帧大小
    size_ = glm::vec2(texture_.src_rect.w, texture_.src_rect.h);        
}
