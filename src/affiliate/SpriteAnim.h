/**
 * @file SpriteAnim.h
 * @brief 动画精灵类 - 支持帧动画的精灵
 * 
 * 【核心功能】
 * 1. 管理动画帧序列（从雪碧图中截取）
 * 2. 控制播放帧率（FPS）
 * 3. 自动循环播放动画
 */

#ifndef SPRITE_ANIM_H
#define SPRITE_ANIM_H

#include "Sprite.h"

/**
 * @class SpriteAnim
 * @brief 支持帧动画的精灵类，继承自 Sprite
 * 
 * 【工作原理】
 * 使用水平排列的雪碧图，每帧是正方形
 * 通过修改 src_rect.x 截取不同的帧
 * 
 * 【关键属性】
 * - current_frame_: 当前显示的帧索引
 * - total_frames_: 总帧数（自动计算）
 * - fps_: 每秒播放的帧数
 * - frame_timer_: 帧切换计时器
 * - isLoop_: 是否循环播放
 * - isFinish_: 是否播放完毕
 */
class SpriteAnim : public Sprite {
private:
    int current_frame_ = 0;      ///< 当前帧索引（从 0 开始）
    int total_frames_ = 0;       ///< 总帧数（雪碧图中的帧数量）
    int fps_ = 10;               ///< 播放帧率（每秒帧数）
    float frame_timer_ = 0.0f;   ///< 帧计时器（累计时间用于切换帧）
    bool isLoop_ = true;         ///< 是否循环播放：true=循环，false=播放一次后停止
    bool isFinish_ = false;      ///< 是否播放完毕：仅在非循环模式下有效

public:
    /**
     * @brief 创建动画精灵并添加为子对象
     * @param parent 父对象指针
     * @param file_path 雪碧图文件路径
     * @param scale 缩放比例（默认 1.0）
     * @param anchor 锚点位置（默认 CENTER）
     * @return SpriteAnim* 新创建的动画精灵指针
     * 
     * 【使用示例】
     * SpriteAnim* anim = SpriteAnim::addSpriteAnimChild(parent, "assets/anim.png", 2.0f);
     */
    static SpriteAnim* addSpriteAnimChild(ObjectScreen* parent, const std::string& file_path, float scale = 1.0f, Anchor anchor = Anchor::CENTER);

    /**
     * @brief 更新动画逻辑（自动播放）
     * @param dt 时间增量（秒）
     * 
     * 【执行流程】
     * 1. 累加帧计时器
     * 2. 达到帧间隔时切换到下一帧
     * 3. 超出范围时循环到第一帧
     * 4. 更新 src_rect.x 截取当前帧
     */
    virtual void update(float dt) override;

    /**
     * @brief 设置纹理（重载版本，自动计算帧数）
     * @param texture 纹理对象
     * 
     * 【特殊处理】自动计算总帧数 = 宽度 / 高度
     */
    virtual void setTexture(const Texture &texture) override;

    // ==================== 动画控制 ====================

    /**
     * @brief 设置是否循环播放
     * @param loop true=循环播放，false=播放一次后停止
     */
    void setLoop(bool loop) { isLoop_ = loop; }

    /**
     * @brief 获取循环播放设置
     * @return bool true=循环播放，false=播放一次后停止
     */
    bool getLoop() const { return isLoop_; }

    /**
     * @brief 检查动画是否播放完毕
     * @return bool true=已播放完毕，false=正在播放或循环中
     */
    bool getFinish() const { return isFinish_; }

    /**
     * @brief 重置动画状态
     * 
     * 【效果】重置到第 0 帧，清除完成标记，重新开始播放
     */
    void reset() {
        current_frame_ = 0;
        frame_timer_ = 0.0f;
        isFinish_ = false;
    }

    // ==================== 获取器和设置器 ====================

    int getCurrentFrame() const { return current_frame_; }           ///< 获取当前帧索引
    void setCurrentFrame(int current_frame) {current_frame_ = current_frame;}  ///< 设置当前帧
    int getTotalFrames() const { return total_frames_; }             ///< 获取总帧数
    void setTotalFrames(int total_frames) { total_frames_ = total_frames; }    ///< 设置总帧数
    int getFPS() const { return fps_; }                              ///< 获取播放帧率
    void setFPS(int fps) {fps_ = fps;}                               ///< 设置播放帧率
    float getFrameTimer() const { return frame_timer_; }             ///< 获取帧计时器
    void setFrameTimer(float frame_timer) { frame_timer_ = frame_timer; }      ///< 设置帧计时器
};

#endif
