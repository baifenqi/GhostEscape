/**
 * @file ObjectAffiliate.cpp
 * @brief 附属对象类的实现文件
 * 
 * 【主要内容】
 * 实现锚点偏移计算、尺寸设置和缩放功能
 */

#include "ObjectAffiliate.h"

/**
 * @brief 根据锚点设置偏移量
 * @param anchor 锚点类型
 * 
 * 【功能说明】
 * 根据不同的锚点位置，计算相对于父对象的偏移量
 * 确保对象按照指定的锚点对齐（如中心、左上角等）
 * 
 * 【9 个锚点的偏移规则】
 * - TOP_LEFT: (0, 0)
 * - TOP_CENTER: (-width/2, 0)
 * - TOP_RIGHT: (-width, 0)
 * - CENTER_LEFT: (0, -height/2)
 * - CENTER: (-width/2, -height/2)
 * - CENTER_RIGHT: (-width, -height/2)
 * - BOTTOM_LEFT: (0, -height)
 * - BOTTOM_RIGHT: (-width, -height)
 */
void ObjectAffiliate::setOffsetByAnchor(Anchor anchor)
{
    anchor_ = anchor;
    switch (anchor)
    {
    case Anchor::TOP_LEFT:
        offset_ = glm::vec2(0.0f, 0.0f);
        break;
    case Anchor::TOP_CENTER:
        offset_ = glm::vec2(-size_.x / 2.0f, 0.0f);
        break;
    case Anchor::TOP_RIGHT:
        offset_ = glm::vec2(-size_.x, 0.0f);
        break;
    case Anchor::CENTER_LEFT:
        offset_ = glm::vec2(0.0f, -size_.y / 2.0f);
        break;
    case Anchor::CENTER:
        offset_ = glm::vec2(-size_.x / 2.0f, -size_.y / 2.0f);
        break;
    case Anchor::CENTER_RIGHT:
        offset_ = glm::vec2(-size_.x, -size_.y / 2.0f);
        break;
    case Anchor::BOTTOM_LEFT:
        offset_ = glm::vec2(0.0f, -size_.y);
        break;
    case Anchor::BOTTOM_CENTER:
        offset_ = glm::vec2(-size_.x / 2.0f, -size_.y);
        break;
    case Anchor::BOTTOM_RIGHT:
        offset_ = glm::vec2(-size_.x, -size_.y);
        break;
    default:
        break;
    }
}

/**
 * @brief 设置对象尺寸
 * @param size 新的尺寸值
 * 
 * 【功能说明】
 * 1. 更新 size_成员变量
 * 2. 根据当前锚点重新计算偏移量
 * 确保尺寸改变后对齐仍然正确
 */
void ObjectAffiliate::setSize(const glm::vec2 &size)
{
    size_ = size;
    setOffsetByAnchor(anchor_);
}

/**
 * @brief 设置缩放比例
 * @param scale 缩放因子
 * 
 * 【功能说明】
 * 1. 将当前尺寸乘以缩放因子
 * 2. 根据锚点重新计算偏移量
 * 用于调整对象的显示大小
 */
void ObjectAffiliate::setScale(float scale)
{
    size_ *= scale;
    setOffsetByAnchor(anchor_);
}
