/**
 * @file Player.cpp
 * @brief 玩家类的实现文件
 * 
 * 【主要内容】
 * 实现玩家的移动控制、动画切换和摄像机跟随功能
 */

#include "Player.h"
#include "core/Scene.h"
#include "Affiliate/SpriteAnim.h"

/**
 * @brief 初始化玩家对象
 * 
 * 【初始化流程】
 * 1. 调用 Actor::init() 设置对象类型为 OBJECT_WORLD
 * 2. 设置最大速度为 600 像素/秒（较快的移动速度）
 * 3. 创建 idle 动画精灵：
 *    - 图片路径："assets/sprite/ghost-idle.png"
 *    - 缩放比例：2.0 倍
 *    - 作为子对象添加到玩家
 * 4. 创建 move 动画精灵：
 *    - 图片路径："assets/sprite/ghost-move.png"
 *    - 缩放比例：2.0 倍
 *    - 作为子对象添加到玩家
 * 5. 初始停用 move 动画（只显示 idle 状态）
 * 
 * 【资源加载】
 * SpriteAnim::addSpriteAnimChild() 会自动：
 * - 加载纹理
 * - 计算总帧数（图片宽度 / 高度）
 * - 设置父对象关系
 */
void Player::init()
{
    // 调用父类初始化函数
    Actor::init();
    
    // 设置最大移动速度：600 像素/秒
    max_speed_ = 600.0f;
    
    // 创建静止状态的精灵动画（2 倍大小）
    sprite_idle_ = SpriteAnim::addSpriteAnimChild(this, "assets/sprite/ghost-idle.png", 2.0f);
    
    // 创建移动状态的精灵动画（2 倍大小）
    sprite_move_ = SpriteAnim::addSpriteAnimChild(this, "assets/sprite/ghost-move.png", 2.0f);
    
    // 初始隐藏移动动画，只显示静止动画
    sprite_move_->setActive(false);
}

/**
 * @brief 处理 SDL 事件
 * 
 * @param event SDL 事件引用
 * 
 * 【当前实现】
 * 调用 Actor::handleEvents(event) 处理子对象事件
 * 
 * 【注意】
 * 实际输入处理在 update() 中的 keyboardControl() 方法
 * 使用 SDL_GetKeyboardState() 轮询而非事件驱动
 */
void Player::handleEvents(SDL_Event &event)
{
    // 调用父类事件处理函数
    Actor::handleEvents(event);
}

/**
 * @brief 更新玩家逻辑
 * 
 * @param dt 时间增量（秒）
 * 
 * 【更新流程】
 * 1. Actor::update(dt) - 更新子对象（精灵动画）
 * 2. velocity_ *= 0.9 - 应用阻尼效果（每帧减速 10%）
 *    产生平滑的停止效果，模拟摩擦力
 * 3. checkState() - 检查移动状态并切换动画
 * 4. keyboardControl() - 读取键盘状态并施加加速度
 * 5. move(dt) - 根据速度积分更新位置
 * 6. syncamera() - 更新摄像机位置以跟随玩家
 * 
 * 【物理模拟】
 * 速度衰减 + 加速度累积 = 平滑的运动手感
 */
void Player::update(float dt)
{
    // 更新子对象（精灵动画）
    Actor::update(dt);
    
    // 应用速度阻尼（每帧乘以 0.9，减速 10%）
    // 产生惯性效果，使移动更自然
    velocity_ *= 0.9;
    
    // 检查移动状态并切换动画
    checkState();
    
    // 读取键盘输入并施加加速度
    keyboardControl();
    
    // 根据速度更新位置
    move(dt);
    
    // 更新摄像机位置以跟随玩家
    syncamera();
}

/**
 * @brief 渲染玩家
 * 
 * 【当前实现】
 * 调用 Actor::render() 渲染子对象
 * 
 * 【实际渲染】
 * 由活跃的精灵动画（idle 或 move）负责渲染
 * 根据 is_moving_ 状态决定显示哪个动画
 */
