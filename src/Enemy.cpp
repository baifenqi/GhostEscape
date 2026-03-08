/**
 * @file Enemy.cpp
 * @brief 敌人类的实现文件
 * 
 * 【主要内容】
 * 实现敌人的 AI 追踪、状态切换和碰撞检测功能
 */

#include "Enemy.h"
#include "affiliate/SpriteAnim.h"
#include "core/Scene.h"
#include "affiliate/Collider.h"

/**
 * @brief 初始化敌人对象
 * 
 * 【初始化流程】
 * 
 * 1. **调用父类初始化**
 *    ```cpp
 *    Actor::init();
 *    ```
 *    - 设置类型为 OBJECT_WORLD
 *    - 初始化基础属性
 * 
 * 2. **创建三个状态的动画精灵**（2 倍缩放）
 *    ```cpp
 *    animNormal_ = SpriteAnim::addSpriteAnimChild(this, "assets/sprite/ghost-Sheet.png", 2.0f);
 *    animHurt_ = SpriteAnim::addSpriteAnimChild(this, "assets/sprite/ghostHurt-Sheet.png", 2.0f);
 *    animDead_ = SpriteAnim::addSpriteAnimChild(this, "assets/sprite/ghostDead-Sheet.png", 2.0f);
 *    ```
 *    - 路径：assets/sprite/目录下的雪碧图
 *    - 缩放：2.0 倍大小
 *    - 自动计算帧数和尺寸
 * 
 * 3. **设置初始显示状态**
 *    ```cpp
 *    animHurt_->setActive(false);   // 隐藏受伤动画
 *    animDead_->setActive(false);   // 隐藏死亡动画
 *    animDead_->setLoop(false);     // 死亡动画只播放一次
 *    currentAnim_ = animNormal_;    // 当前显示普通动画
 *    ```
 * 
 * 4. **创建碰撞体**
 *    ```cpp
 *    glm::vec2 scaled_size = currentAnim_->getSize();
 *    Anchor anchor = currentAnim_->getAnchor();
 *    collider_ = Collider::addColliderChild(this, scaled_size, Collider::Type::CIRCLE, anchor);
 *    ```
 *    - 使用动画的缩放后尺寸
 *    - 圆形碰撞体
 *    - 中心锚点对齐
 * 
 * 5. **输出调试信息**
 *    ```cpp
 *    SDL_Log("=== ENEMY DEBUG INFO ===");
 *    SDL_Log("animNormal size: %.2f x %.2f", ...);
 *    SDL_Log("collider size: %.2f x %.2f", ...);
 *    SDL_Log("========================");
 *    ```
 * 
 * 【资源需求】
 * 必须存在以下文件：
 * - assets/sprite/ghost-Sheet.png
 * - assets/sprite/ghostHurt-Sheet.png
 * - assets/sprite/ghostDead-Sheet.png
 * 
 * 【注意事项】
 * - 必须调用 init() 才能正常初始化
 * - 碰撞体尺寸基于动画尺寸，确保视觉与物理一致
 */
