#pragma once
#include "FileIO.h"
#include "glm/glm.hpp"
#include "convinient_functions.h"
#include "ObstacleSprite.h"
#include "FoodBlock.h"

class MapIO : public FileIO {
public:
	MapIO() : FileIO() {}

	bool read(const std::string& filepath, int wdim[2],
		std::vector<ObstacleSprite*>* obstacleSprites, std::vector<FoodBlock*>* foodSprites);

	bool write(const std::string& filepath, int wdim[2],
		std::vector<ObstacleSprite*>* obstacleSprites, std::vector<FoodBlock*>* foodSprites);
};

