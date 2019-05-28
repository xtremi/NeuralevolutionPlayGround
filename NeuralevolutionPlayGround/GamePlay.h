#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <map>

#include "convinient_functions.h"
#include "Population.h"
#include "ObstacleBlock.h"
#include "ObstacleCircle.h"
#include "FoodBlock.h"
//this is only in master
#include "UI/UIButtonTypes.h"

#define  KEY_W  87
#define  KEY_A  65
#define  KEY_S  83
#define  KEY_D  68
#define  KEY_X    88
#define  KEY_Z    90
#define  KEY_C    67
#define  KEY_V    86

#define	 KEY_LEFT   37
#define  KEY_UP     38
#define  KEY_RIGHT  39
#define  KEY_DOWN   40
#define  KEY_SPACE    32
#define  KEY_PAGEUP   33
#define  KEY_PAGEDOWN 34

#define MAX_CAR_SELECTIONS 5


class GamePlay
{
public:
	GamePlay();
	~GamePlay();

	void init(int wdimx, int wdimy);
	void update();
	void resetPopulation();

	void createPopulation(const population_input& popInput, QPopulationTableWidget* _statsTable);
	bool loadMap(char* mapFilePath);	
	void loadImageMap(const char* mapFilePath);
	
	void addBox();
	void addCircle();

	bool writeTopNNToFile(const std::string& filepath, int ntop = 5);
	bool readNNFromFile(const std::string& filepath);
	struct NN_header {
		int nNNs = 0, nlayers = 0, nIneurons = 0, nOneurons = 0, nHneurons = 0;
	};
	bool readNNFileHeader(std::ifstream& file, NN_header& header);
	bool readNNmatrices(std::ifstream& file, const NN_header& header, NN::NNdata* nndata);
	bool readNNmatrix(std::ifstream& file, int height, int width, NN::Matrix* mat);

private:
	void initGUI();
	
	void updatePlayerCar();
	void updateCarSelection();

	void processObstacleSelection();

	void resetCurrentGen();
	void setNextGen();
	void clearCarSelection();
	void setNextGenAuto();

	void processUserClick();
	void carIsClicked(int id);

	float timer = 0.0f;
	float maxTimer = 30.0f;

	void initWorldGridSprites();
	void initSelectSprites();
	void processWorldGridSprites(std::vector<std::vector<bool>>* wgrid);
	std::vector<std::vector<Sprite*>> worldGridSprites;

	int vwdim[2];
	Car* playerCar = NULL;		
	Population population;
	std::vector<Sprite*> obstacleSprites;
	//int selectedObstacleId = 0;
	int lastPressedObstacleBlock = 0;
	glm::vec2 pressedObstacleBlockOffset;
	std::vector<FoodBlock*> foodSprites;
	//NNCarInfoFrame* carInfoFrame = NULL;	
	UISpriteAndTextButton_T1* resetGenBtn = NULL;
	UISpriteAndTextButton_T1* nextGenBtn  = NULL;
	UISpriteAndTextButton_T1* clearCarSelectionBtn = NULL;
	UISpriteAndTextButton_T1* nextGenAutoBtn = NULL;

	std::vector<Sprite*> selectSprites;
	std::vector<Car*> topCars;

};