void Enemy::init()
{
    // 调用父类初始化
    Actor::init();
    
    // 创建普通状态动画精灵（2 倍大小）
    animNormal_ = SpriteAnim::addSpriteAnimChild(this, "assets/sprite/ghost-Sheet.png", 2.0f);
    
    // 创建受伤状态动画精灵（2 倍大小）
    animHurt_ = SpriteAnim::addSpriteAnimChild(this, "assets/sprite/ghostHurt-Sheet.png", 2.0f);
    
    // 创建死亡动画精灵（2 倍大小）
    animDead_ = SpriteAnim::addSpriteAnimChild(this, "assets/sprite/ghostDead-Sheet.png", 2.0f);
    
    // 初始只显示普通动画，隐藏其他
    animHurt_->setActive(false);
    animDead_->setActive(false);
    
    // 设置死亡动画为单次循环（播放完一次后停止）
    animDead_->setLoop(false);

    // 设置当前动画为普通状态
    currentAnim_ = animNormal_;
    
    // 获取动画的缩放后尺寸和锚点
    glm::vec2 scaled_size = currentAnim_->getSize();
    Anchor anchor = currentAnim_->getAnchor();
    
    // 输出调试日志
    SDL_Log("=== ENEMY DEBUG INFO ===");
    SDL_Log("animNormal size: %.2f x %.2f", scaled_size.x, scaled_size.y);
    SDL_Log("animNormal anchor: %d", static_cast<int>(anchor));
    SDL_Log("animNormal texture src_rect: %d x %d", 
            currentAnim_->getTexture().src_rect.w, 
            currentAnim_->getTexture().src_rect.h);
    
    // 使用缩放后的尺寸创建圆形碰撞体
    collider_ = Collider::addColliderChild(this, scaled_size, Collider::Type::CIRCLE, anchor);
    
    // 输出碰撞体调试信息
    SDL_Log("collider size: %.2f x %.2f", collider_->getSize().x, collider_->getSize().y);
    SDL_Log("collider anchor: %d", static_cast<int>(collider_->getAnchor()));
    SDL_Log("========================");
}

/**
 * @brief 更新敌人逻辑
 * 
 * @param dt 时间增量（秒）
 * 
 * 【更新流程】
 * 
 * 1. **调用父类更新**
 *    ```cpp
 *    Actor::update(dt);
 *    ```
 *    - 更新子对象（动画精灵、碰撞体）
 * 
 * 2. **AI 追踪**
 *    ```cpp
 *    aimTarget(target_);
 *    ```
 *    - 如果 target_为空，跳过
 *    - 计算朝向玩家的方向向量
 *    - 设置速度 = 方向 × max_speed_
 * 
 * 3. **应用移动**
 *    ```cpp
 *    move(dt);
 *    ```
 *    - 根据速度更新位置
 *    - 自动限制在世界边界内
 * 
 * 4. **攻击检测**
 *    ```cpp
 *    attack();
 *    ```
 *    - 检测与玩家的碰撞
 *    - 如果碰撞，输出日志
 * 
 * 【每帧行为】
 * 敌人会持续追踪玩家当前位置
 * 接触时触发攻击检测
 * 
 * 【性能考虑】
 * - aimTarget 只进行简单向量计算
 * - attack 的碰撞检测是 O(1) 复杂度
 */
void Enemy::update(float dt)
{
    // 调用父类更新函数（更新子对象）
    Actor::update(dt);
    
    // AI 追踪：计算朝向玩家的方向并设置速度
    aimTarget(target_);
    
    // 应用移动（带边界限制）
    move(dt);

    // 执行攻击检测
    attack();
}

/**
 * @brief 渲染敌人
 * 
 * 【渲染流程】
 * 1. 调用 Actor::render()
 * 2. 遍历子对象并渲染
 * 3. 当前激活的动画精灵会被绘制
 * 4. 碰撞体在 DEBUG_MODE 下可见
 * 
 * 【渲染内容】
 * - currentAnim_: 当前状态的动画
 * - collider_: 碰撞体（仅调试模式）
 * 
 * 【图层顺序】
 * 由 Scene::render() 控制：
 * 背景 → 世界对象 → 屏幕对象
 */
void Enemy::render()
{
    // 调用父类渲染函数
    Actor::render();
}

