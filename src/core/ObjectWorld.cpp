#include "ObjectWorld.h"
#include "Scene.h"

/*
*双向坐标转换：当设置世界位置时，自动更新渲染位置；反之亦然
*自动同步：在update方法中自动同步世界位置和渲染位置
*场景关联：通过Game类获取当前场景进行坐标转换
*/
void ObjectWorld::update(float dt)
{
    ObjectScreen::update(dt);
    render_position_ = game_.getCurrentScene()->worldToScreenPosition(position_);
}

void ObjectWorld::setPosition(const glm::vec2 &position)
{
    position_ = position;
    render_position_ = game_.getCurrentScene()->worldToScreenPosition(position_);
}

void ObjectWorld::setRenderPosition(const glm::vec2 &render_position)
{
    render_position_ = render_position;
    position_ = game_.getCurrentScene()->screenToWorldPosition(render_position);
}
