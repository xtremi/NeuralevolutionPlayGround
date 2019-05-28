#include "Car.h"

AgkImage* CAR_IMG = NULL;



void loadCarImage() {

	CAR_IMG = new AgkImage("car-1.png");
}

Car::Car(const glm::vec2& _pos, const glm::vec2& _dir)
	: Sprite(CAR_IMG, _pos, glm::vec2(DEFAULT_CAR_SIZE, -1.0f), AgkHAlign::center, AgkVAlign::center)
{
	startPos = _pos;
	startDir = _dir;
	
	//this->setShape(AgkSpriteShape::box);
	this->setShape(AgkSpriteShape::polygon);
	agk::SetSpritePhysicsOn(this->id, 2);	
	agk::SetSpritePhysicsDamping(this->id, 5.00f);
	agk::SetSpritePhysicsAngularDamping(this->id, 15.00f);
	agk::SetSpritePhysicsRestitution(this->id, 0.5f);
	agk::SetSpriteCategoryBit(this->id, CAR_SPRITE_CATEGORY, 1);
	agk::SetSpriteCollideBit(this->id, CAR_SPRITE_CATEGORY, 0);

	setDepth(10);
	initLifeSprite();
	initTextSprite();
	initSelectionSprite();
	initWorldGrid();
	reset();

	sensorRayAngles.push_back(0.0f);
	sensorRayAngles.push_back(30.0f);
	sensorRayAngles.push_back(-30.0f);
	sensorRayAngles.push_back(60.0f);
	sensorRayAngles.push_back(-60.0f);	
	sensorRayAngles.push_back(90.0f);
	sensorRayAngles.push_back(-90.0f);
	sensorRayDistances.resize(7);
}
Car::~Car() {
	deleteSprite();
	delete selectionSprite;
	delete idSprite;
	delete lifeBackground;
	delete lifeLeft;
}

void Car::deleteSprite() {
	Sprite::deleteSprite();
	selectionSprite->deleteSprite();
	idSprite->deleteSprite();
	lifeBackground->deleteSprite();
	lifeLeft->deleteSprite();
}

void Car::reset() {
	lastPos = startPos;
	lastCheckPointPos = startPos;
	setPosition(startPos);
	setRotation(0.0f);
	life = max_life;
	dir = startDir;
	setColor(glm::vec3(1.0f));
	dead = false;
	frame_counter1 = 0;
	frame_counter2 = 0;
	ncollisions = 0;
	ncheckpoints = 0;
	distance_travelled = 0.0f;
	checkpoint_distances = 0.0f;
	n_visited_cells = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			worldgrid[i][j] = false;
		}
	}
}

void Car::toogleSelected() {
	if (!_isSelected) {
		_isSelected = true;
		selectionSprite->show();
		selectionSprite->setPosition(getPos());
	}
	else {
		_isSelected = false;
		selectionSprite->hide();
	}
}

void Car::updateSelectionSpritePosition() {
	selectionSprite->setPosition(getPos());
}

void Car::initTextSprite() {
	idSprite = new TextSprite("0", glm::vec3(0,0,1), glm::vec2(0), 25.0f);
	updateTextSpritePosition();
}
void Car::setCarId(int id) {
	std::string idstr = std::to_string(id);
	idSprite->setText(&idstr[0]);		
}
void Car::updateTextSpritePosition() {
	static const glm::vec2 offset(TEXT_SPRITE_OFFSET_X, TEXT_SPRITE_OFFSET_Y);
	idSprite->setPosition(getPos() + offset);
	idSprite->realign();
}