/**
 * @brief 瞄准追踪目标
 * 
 * @param target 目标玩家指针
 * 
 * 【追踪算法详解】
 * 
 * 1. **空指针检查**
 *    ```cpp
 *    if(target == nullptr){
 *        return;
 *    }
 *    ```
 *    - 目标为空时不做任何操作
 * 
 * 2. **计算方向向量**
 *    ```cpp
 *    auto direction = target->getPosition() - this->getPosition();
 *    ```
 *    - direction.x = player.x - enemy.x
 *    - direction.y = player.y - enemy.y
 * 
 * 3. **归一化方向向量**
 *    ```cpp
 *    direction = glm::normalize(direction);
 *    ```
 *    - 使方向向量长度为 1
 *    - 只保留方向信息
 * 
 * 4. **设置速度向量**
 *    ```cpp
 *    velocity_ = direction * max_speed_;
 *    ```
 *    - velocity_.x = direction.x × max_speed_
 *    - velocity_.y = direction.y × max_speed_
 * 
 * 【数学原理】
 * ```
 * 设：enemy_pos = (ex, ey), player_pos = (px, py)
 * 则：direction = (px-ex, py-ey)
 *      normalized = direction / |direction|
 *      velocity = normalized × max_speed_
 * ```
 * 
 * 【效果】
 * 敌人会以恒定速度（max_speed_）朝玩家当前位置直线移动
 * 
 * 【改进建议】
 * - 可添加预测算法（预判玩家移动轨迹）
 * - 可添加随机扰动（更自然的追踪）
 */
void Enemy::aimTarget(Player *target)
{
    // 空指针检查
    if(target == nullptr){
        return;
    }
    
    // 计算从敌人指向玩家的方向向量
    auto direction = target->getPosition() - this->getPosition();
    
    // 归一化方向向量（长度为 1）
    direction = glm::normalize(direction);
    
    // 设置速度 = 方向 × 最大速度
    velocity_ = direction * max_speed_;
}

/**
 * @brief 检查并更新状态
 * 
 * 【当前实现】
 * 空函数，待扩展
 * 
 * 【预期功能】
 * 
 * 1. **受伤检测**
 *    ```cpp
 *    if(player 攻击命中 && currentState_ != State::DEAD){
 *        changeState(State::HURT);
 *    }
 *    ```
 * 
 * 2. **死亡检测**
 *    ```cpp
 *    if(health <= 0){
 *        changeState(State::DEAD);
 *    }
 *    ```
 * 
 * 3. **状态恢复**
 *    ```cpp
 *    if(hurtTimer >= hurtDuration){
 *        changeState(State::NORMAL);
 *    }
 *    ```
 * 
 * 【调用时机】
 * 应在 update() 中调用
 * 
 * 【扩展建议】
 * 添加生命值、无敌时间等属性
 */
void Enemy::checkState()
{
    // TODO: 实现状态检测逻辑
}

/**
 * @brief 切换状态
 * 
 * @param new_state 新的状态值
 * 
 * 【状态切换流程】
 * 
 * 1. **提前返回**
 *    ```cpp
 *    if(new_state == currentState_) return;
 *    ```
 *    - 避免重复切换同一状态
 * 
 * 2. **停用当前动画**
 *    ```cpp
 *    currentAnim_->setActive(false);
 *    ```
 *    - 不再渲染当前动画
 * 
 * 3. **选择新动画**
 *    ```cpp
 *    switch (new_state) {
 *    case State::NORMAL:
 *        currentAnim_ = animNormal_;
 *        currentAnim_->setActive(true);
 *        break;
 *    case State::HURT:
 *        currentAnim_ = animHurt_;
 *        currentAnim_->setActive(true);
 *        break;
 *    case State::DEAD:
 *        currentAnim_ = animDead_;
 *        currentAnim_->setActive(true);
 *        break;
 *    }
 *    ```
 * 
 * 4. **更新状态标志**
 *    ```cpp
 *    currentState_ = new_state;
 *    ```
 * 
 * 【动画特性】
 * - NORMAL/HURT: 循环播放
 * - DEAD: 单次播放（setLoop(false)）
 * 
 * 【使用示例】
 * ```cpp
 * // 玩家攻击命中时
 * enemy->changeState(State::HURT);
 * 
 * // 敌人死亡时
 * enemy->changeState(State::DEAD);
 * ```
 */
