/**
 * @file SpriteAnim.cpp
 * @brief 动画精灵类的实现文件
 *
 * 【主要内容】
 * 实现动画播放、帧切换和雪碧图处理功能
 */

#include "SpriteAnim.h"


/**
 * @brief 创建动画精灵并添加为子对象
 *
 * @param parent 父对象指针
 * @param file_path 雪碧图文件路径
 * @param scale 缩放比例
 * @param anchor 锚点位置
 * @return SpriteAnim* 新创建的动画精灵指针
 *
 * 【创建步骤】
 * 1. 创建 SpriteAnim 实例
 * 2. 初始化
 * 3. 设置纹理（自动计算帧数）
 * 4. 先设置锚点（确保后续缩放使用正确的锚点）
 * 5. 应用缩放
 * 6. 设置父对象关系
 * 7. 添加到父对象
 */
SpriteAnim *SpriteAnim::addSpriteAnimChild(ObjectScreen *parent, const std::string &file_path, float scale, Anchor anchor)
{
    auto spriteAnim = new SpriteAnim();
    spriteAnim->init();
    spriteAnim->setTexture(Texture(file_path));
    spriteAnim->setAnchor(anchor);     // 先设置锚点
    spriteAnim->setScale(scale);       // 再应用缩放（会根据锚点计算 offset）
    spriteAnim->setParent(parent);
    parent->addChild(spriteAnim);
    return spriteAnim;
}

/**
 * @brief 更新动画逻辑（自动播放）
 *
 * @param dt 时间增量（秒）
 *
 * 【播放原理】
 * 1. 累加帧计时器：frame_timer_ += dt
 * 2. 达到阈值（1.0f / fps_）时切换帧
 * 3. 超出范围时根据循环设置处理
 * 4. 更新 src_rect.x 截取当前帧
 *
 * 【示例】fps_=10 时，每 0.1 秒切换一帧
 */
void SpriteAnim::update(float dt)
{
    if(isFinish_) return;

    frame_timer_ += dt;

    if(frame_timer_ >= 1.0f / fps_){
        current_frame_ ++;

        if(current_frame_ >= total_frames_){
            current_frame_ = 0;  // 循环到第0帧
            if(!isLoop_) isFinish_ = true;  // 非循环模式标记结束
        }

        frame_timer_ = 0.0f;
    }

    texture_.src_rect.x = current_frame_ * texture_.src_rect.w;
}

/**
 * @brief 设置纹理并自动计算动画参数
 *
 * @param texture 雪碧图纹理对象
 *
 * 【特殊处理】
 * 1. 复制纹理数据
 * 2. 计算总帧数 = 宽度 / 高度（假设正方形帧）
 * 3. 修正单帧宽度为高度
 * 4. 设置渲染尺寸为单帧大小
 *
 * 【示例】800×100 的雪碧图会被识别为 8 帧，每帧 100×100
 */
void SpriteAnim::setTexture(const Texture &texture)
{
    texture_ = texture;
    total_frames_ = texture_.src_rect.w / texture_.src_rect.h;  // 计算总帧数
    texture_.src_rect.w = texture_.src_rect.h;                  // 强制正方形帧
    size_ = glm::vec2(texture_.src_rect.w, texture_.src_rect.h);
}
