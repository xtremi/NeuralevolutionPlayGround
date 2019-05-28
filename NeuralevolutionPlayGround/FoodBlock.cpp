#include "FoodBlock.h"



FoodBlock::FoodBlock(const glm::vec2& pos) : Sprite(glm::vec3(0,1,0), pos, glm::vec2(15))
{
	agk::SetSpriteCategoryBits(this->id, FOOD_BLOCK_SPRITE_CATEGORY, 1);
	agk::SetSpriteGroup(this->id, FOOD_BLOCK_SPRITE_CATEGORY);
	setDepth(15);
}