void Enemy::changeState(State new_state)
{
    // 如果状态相同，直接返回
    if(new_state == currentState_) return;
    
    // 停用当前动画
    currentAnim_->setActive(false);
    
    // 根据新状态选择并激活对应动画
    switch (new_state) {
    case State::NORMAL:
        // 切换到普通状态动画
        currentAnim_ = animNormal_;
        currentAnim_->setActive(true);
        break;
        
    case State::HURT:
        // 切换到受伤状态动画
        currentAnim_ = animHurt_;
        currentAnim_->setActive(true);
        break;
        
    case State::DEAD:
        // 切换到死亡动画
        currentAnim_ = animDead_;
        currentAnim_->setActive(true);
        break;
    }
    
    // 更新状态标志
    currentState_ = new_state;
}

/**
 * @brief 执行攻击检测
 * 
 * 【检测流程】
 * 
 * 1. **空指针检查**
 *    ```cpp
 *    if(!collider_ || !target_->getCollider()) return;
 *    ```
 *    - 检查自身碰撞体是否存在
 *    - 检查目标碰撞体是否存在
 * 
 * 2. **碰撞检测**
 *    ```cpp
 *    if(collider_->isColliding(target_->getCollider())){
 *        SDL_Log("Circle vs Circle");
 *    }
 *    ```
 *    - 调用圆形碰撞检测
 *    - 如果重叠，输出调试日志
 * 
 * 【检测原理】
 * 圆形 vs 圆形：
 * - 计算圆心距离
 * - 判断：distance < radius1 + radius2
 * 
 * 【当前实现】
 * 仅输出日志，未实现实际伤害
 * 
 * 【扩展建议】
 * ```cpp
 * void Enemy::attack() {
 *     if(!collider_ || !target_->getCollider()) return;
 *     
 *     if(collider_->isColliding(target_->getCollider())){
 *         // 对玩家造成伤害
 *         target_->takeDamage(damage_);
 *         
 *         // 播放攻击特效
 *         spawnAttackEffect();
 *         
 *         // 重置攻击冷却
 *         attackCooldown_ = attackInterval;
 *     }
 * }
 * ```
 */
void Enemy::attack()
{
    // 检查碰撞体是否存在
    if(!collider_ || !target_->getCollider()) return;
    
    // 检测是否发生碰撞
    if(collider_->isColliding(target_->getCollider())){
        // 输出调试日志
        SDL_Log("Circle vs Circle");
        
        // TODO: 实现实际攻击逻辑
        // - 对玩家造成伤害
        // - 播放音效
        // - 添加击退效果
    }
}

/**
 * @brief 移除死亡的敌人
 * 
 * 【移除条件】
 * ```cpp
 * if(animDead_->getFinish()){
 *     needMove_ = true;
 * }
 * ```
 * 
 * 【执行时机】
 * 当死亡动画播放完毕时：
 * - animDead_->getFinish() == true
 * - 设置 needMove_标记
 * 
 * 【预期用途】
 * SceneMain::update()应检查此标志：
 * ```cpp
 * void SceneMain::update(float dt) {
 *     Scene::update(dt);
 *     
 *     // 检查敌人是否需要移除
 *     if(enemy_ && enemy_->needMove_){
 *         removeChild(enemy_);
 *         delete enemy_;
 *         enemy_ = nullptr;
 *     }
 * }
 * ```
 * 
 * 【内存管理】
 * 被场景删除后会自动调用 clean()：
 * 1. 清理所有子对象（动画、碰撞体）
 * 2. 释放内存
 * 
 * 【注意事项】
 * - needMove_成员需要在 Enemy.h 中定义
 * - 需要 SceneMain 配合检查移除
 */
void Enemy::remove()
{
    // 检查死亡动画是否播放完毕
    if(animDead_->getFinish()){
        // 标记需要移除
        needMove_ = true;
    }
}
