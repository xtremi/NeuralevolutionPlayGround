#pragma once
#include "UI/Sprite.h"
#include "UI/TextSprite.h"
#include "convinient_functions.h"

#include "FoodBlock.h"
#include "ObstacleBlock.h"

#include "glm/gtx/rotate_vector.hpp"

extern AgkImage* CAR_IMG;
void loadCarImage();

#define MAX_RAY_CAST_LENGTH 400.0f

#define DEFAULT_CAR_SIZE 30.0f

#define TEXT_SPRITE_OFFSET_X   0.0f
#define TEXT_SPRITE_OFFSET_Y -15.0f

#define LIFE_SPRITE_OFFSET_X -15.0f
#define LIFE_SPRITE_OFFSET_Y -20.0f

#define LIFE_SPRITE_SIZE_X 30.0f
#define LIFE_SPRITE_SIZE_Y 7.5f

#define CAR_SPRITE_CATEGORY 1



class Car : public Sprite
{
public:
	Car(const glm::vec2& _pos, const glm::vec2& _dir);
	~Car();
	void deleteSprite();
	void reset();
	void setCarId(int id);

	void update() {
		updatePhysics();
		checkCollision();
		updateLifeSpritePosition();
		updateTextSpritePosition();
		if(_isSelected)
			updateSelectionSpritePosition();
		updateSensorRays();

		checkLife();
		
		frame_counter1 += 1;
		frame_counter2 += 1;

		if (frame_counter1 == 8) {
			frame_counter1 = 0;
			glm::vec2 curPos = getPos();
			float ddtravelled = glm::distance(lastPos, curPos);
			distance_travelled += ddtravelled;
			lastPos = curPos;
			if (ddtravelled < 0.1f){
				life -= 10.0f;
				updateLifeSpriteSize();
			}

			int r = curPos.x / cell_w;
			int c = curPos.y / cell_h;
			if (!worldgrid[r][c]) {
				worldgrid[r][c] = true;
				n_visited_cells++;
			}

		}

		if (frame_counter2 == 100) {
			frame_counter2 = 0;
			glm::vec2 curPos = getPos();
			checkpoint_distances += glm::distance(lastCheckPointPos, curPos);
			lastCheckPointPos = curPos;
			ncheckpoints++;
		}
		

	}

	void gas();
	void back();
	void turnLeft();
	void turnRight();
	

	void toogleSelected();
	bool isDead() { return dead; }
	bool isSelected() { return _isSelected; }
	
	std::vector<std::vector<bool>>* getWorldGrid() { return &worldgrid; }

	int getNumberOfCollisions() { return ncollisions; }
	float getLifePercent() { return life / max_life; }
	float getDistanceTravelled() { return distance_travelled; };
	int getNvisitedCells() { return n_visited_cells; }
	float getAverageCheckPointDistances() { 
		if (ncheckpoints == 0) 
			return 0.0f;
		return checkpoint_distances / (float)ncheckpoints; 
	}

	std::vector<float>* getSensorRayDistances() { return &sensorRayDistances; }

private:	
	void checkCollision();
	
	void updatePhysics();
	
	void initWorldGrid();
	void initLifeSprite();
	void updateLifeSpritePosition();
	void updateLifeSpriteSize();
	void updateSensorRays();
	void checkLife();
	void initTextSprite();	
	void initSelectionSprite();
	void updateTextSpritePosition();
	void updateSelectionSpritePosition();


	int frame_counter1 = 0;
	int frame_counter2 = 0;
	int ncheckpoints = 0;
	int ncollisions = 0;

	float distance_travelled = 0.0f;
	float checkpoint_distances = 0.0f;
	int n_visited_cells = 0;

	glm::vec2 lastPos;
	glm::vec2 lastCheckPointPos;

	bool _isSelected = false;
	bool dead = false;
	glm::vec2 startPos;
	glm::vec2 startDir;
	glm::vec2 dir;

	glm::vec2 vel;
	float max_speed = 3000.0f;
	float max_rev_speed = 50.0f;
	float max_turn_vel = 4.0f;
	float power = 15e3f;
	
	float torq_power = 10e3;

	float max_life = 100.0f;
	float life = 100.0f;

	float cell_w = 0.0f;
	float cell_h = 0.0f;

	float timeSinceLastCollision = 0.0f;

	std::vector<std::vector<bool>> worldgrid;

private:
	Sprite* selectionSprite = NULL;
	TextSprite* idSprite = NULL;
	Sprite* lifeBackground = NULL;
	Sprite* lifeLeft = NULL;
	int n_ray_directions = 7;
	std::vector<float> sensorRayAngles;
	std::vector<float> sensorRayDistances;

	
		
};

