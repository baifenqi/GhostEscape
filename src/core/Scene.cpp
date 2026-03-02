#include "Scene.h"

void Scene::setCameraPosition(const glm::vec2 &camera_position)
{
    camera_position_ = camera_position;
    camera_position_ = glm::clamp(camera_position_,glm::vec2(-300),world_size_ - game_.getScreenSize()+glm::vec2(300));
}