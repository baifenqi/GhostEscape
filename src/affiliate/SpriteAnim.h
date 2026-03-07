/**
 * @file SpriteAnim.h
 * @brief 动画精灵类 - 支持帧动画的精灵
 * 
 * 【核心职责】
 * 1. 管理动画帧序列（从雪碧图中截取）
 * 2. 控制帧率（FPS）
 * 3. 自动播放动画（在 update 中切换帧）
 * 4. 循环播放（到达最后一帧后回到第一帧）
 * 
 * 【动画原理】
 * - 使用水平排列的雪碧图（spritesheet）
 * - 每帧是正方形，边长 = src_rect.h
 * - 总帧数 = 图片宽度 / 高度
 * - 每帧显示时间 = 1.0f / fps_
 * 
 * 【继承体系】
 * Object -> ObjectAffiliate -> Sprite -> SpriteAnim
 */

#ifndef SPRITE_ANIM_H
#define SPRITE_ANIM_H

#include "Sprite.h"

/**
 * @class SpriteAnim
 * @brief 支持帧动画的精灵类
 * 
 * 【主要功能】
 * 1. 加载雪碧图（水平排列的帧序列）
 * 2. 自动计算总帧数
 * 3. 按指定 FPS 播放动画
 * 4. 循环播放
 * 5. 可访问当前帧索引和计时器
 * 
 * 【动画参数】
 * - current_frame_: 当前显示的帧索引（0 到 total_frames_-1）
 * - total_frames_: 总帧数（自动计算）
 * - fps_: 每秒帧数（控制播放速度）
 * - frame_timer_: 帧计时器（累加 dt，超过阈值时切换帧）
 * 
 * 【典型用途】
 * 角色行走动画、特效动画等需要连续播放的场景
 */
class SpriteAnim : public Sprite {
private:
    int current_frame_ = 0;      ///< 当前帧索引：从 0 开始
    int total_frames_ = 0;       ///< 总帧数：雪碧图中的帧数量
    int fps_ = 10;               ///< 播放帧率：每秒播放的帧数
    float frame_timer_ = 0.0f;   ///< 帧计时器：累计时间，用于控制帧切换

public:
    /**
     * @brief 创建动画精灵并添加为子对象
     * @param parent 父对象指针
     * @param file_path 雪碧图文件路径
     * @param scale 缩放比例（默认 1.0）
     * @return SpriteAnim* 新创建的动画精灵指针
     * 
     * 【创建流程】
     * 1. 创建 SpriteAnim 实例
     * 2. 初始化
     * 3. 设置纹理（自动计算总帧数）
     * 4. 应用缩放
     * 5. 设置父对象关系
     * 6. 添加到父对象的子对象列表
     * 
     * 【使用示例】
     * SpriteAnim* idle = SpriteAnim::addSpriteAnimChild(player, "assets/ghost-idle.png", 2.0f);
     */
    static SpriteAnim* addSpriteAnimChild(ObjectScreen* parent, const std::string& file_path, float scale = 1.0f);

    /**
     * @brief 更新动画逻辑（自动播放）
     * @param dt 时间增量（秒）
     * 
     * 【更新流程】
     * 1. frame_timer_ += dt - 累加时间
     * 2. 检查是否达到帧间隔：frame_timer_ >= 1.0f / fps_
     * 3. 如果达到：
     *    - current_frame_++ - 切换到下一帧
     *    - 如果超出范围：current_frame_ = 0 - 循环播放
     *    - frame_timer_ = 0.0f - 重置计时器
     * 4. 更新 src_rect.x = current_frame_ * src_rect.w
     *    - 通过修改源矩形的 X 偏移来截取不同的帧
     * 
     * 【播放控制】
     * 帧率 fps_ 决定播放速度
     * 例如：fps_=10，则每 0.1 秒切换一帧
     */
    virtual void update(float dt) override;

    // ==================== 获取器和设置器 ====================

    /**
     * @brief 设置纹理（重载版本，自动计算帧数）
     * @param texture 纹理对象
     * 
     * 【特殊处理】
     * 1. 调用父类 setTexture(texture) 设置基础数据
     * 2. 计算总帧数：total_frames_ = width / height
     *    （假设雪碧图是水平排列的正方形帧）
     * 3. 修正 src_rect.w = src_rect.h
     *    （确保每帧都是正方形）
     * 4. 设置 size_ 为单帧尺寸
     */
    virtual void setTexture(const Texture &texture) override;

    /**
     * @brief 获取当前帧索引
     * @return int 当前帧索引（0 到 total_frames_-1）
     */
    int getCurrentFrame() const { return current_frame_; }

    /**
     * @brief 设置当前帧索引
     * @param current_frame 新的帧索引
     * 
     * 【用途】
     * - 手动跳转到特定帧
     * - 动画同步时使用
     */
    void setCurrentFrame(int current_frame) {current_frame_ = current_frame;}

    /**
     * @brief 获取总帧数
     * @return int 雪碧图中的总帧数
     */
    int getTotalFrames() const { return total_frames_; }

    /**
     * @brief 设置总帧数
     * @param total_frames 新的总帧数
     * 
     * 【注意】
     * 通常不需要手动设置，setTexture() 会自动计算
     */
    void setTotalFrames(int total_frames) { total_frames_ = total_frames; }

    /**
     * @brief 获取播放帧率
     * @return int 每秒播放的帧数
     */
    int getFPS() const { return fps_; }

    /**
     * @brief 设置播放帧率
     * @param fps 新的帧率值
     * 
     * 【效果】
     * fps 越高，动画播放越快
     * 例如：fps=10 时每 0.1 秒一帧，fps=20 时每 0.05 秒一帧
     */
    void setFPS(int fps) {fps_ = fps;}

    /**
     * @brief 获取帧计时器
     * @return float 当前累计的时间（秒）
     */
    float getFrameTimer() const { return frame_timer_; }

    /**
     * @brief 设置帧计时器
     * @param frame_timer 新的计时器值
     * 
     * 【用途】
     * 用于动画同步，例如切换状态时保持播放进度
     */
    void setFrameTimer(float frame_timer) { frame_timer_ = frame_timer; }
};

#endif
