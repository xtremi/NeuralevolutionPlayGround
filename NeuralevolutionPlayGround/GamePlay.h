#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <map>

#include "ProjectIO.h"
#include "MapIO.h"
#include "NNIO.h"

#include "convinient_functions.h"
#include "Population.h"
#include "ObstacleSprite.h"
#include "FoodBlock.h"

#include "UI/UIButtonTypes.h"

#include <Qtablewidget>

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
	void updatePopulation();
	void updateInterface();
	void resetPopulation();

	void createPopulation(const population_input& popInput, QPopulationTableWidget* _statsTable);
	
	bool loadProject(char* filePath, population_input& popInput);
	bool saveProject(char* filePath);
	bool saveMap(char* mapFilePath);
	bool loadMap(char* mapFilePath);
	bool loadNNfile(char* filePath);
	bool saveNNfile(char* filePath);

	void loadImageMap(const char* mapFilePath);
	
	void addBox();
	void addCircle();
	void addBorder(double thk);

	void resetCurrentGen();
	void setNextGen();
	void clearCarSelection();
	void setNextGenAuto();

	void showSensorRays(bool state);


	void setMatrixWidgets(const std::vector<QTableWidget*> _matrixWidgets) {
		matrixWidgets = _matrixWidgets;
		initMatrixWidgets();
	}
	
private:
	void initGUI();
	
	void updatePlayerCar();
	void updateCarSelection();

	void processObstacleSelection();

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
	std::vector<ObstacleSprite*> obstacleSprites;
	std::vector<FoodBlock*> foodSprites;
	//int selectedObstacleId = 0;
	int lastPressedObstacleBlock = 0;
	glm::vec2 pressedObstacleBlockOffset;

	std::vector<Sprite*> selectSprites;
	std::vector<Car*> topCars;



	float nndataDisplayTimer = 0.0f;
	NN::NNdata* displayedNNdata = NULL;
	std::vector<QTableWidget*> matrixWidgets;
	void updateNNmatrixWidgets();
	void setDisplayedNNdata();
	void fillMatrixWidget(NN::Matrix* mat, QTableWidget* widget);
	void initMatrixWidgets();
};

