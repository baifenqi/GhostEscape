#include "SceneMain.h"
#include "Player.h"
#include <algorithm>
#include <chrono>
#include <cstring>
#include <cmath>

void SceneMain::init()
{
    world_size_ = game_.getScreenSize() * 3.0f;
    camera_position_ = world_size_ / 2.0f - game_.getScreenSize() / 2.0f;
    player_ = new Player();
    player_->init(); 
    player_->setPosition(world_size_ / 2.0f);   
}

void SceneMain::handleEvents(SDL_Event& event)
{
    switch(event.type){
        case SDL_EVENT_KEY_DOWN:
            if (event.key.key < 256) {
                keys_[event.key.key] = true;
            }
            break;
        case SDL_EVENT_KEY_UP:
            if (event.key.key < 256) {
                keys_[event.key.key] = false;
            }
            break;
    }
}

void SceneMain::update(float dt)
{
    player_->update(dt);     
}

void SceneMain::render()
{
   renderBackground();
   player_->render(); 
}


void SceneMain::renderBackground()
{
    auto start = (0.0f,0.0f)- camera_position_;
    auto end = world_size_ - camera_position_;

    
    //主网格
    game_.drawGrid(start,end,GRID_SIZE,GRID_COLOR);

    //边界矩形
    game_.drawBoundary(start,end,BOUNDARY_WIDTH,BOUNDARY_COLOR);

}


void SceneMain::clean()
{
    player_->clean();
    delete player_;
}