void Player::render()
{
    // 调用父类渲染函数
    Actor::render();
}

/**
 * @brief 清理玩家资源
 * 
 * 【清理内容】
 * 调用 Actor::clean() 清理所有子对象
 * sprite_idle_ 和 sprite_move_ 会被自动清理
 */
void Player::clean()
{
    // 调用父类清理函数
    Actor::clean();
}

/**
 * @brief 键盘控制移动
 * 
 * 【控制方案】
 * - W / ↑ : 向上加速（-Y 方向）
 * - S / ↓ : 向下加速（+Y 方向）
 * - A / ← : 向左加速（-X 方向）
 * - D / → : 向右加速（+X 方向）
 * 
 * 【物理参数】
 * - 加速度：max_speed_ × 0.8 = 480 像素/秒²
 * - 最大速度：600 像素/秒
 * 
 * 【实现细节】
 * 1. 使用 SDL_GetKeyboardState() 获取实时键盘状态
 * 2. 每个方向键按下时施加恒定加速度
 * 3. 速度分量限制在 [-max_speed_, +max_speed_]
 * 4. 最后对合速度进行限幅（不超过 max_speed_）
 * 
 * 【设计意图】
 * 加速度控制产生平滑的启动和转向效果
 * 类似物理中的力驱动运动
 */
void Player::keyboardControl()
{
    // 获取当前键盘状态数组
    auto currentKeyStates = SDL_GetKeyboardState(NULL);
    
    // 计算加速度步长：最大速度的 80% = 480 像素/秒²
    const float accel_step = max_speed_ * 0.8f;
    
    // W 键或上箭头：向上加速（-Y 方向）
    if (currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_UP]){
        velocity_.y -= accel_step;
        // 限制 Y 方向速度不超过最大值
        if(velocity_.y < -max_speed_){
            velocity_.y = -max_speed_;
        }
    }
    
    // S 键或下箭头：向下加速（+Y 方向）
    if (currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_DOWN]){
        velocity_.y += accel_step;
        if(velocity_.y > max_speed_){
            velocity_.y = max_speed_;
        }
    }
    
    // A 键或左箭头：向左加速（-X 方向）
    if (currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_LEFT]){
        velocity_.x -= accel_step;
        if(velocity_.x < -max_speed_){
            velocity_.x = -max_speed_;
        }
    }
    
    // D 键或右箭头：向右加速（+X 方向）
    if (currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_RIGHT]){
        velocity_.x += accel_step;
        if(velocity_.x > max_speed_){
            velocity_.x = max_speed_;
        }
    }

    // 计算当前速度大小（模长）
    float speed = glm::length(velocity_);
    
    // 如果合速度超过最大速度，归一化后缩放
    if (speed > max_speed_) {
        velocity_ = glm::normalize(velocity_) * max_speed_;
    }
}

/**
 * @brief 根据速度更新位置
 * 
 * @param dt 时间增量（秒）
 * 
 * 【运动方程】
 * position_new = position_old + velocity × dt
 * 
 * 【边界处理】
 * 使用 glm::clamp 将位置限制在 [0, world_size] 范围内：
 * - 最小值：glm::vec2(0) - 世界左上角
 * - 最大值：game_.getCurrentScene()->getWorldSize() - 世界右下角
 * 
 * 【效果】
 * 玩家碰到世界边界时会被阻挡，无法继续移动
 */
void Player::move(float dt)
{
    // 根据速度和dt计算位移，更新位置
    setPosition(position_ + velocity_ * dt);
    
    // 限制玩家位置在世界边界内
    position_ = glm::clamp(position_,
                           glm::vec2(0),
                           game_.getCurrentScene()->getWorldSize());
}

