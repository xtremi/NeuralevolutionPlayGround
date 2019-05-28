#include "NNCarController.h"



NNCarController::NNCarController(const glm::vec2& startPos, const glm::vec2& startDir, QPopulationTableRow* _tableItem)
{	
	tableItem = _tableItem;
	car = new Car(startPos, startDir);
	nndata.init(7, 10, 2, 0.7);
}
NNCarController::~NNCarController() {
	/*delete car;
	delete tableItem;*/
}
void NNCarController::clear() {
	delete car;
	tableItem = NULL;
}
void NNCarController::reset() {
	car->reset();
	tableItem->reset();
}

void NNCarController::update() {
	car->update();
	tableItem->setLife(car->getLifePercent());
	tableItem->setDistance(car->getDistanceTravelled());
	tableItem->setVisitedCells(car->getNvisitedCells());
	nndata.computeOutput(getNormalizedCarInput());

	if (nndata.Y.at(0, 0) > 0.667) {
		car->gas();
		tableItem->wflash();
	}
	else if (nndata.Y.at(0, 0) < 0.333) {
		car->back();
		tableItem->sflash();
	}

	if (nndata.Y.at(0, 1) > 0.667) {
		car->turnLeft();
		tableItem->aflash();
	}
	else if (nndata.Y.at(0, 1) < 0.333) {
		car->turnRight();
		tableItem->dflash();
	}


	/*if (nndata.Y.at(0, 0) > 0.5){
		car->gas();
		guiFrame->wflash();
	}
	 if (nndata.Y.at(0, 1) > 0.5){
		car->back();
		guiFrame->sflash();
	}
	if (nndata.Y.at(0, 2) > 0.5){
		car->turnLeft();
		guiFrame->aflash();
	}
	 if (nndata.Y.at(0, 3) > 0.5){
		car->turnRight();
		guiFrame->dflash();
	}*/
	

}

float NNCarController::calculateFitness(float maxLife, float maxDist, float maxCheckPointDistAvg, int maxCollisions, int maxVisitedCells) {
	static const float w1 = 0.00f;		//life
	static const float w2 = 0.000f;		//total dist
	static const float w3 = 0.225f;		//avg checkpoint dist
	static const float w4 = 0.225f;		//collisions
	static const float w5 = 0.550f;		//visited cells


	float fv1, fv2, fv3, fv4, fv5;
	fv1 = car->getLifePercent();
	fv2 = car->getDistanceTravelled();
	fv3 = car->getAverageCheckPointDistances();
	fv4 = (float)car->getNumberOfCollisions();
	fv5 = (float)car->getNvisitedCells();

	float f1, f2, f3, f4, f5;	
	f1 = maxLife < 0.001f ? 0.0f : w1 * fv1 / maxLife;
	f2 = maxDist < 0.001f ? 0.0f : w2 * fv2 / maxDist;//high
	f3 = maxCheckPointDistAvg < 0.001f ? 0.0f : w3 * fv3 / maxCheckPointDistAvg; //high
	f4 = maxCollisions <= 0 ? w4 : w4 * (1.0f - fv4 / (float)maxCollisions); //low
	f5 = maxVisitedCells <= 0 ? 0.0f : w5 * fv5 / (float)maxVisitedCells;

	fitness = f1 + f2 + f3 + f4 + f5;
	return fitness;
}

std::vector<float> NNCarController::getNormalizedCarInput() {
	std::vector<float> normInput;
	std::vector<float>* rayDist = car->getSensorRayDistances();
	/*float life = car->getLifePercent();

	normInput.push_back(life - 0.5f);*/

	for (int i = 0; i < rayDist->size(); i++){
		float norm = (*rayDist)[i] / MAX_RAY_CAST_LENGTH;
		norm -= 0.5f;
		normInput.push_back(norm);
	}

	return normInput;
}

void NNCarController::crossover1(NNCarController* parent1, NNCarController* parent2, float coef) {
	nndata.crossover1(&parent1->nndata, &parent2->nndata, coef);
}
void NNCarController::crossover2(NNCarController* parent1, NNCarController* parent2, float coef) {
	nndata.crossover2(&parent1->nndata, &parent2->nndata, coef);
}

void NNCarController::mutate(float threshold) {
	nndata.mutate(threshold);
}