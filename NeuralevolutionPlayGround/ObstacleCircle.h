#pragma once
#include "UI/Sprite.h"
#include "ObstacleBlock.h"

extern AgkImage* CIRCLE_IMAGE;
void loadCircleImage();

class ObstacleCircle : public Sprite
{
public:
	ObstacleCircle(const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color);
};