/**
 * @brief 同步摄像机位置
 * 
 * 【跟随算法】
 * camera_position = player_position - screen_size / 2
 * 
 * 【效果】
 * 摄像机始终将玩家保持在屏幕中央
 * 玩家移动时，摄像机会实时跟随
 * 
 * 【调用时机】
 * 每帧 update() 结束时调用，确保即时跟随
 */
void Player::syncamera()
{
    // 设置摄像机位置，使玩家在屏幕中心
    game_.getCurrentScene()->setCameraPosition(
        position_ - game_.getScreenSize() / 2.0f
    );
}

/**
 * @brief 检查并更新移动状态
 * 
 * 【检查内容】
 * 1. 方向判断：
 *    - velocity_.x < 0: 设置精灵水平翻转（向左）
 *    - velocity_.x >= 0: 取消翻转（向右）
 *    - 同时影响 idle 和 move 两种动画
 * 
 * 2. 移动状态判断：
 *    - 计算速度模长：glm::length(velocity_)
 *    - 阈值判断：> 0.1 视为移动，<= 0.1 视为静止
 *    - 使用小阈值避免数值抖动
 * 
 * 3. 状态切换：
 *    - 如果 new_is_moving != is_moving_
 *    - 更新 is_moving_ 标志
 *    - 调用 changeState() 切换动画
 * 
 * 【设计意图】
 * 自动根据速度切换动画，无需手动控制
 */
void Player::checkState()
{
    // 根据 X 方向速度设置精灵翻转
    if(velocity_.x < 0){
        // 向左移动：水平翻转精灵
        sprite_idle_->setFlip(true);
        sprite_move_->setFlip(true);
    } else {
        // 向右移动：不翻转
        sprite_idle_->setFlip(false);
        sprite_move_->setFlip(false);
    }

    // 计算速度大小，判断是否在移动（阈值 0.1 防止抖动）
    bool new_is_moving = (glm::length(velocity_) > 0.1f);
    
    // 如果移动状态发生变化
    if(new_is_moving != is_moving_){
        // 更新状态标志
        is_moving_ = new_is_moving;
        
        // 切换动画状态
        changeState(is_moving_);
    }
}

/**
 * @brief 切换动画状态
 * 
 * @param is_moving true=切换到移动动画，false=切换到静止动画
 * 
 * 【切换逻辑】
 * 
 * 移动到移动状态：
 * 1. 停用 idle 动画
 * 2. 激活 move 动画
 * 3. 同步帧索引：move 从 idle 的当前帧开始
 * 4. 同步计时器：保持动画播放进度一致
 * 
 * 移动到静止状态：
 * 1. 停用 move 动画
 * 2. 激活 idle 动画
 * 3. 同步帧索引：idle 从 move 的当前帧开始
 * 4. 同步计时器：保持动画播放进度一致
 * 
 * 【设计意图】
 * 帧同步确保动画切换时不会出现视觉跳跃
 * 例如：从跑到停，精灵姿势保持连续
 */
void Player::changeState(bool is_moving)
{
    if(is_moving){
        // 切换到移动动画
        
        // 停用静止动画
        sprite_idle_->setActive(false);
        
        // 激活移动动画
        sprite_move_->setActive(true);
        
        // 同步帧索引（从 idle 的当前帧继续）
        sprite_move_->setCurrentFrame(sprite_idle_->getCurrentFrame());
        
        // 同步帧计时器（保持播放节奏）
        sprite_move_->setFrameTimer(sprite_idle_->getFrameTimer());
    } else {
        // 切换到静止动画
        
        // 停用移动动画
        sprite_move_->setActive(false);
        
        // 激活静止动画
        sprite_idle_->setActive(true);
        
        // 同步帧索引（从 move 的当前帧继续）
        sprite_idle_->setCurrentFrame(sprite_move_->getCurrentFrame());
        
        // 同步帧计时器
        sprite_idle_->setFrameTimer(sprite_move_->getFrameTimer());
    }
}
