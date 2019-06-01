#include "ObstacleSprite.h"

AgkImage* CIRCLE_IMAGE = NULL;
void loadCircleImage() {
	CIRCLE_IMAGE = new AgkImage("circle.png");
}

ObstacleSprite::ObstacleSprite(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, AgkHAlign halign, AgkVAlign valign)
	: Sprite(img, pos, size, halign, valign)
{
	init();
}
ObstacleSprite::ObstacleSprite(const glm::vec3& color, const glm::vec2& pos, const glm::vec2& size, AgkHAlign halign, AgkVAlign valign) 
	: Sprite(color, pos, size, halign, valign)
{
	init();
}

void ObstacleSprite::init(){
	agk::SetSpritePhysicsOn(this->id, 1);
	agk::SetSpritePhysicsRestitution(this->id, 0.5f);
	agk::SetSpriteCategoryBit(this->id, OBSTACLE_SPRITE_CATEGORY, 1);
	setDepth(15);
}



//BLOCK
ObstacleBlock::ObstacleBlock(const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color) 
	: ObstacleSprite(color, pos, size, AgkHAlign::left, AgkVAlign::top)
{
	agk::SetSpriteGroup(id, OBSTACLE_BLOCK_GROUP);
}

//CIRCLE
ObstacleCircle::ObstacleCircle(const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color) 
	: ObstacleSprite(CIRCLE_IMAGE, pos, size)
{
	setShape(AgkSpriteShape::circle);
	setColor(color);
	agk::SetSpriteGroup(id, OBSTACLE_CIRCLE_GROUP);
}