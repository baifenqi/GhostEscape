#include "Scene.h"

void Scene::handleEvents(SDL_Event &event)
{
    Object::handleEvents(event);
    for (auto child : children__screen_)
    {
        if(child->getActive())
            child->handleEvents(event);

        }
    for (auto child : children__world_)
    {
        if(child->getActive()){
            child->handleEvents(event);
        }


    }
}

void Scene::update(float dt)
{
    Object::update(dt);
    for(auto it = children__screen_.begin(); it != children__screen_.end();){
        auto child = *it;
        if(child->getNeedMove()){
            it = children__screen_.erase(it);
            child->clean();
            delete child;
        }else{
            if(child->getActive()){
                child->update(dt);
            }
            ++it;
        }
    }
    for(auto it = children__world_.begin(); it != children__world_.end();){
        auto child = *it;
        if(child->getNeedMove()){
            it = children__world_.erase(it);
            child->clean();
            delete child;
        }else{
            if(child->getActive()){
                child->update(dt);
            }
            ++it;
        }
    }

}

void Scene::render()
{
    Object::render();
    for (auto child : children__screen_)
    {
        if(child->getActive())
        {
            child->render();
        }

    }
    for (auto child : children__world_)
    {
        if(child->getActive())
        {
            child->render();
        }
    }

}

void Scene::clean()
{
    Object::clean();
    for (auto child : children__screen_)
    {
        child->clean();
    }
    children__screen_.clear();
    for (auto child : children__world_)
    {
        child->clean();
    }
    children__world_.clear();
}

void Scene::addChild(Object *child)
{
    switch (child->getType())
    {
    case ObjectType::OBJECT_SCREEN:
        children__screen_.push_back(dynamic_cast<ObjectScreen*>(child));
        break;
    case ObjectType::OBJECT_WORLD:
        children__world_.push_back(dynamic_cast<ObjectWorld*>(child));
        break;

    default:
        children_.push_back(child);
        break;
    }
}

void Scene::removeChild(Object *child)
{
    switch (child->getType())
    {
    case ObjectType::OBJECT_SCREEN:
        children__screen_.erase(remove(children__screen_.begin(),children__screen_.end(),dynamic_cast<ObjectScreen*>(child)),children__screen_.end());
        break;
    case ObjectType::OBJECT_WORLD:
        children__world_.erase(remove(children__world_.begin(),children__world_.end(),dynamic_cast<ObjectWorld*>(child)),children__world_.end());
        break;

    default:
        children_.erase(remove(children_.begin(),children_.end(),child),children_.end());
        break;
    }
}

void Scene::setCameraPosition(const glm::vec2 &camera_position)
{
    camera_position_ = camera_position;
    camera_position_ = glm::clamp(camera_position_,glm::vec2(-300),world_size_ - game_.getScreenSize()+glm::vec2(300));
}