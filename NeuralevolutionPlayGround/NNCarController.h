#pragma once
#include "Car.h"
#include "NeuralNetwork.h"
//#include "NNCarInfoFrame.h"
#include "QPopulationTableWidget.h"


class NNCarController
{
public:
	NNCarController(const glm::vec2& startPos, const glm::vec2& startDir, QPopulationTableRow* _tableItem);
	~NNCarController();
	void update(bool showRays);
	void reset();
	void clear();
	std::vector<float> getNormalizedCarInput();

	bool carIsDead() { return car->isDead(); }

	void setId(int id) {
		car->setCarId(id);
		tableItem->setId(id);
	}

	Car* getCar() { return car; }
	//NNCarInfoFrameElement* getGUIFrame() { return guiFrame; }
	QPopulationTableRow* getTableItem() { return tableItem; }
	NN::NNdata* getNNdata() { return &nndata; }

	void crossover1(NNCarController* parent1, NNCarController* parent2, float coef);
	void crossover2(NNCarController* parent1, NNCarController* parent2, float coef);
	void mutate(float threshold);
	float calculateFitness(float maxLife, float maxDist, float maxCheckPointDistAvg, int maxCollisions, int maxVisitedCells);
	float getFitness() const { return fitness; }

private:
	Car* car;
	NN::NNdata nndata;
	//NNCarInfoFrameElement* guiFrame = NULL;
	QPopulationTableRow* tableItem = NULL;
	
	int id = 0;

	float fitness = 0.0f;
};

struct NNCarCompare
{
	inline bool operator() (const NNCarController& c1, const NNCarController& c2)
	{
		return (c1.getFitness() > c2.getFitness());
	}
};