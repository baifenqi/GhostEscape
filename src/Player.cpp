#include "Player.h"
#include "core/Scene.h"

void Player::init()
{
    max_speed_ = 500.0f;
}

void Player::handleEvents(SDL_Event &event)
{
    
}

void Player::update(float dt)
{
    velocity_ *= 0.9;
    keyboardControl();   
    move(dt);
    syncamera();
}

void Player::render()
{
    game_.drawBoundary(render_position_,render_position_+glm::vec2(20.0f),5.0f,{1.0f,0.0f,0.0f,1.0f,});//测试方框
}

void Player::clean()
{
}

void Player::keyboardControl()
{
    auto currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_UP]){
        velocity_.y = -max_speed_;
    }
    if (currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_DOWN]){
        velocity_.y = max_speed_;
    }
    if (currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_LEFT]){
        velocity_.x = -max_speed_;
    }
    if (currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_RIGHT]){
        velocity_.x = max_speed_;
    }
}

void Player::move(float dt)
{
    setPosition(position_ + velocity_ * dt);
    position_ = glm::clamp(position_,glm::vec2(0),game_.getCurrentScene()->getWorldSize());
}

void Player::syncamera()
{
    game_.getCurrentScene()->setCameraPosition(position_ - game_.getScreenSize() / 2.0f);
}