void Car::initLifeSprite() {
	glm::vec2 size(LIFE_SPRITE_SIZE_X, LIFE_SPRITE_SIZE_Y);
	lifeBackground = new Sprite(glm::vec3(1, 0, 0), glm::vec2(0), size, AgkHAlign::left, AgkVAlign::top);
	lifeLeft = new Sprite(glm::vec3(0, 1, 0), glm::vec2(0), size, AgkHAlign::left, AgkVAlign::top);
	lifeBackground->setDepth(10);
	lifeLeft->setDepth(10);
	updateLifeSpritePosition();
}
void Car::initSelectionSprite() {
	selectionSprite = new Sprite(glm::vec3(1, 0, 0), glm::vec2(0), glm::vec2(50.0f));
	selectionSprite->setColorAlpha(0.3f);
	selectionSprite->hide();
}
void Car::updateLifeSpritePosition() {
	static const glm::vec2 offset(LIFE_SPRITE_OFFSET_X, LIFE_SPRITE_OFFSET_Y);
	lifeBackground->setPosition(getPos() + offset);
	lifeLeft->setPosition(getPos() + offset);
}

void Car::updateLifeSpriteSize() {
	float width = (life / max_life) * LIFE_SPRITE_SIZE_X;
	lifeLeft->setSize(width, lifeLeft->getHeight());	
}

void Car::updateSensorRays() {
	glm::vec2 rstartPos = getPos();
	

	for (int i = 0; i < n_ray_directions; i++) {

		glm::vec2 raydir = glm::rotate(dir, sensorRayAngles[i]);
		glm::vec2 raypnt = rstartPos + raydir * MAX_RAY_CAST_LENGTH;

		if (agk::PhysicsRayCastCategory(OBSTACLE_SPRITE_CATEGORY, rstartPos.x, rstartPos.y, raypnt.x, raypnt.y)) {
			raypnt.x = agk::GetRayCastX();
			raypnt.y = agk::GetRayCastY();
			sensorRayDistances[i] = glm::distance(raypnt, rstartPos);
			agk::DrawLine(rstartPos.x, rstartPos.y, raypnt.x, raypnt.y, 125, 25, 25);
		}
		else{
			sensorRayDistances[i] = MAX_RAY_CAST_LENGTH;
			agk::DrawLine(rstartPos.x, rstartPos.y, raypnt.x, raypnt.y, 25, 125, 25);
		}
		
	}
}


void Car::updatePhysics() {	
	vel.x = agk::GetSpritePhysicsVelocityX(this->id);
	vel.y = agk::GetSpritePhysicsVelocityY(this->id);

	float rot = this->getRotation();
	dir.x = agk::Cos(rot);
	dir.y = agk::Sin(rot);

}

void Car::checkCollision() {
	bool collided = agk::GetSpriteFirstContact(this->id);
	if (collided)
		ncollisions++;
	if (collided && timeSinceLastCollision > 1.0f && glm::length(vel) > 8.0f && life > 0.0f) {
		life -= 25.f;
		updateLifeSpriteSize();
		timeSinceLastCollision = 0.0f;
	}
	timeSinceLastCollision += 0.1f;

	if (agk::GetSpriteHitGroup(FOOD_BLOCK_SPRITE_CATEGORY, getX(), getY())) {
		int f = 0;
	}

}

void Car::gas() {
	if (glm::length(vel) < max_speed) {
		agk::SetSpritePhysicsForce(this->id, getX(), getY(), dir.x * power, dir.y * power);
	}
}
void Car::back() {	

	if (glm::length(vel) < max_rev_speed) {
		agk::SetSpritePhysicsForce(this->id, getX(), getY(), dir.x * -power * 0.25f, dir.y * -power * 0.25f);
	}
}
void Car::turnLeft() {
	if (abs(agk::GetSpritePhysicsAngularVelocity(this->id)) < max_turn_vel){
		agk::SetSpritePhysicsTorque(this->id, -torq_power);
	}
}
void Car::turnRight() {
	if (agk::GetSpritePhysicsAngularVelocity(this->id) < max_turn_vel){
		agk::SetSpritePhysicsTorque(this->id, torq_power);
	}
}

void Car::checkLife() {
	if (life <= 0.0f){
		dead = true;
		setColor(glm::vec3(0.5f, 0.5f, 0.5f));
	}
}

void Car::initWorldGrid() {

	static const float H = 800.0f;
	static const float W = 1000.0f;
	static const int ncells = 10;

	cell_w = W / (float)ncells;
	cell_h = H / (float)ncells;

	worldgrid = std::vector<std::vector<bool>>(ncells, std::vector<bool>(ncells));
}