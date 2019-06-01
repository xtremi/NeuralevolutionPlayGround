#pragma once
#include "UI/Sprite.h"

#define OBSTACLE_SPRITE_CATEGORY 2
#define OBSTACLE_BLOCK_GROUP 10
#define OBSTACLE_CIRCLE_GROUP 20

extern AgkImage* CIRCLE_IMAGE;
void loadCircleImage();


class ObstacleSprite : public Sprite 
{
public:
	ObstacleSprite(AgkImage* img, const glm::vec2& pos = glm::vec2(0.0f), const glm::vec2& size = glm::vec2(1.0f), AgkHAlign halign = AgkHAlign::center, AgkVAlign valign = AgkVAlign::center);
	ObstacleSprite(const glm::vec3& color, const glm::vec2& pos = glm::vec2(0.0f), const glm::vec2& size = glm::vec2(1.0f), AgkHAlign halign = AgkHAlign::center, AgkVAlign valign = AgkVAlign::center);

	enum class obstacle{block, circle};
	virtual obstacle type() = 0;

private:
	void init();
};

class ObstacleBlock : public ObstacleSprite
{
public:
	ObstacleBlock(const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color);

	ObstacleSprite::obstacle type() { return ObstacleSprite::obstacle::block; }
};



class ObstacleCircle : public ObstacleSprite
{
public:
	ObstacleCircle(const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color);

	ObstacleSprite::obstacle type() { return ObstacleSprite::obstacle::circle; }
};

