#include "Population.h"

void Population::init(const population_input& popInput, QPopulationTableWidget* _statsTable) {
	statsTable = _statsTable;
	nmembers = popInput.size;
	for (int i = 0; i < nmembers; i++) {
		//NNCarInfoFrameElement* guiEl = carInfoFrame->addFrameElement();
		QPopulationTableRow* tableItem = statsTable->newRow();
		members.emplace_back(NNCarController(popInput.startpos, popInput.startdir, tableItem));
		members[i].setId(i);
	}
	liveMembers = std::vector<int>();
	deadMembers = std::vector<int>();
	setAllLive();
}

void Population::toogleCarSelection(int id) {
	members[id].getCar()->toogleSelected();
	members[id].getTableItem()->toogleSelected();
}

void Population::clearCarSelection() {
	for (int i = 0; i < nmembers; i++) {
		if (members[i].getCar()->isSelected()) {
			members[i].getCar()->toogleSelected();
			members[i].getTableItem()->toogleSelected();
		}
	}
}


void Population::setNextGenAuto() {
	generationNumber++;

	float maxLife = -1.0f;
	float maxDist = -1.0f;
	float maxCheckDistAvg = -1.0f;
	int maxCollisions = -1;
	int maxVisitedCells = -1;

	for (int i = 0; i < nmembers; i++) {
		float life = members[i].getCar()->getLifePercent();
		float dist = members[i].getCar()->getDistanceTravelled();
		float checkPointDistAvg = members[i].getCar()->getAverageCheckPointDistances();
		int ncollisions = members[i].getCar()->getNumberOfCollisions();
		int visitedCells = members[i].getCar()->getNvisitedCells();

		if (life > maxLife) maxLife = life;
		if (dist > maxDist) maxDist = dist;
		if (checkPointDistAvg > maxCheckDistAvg) maxCheckDistAvg = checkPointDistAvg;
		if (ncollisions > maxCollisions) maxCollisions = ncollisions;
		if (visitedCells > maxVisitedCells) maxVisitedCells = visitedCells;
	}

	for (int i = 0; i < nmembers; i++) {
		members[i].calculateFitness(maxLife, maxDist, maxCheckDistAvg, maxCollisions, maxVisitedCells);
	}
	
	static const int ntop = 5;
	//auto swap1 = members.begin() + NN::randomFromTo(0, ntop - 1);
	//auto swap2 = members.begin() + 1;// NN::randomFromTo(ntop, nmembers - ntop - 1);
	std::sort(members.begin(), members.end(), NNCarCompare());	
	/*std::iter_swap(
		swap1, 
		swap2);*/
	//std::swap(members[ntop - 1], members[NN::randomFromTo(ntop, nmembers - 1)]);
	breedSorted(ntop);	
	resetCurrentGen();
}


void Population::breedSorted(int topN) {
	for (int id = topN; id < nmembers; id++) {
		int rindex1 = NN::randomFromTo(0, topN - 1);
		int rindex2 = NN::randomFromTo(0, topN - 1);

		float coef = 0.5f*NN::random() + 0.5f;

		members[id].crossover2(&members[rindex1], &members[rindex2], coef);
		members[id].mutate(0.25f);
		members[id].mutate(0.25f*0.5f);
		members[id].mutate(0.25f*0.25f);
	}
}


void Population::setNextGen() {
	generationNumber++;
	std::vector<int> selectedCars;
	std::vector<int> unselectedCars;
	int nselections = 0;
	for (int i = 0; i < nmembers; i++) {
		if (members[i].getCar()->isSelected()) {
			selectedCars.push_back(i);
			nselections++;
		}
		else
			unselectedCars.push_back(i);
	}

	int runselected = NN::randomFromTo(0, unselectedCars.size() - 1);
	selectedCars.push_back(unselectedCars[runselected]);

	breed(&selectedCars, &unselectedCars);
	resetCurrentGen();
}



void Population::breed(std::vector<int>* parents, std::vector<int>* childs){

	for (int id : (*childs)) {
		int rindex1 = NN::randomFromTo(0, parents->size() - 1);
		int rindex2 = NN::randomFromTo(0, parents->size() - 1);
		int pid1 = (*parents)[rindex1];
		int pid2 = (*parents)[rindex2];

		float coef = 0.5f*NN::random() + 0.5f;

		members[id].crossover2(&members[pid1], &members[pid2], coef);
		members[id].mutate(0.25f);
		members[id].mutate(0.25f*0.5f);
		members[id].mutate(0.25f*0.25f);
	}

}

void Population::reset() {
	liveMembers.clear();
	deadMembers.clear();
	for (int i = 0; i < members.size(); i++) {
		members[i].clear();
	}
	members.clear();
	generationNumber = 0;
	nmembers = 0;
}

void Population::resetCurrentGen() {
	liveMembers.clear();
	deadMembers.clear();
	setAllLive();

	for (int i = 0; i < nmembers; i++) {
		members[i].reset();
	}
}

void Population::setAllLive() {	
	for (int i = 0; i < nmembers; i++){
		liveMembers.push_back(i);

	}

	//nLiveMembers = nmembers;
	deadMembers.clear();
}

void Population::update() {

	std::string allstr = "Members: " + std::to_string(members.size());
	std::string livstr = "Alive  : " + std::to_string(liveMembers.size());
	std::string dedstr = "Dead   : " + std::to_string(deadMembers.size());
	agk::Print(&allstr[0]);
	agk::Print(&livstr[0]);
	agk::Print(&dedstr[0]);
	for (auto it = liveMembers.begin(); it != liveMembers.end();) {
		int id = (*it);

		members[id].update();
		if (members[id].carIsDead()) {
			it = liveMembers.erase(it);
			deadMembers.push_back(id);
			members[id].getTableItem()->markDead();
		}
		else {
			it++;
		}
	}
}

int Population::hitTest(const glm::vec2& pos) {
	for (int i = 0; i < nmembers; i++) {
		if (members[i].getCar()->hitTest(pos)) {
			return i;
		}
	}
	for (int i = 0; i < nmembers; i++) {
		if (members[i].getTableItem()->hitTest(pos)) {
			return i;
		}
	}
	return -1;
}

glm::vec2 Population::getCarPosition(int id) {
	if (id < nmembers) {
		return members[id].getCar()->getPos();
	}
	else
		return glm::vec2(0.0f);
}


std::vector<Car*> Population::getTopCars() {
	std::vector<Car*> topcars;
	for (int i = 0; i < 5; i++)
		topcars.push_back(members[i].getCar());
	return topcars;
}


void Population::getTopNNdata(std::vector<NN::NNdata*>& topnns, int topn) {
	//assume already sorted:

	for (int i = 0; i < topn; i++) {
		topnns.push_back(members[i].getNNdata());
	}

}
