#include "ObstacleBlock.h"


ObstacleBlock::ObstacleBlock(const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color) :	Sprite(color, pos, size, AgkHAlign::left, AgkVAlign::top)
{
	agk::SetSpriteOffset(this->id, 0, 0);
	agk::SetSpritePhysicsOn(this->id, 1);
	agk::SetSpritePhysicsRestitution(this->id, 0.5f);
	agk::SetSpriteCategoryBit(this->id, OBSTACLE_SPRITE_CATEGORY, 1);
	setDepth(15);
}