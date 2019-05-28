#include "ObstacleCircle.h"

AgkImage* CIRCLE_IMAGE = NULL;

void loadCircleImage() {

	CIRCLE_IMAGE = new AgkImage("circle.png");
}
ObstacleCircle::ObstacleCircle(const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color) : Sprite(CIRCLE_IMAGE, pos, size)
{
	setShape(AgkSpriteShape::circle);
	setColor(color);
	agk::SetSpritePhysicsOn(this->id, 1);
	agk::SetSpritePhysicsRestitution(this->id, 0.5f);
	agk::SetSpriteCategoryBit(this->id, OBSTACLE_SPRITE_CATEGORY, 1);
	setDepth(15);
}