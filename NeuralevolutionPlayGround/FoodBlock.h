#pragma once
#include "UI/Sprite.h"

#define FOOD_BLOCK_SPRITE_CATEGORY 5

class FoodBlock : public Sprite
{
public:
	FoodBlock(const glm::vec2& pos);
};

