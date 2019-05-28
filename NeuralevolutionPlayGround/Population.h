#pragma once
#include "NNCarController.h"
#include "QPopulationTableWidget.h"
//#include "NNCarInfoFrame.h"
#include <algorithm>
#include <vector>


struct population_input {
	int size = 0;
	glm::vec2 startpos;
	glm::vec2 startdir;
};

class Population {
public:
	Population() {}
	void init(const population_input& popInput, QPopulationTableWidget* _statsTable);
	void resetCurrentGen();
	void reset();
	void setNextGen();
	void setNextGenAuto();
	void update();

	void setAllLive();
	void getTopNNdata(std::vector<NN::NNdata*>& topnns, int topn = 5);

	int hitTest(const glm::vec2& pos);
	glm::vec2 getCarPosition(int id);

	void toogleCarSelection(int id);
	void clearCarSelection();

	int getNdeadMembers() { return deadMembers.size(); }
	int getNaliveMembers() { return liveMembers.size(); }
	int getGenerationNumber() { return generationNumber; }

	std::vector<Car*> getTopCars();

private:
	void breed(std::vector<int>* parents, std::vector<int>* childs);
	void breedSorted(int topN);

	std::vector<NNCarController> members;
	std::vector<int> liveMembers;
	std::vector<int> deadMembers;
	
	int nmembers = 0;
	/*int nLiveMembers = 0;
	int nDeadMembers = 0;*/
	//NNCarInfoFrame* carInfoFrame = NULL;
	QPopulationTableWidget* statsTable = NULL;

	int generationNumber = 0;

};
