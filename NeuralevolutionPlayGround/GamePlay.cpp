#include "GamePlay.h"

GamePlay::GamePlay(){}
GamePlay::~GamePlay(){}


void GamePlay::init(int wdimx, int wdimy) {
	std::srand(time(0));
	agk::SetPhysicsGravity(0.0f, 0.0f);
	vwdim[0] = wdimx; vwdim[1] = wdimy;

	loadCarImage();
	loadCircleImage();
	initGUI();
	initWorldGridSprites();
	initSelectSprites();
	
	maxTimer = 25.0f;
	//loadMap("map_05.txt");
	//loadImageMap("corners_map.png");

	agk::SetPhysicsWallLeft(1);
	agk::SetPhysicsWallRight(1);
	agk::SetPhysicsWallTop(1);
	agk::SetPhysicsWallBottom(1);

}

void GamePlay::resetPopulation() {
	delete playerCar;
	playerCar = NULL;
	population.reset();	
	topCars.clear();
	for (Sprite* s : selectSprites)
		s->hide();
	lastPressedObstacleBlock = 0;
	timer = 0.0f;
	maxTimer = 30.0f;
	
}

void GamePlay::createPopulation(const population_input& popInput, QPopulationTableWidget* _statsTable) {
	playerCar = new Car(popInput.startpos, popInput.startdir);
	//carInfoFrame = new NNCarInfoFrame(glm::vec2(vwdim[0] - 400.0f, 0.0f), glm::vec2(100.0f, 35.0f), 22);
	population.init(popInput, _statsTable);
	setDisplayedNNdata();
}

void GamePlay::clearCarSelection() {
	population.clearCarSelection();
}
void GamePlay::setNextGen() {
	population.setNextGen();
	setDisplayedNNdata();
}
void GamePlay::resetCurrentGen(){
	population.resetCurrentGen();
}
void GamePlay::setNextGenAuto(){
	population.setNextGenAuto();
	topCars = population.getTopCars();
	for (int i = 0; i < topCars.size(); i++) {
		selectSprites[i]->setPosition(topCars[i]->getPos());
		selectSprites[i]->show();
	}
	setDisplayedNNdata();
}
void GamePlay::showSensorRays(bool state) {
	population.showSensorRays(state);
}

void GamePlay::initGUI() {}

void GamePlay::update() {
	updatePopulation();
	updateInterface();
	updateNNmatrixWidgets();
}

void GamePlay::updatePopulation() {
	population.update();
	updatePlayerCar();

	timer += 0.1f;
	if (timer > maxTimer || population.getNaliveMembers() == 0) {
		timer = 0.0f;
		setNextGenAuto();
		maxTimer += 1.0f;
	}

	agk::StepPhysics(1.0f / 60.0f);


}
void GamePlay::updateInterface() {
	bool ppressed = agk::GetPointerPressed();
	bool preleased = agk::GetPointerReleased();
	bool pstate = agk::GetPointerState();

	updateCarSelection();
	processUserClick();
	/*resetGenBtn->processButtonState(ppressed, preleased, pstate);
	nextGenBtn->processButtonState(ppressed, preleased, pstate);
	nextGenAutoBtn->processButtonState(ppressed, preleased, pstate);
	clearCarSelectionBtn->processButtonState(ppressed, preleased, pstate);*/

	processObstacleSelection();

	/*if (resetGenBtn->getButtonState()) {
		resetCurrentGen();
	}
	else if (nextGenBtn->getButtonState()) {
		setNextGen();
	}
	else if (clearCarSelectionBtn->getButtonState()) {
		clearCarSelection();
	}
	else if (nextGenAutoBtn->getButtonState()) {
		setNextGenAuto();
	}*/

	std::string genstr = "Gen: " + TOSTR(population.getGenerationNumber(), 0);
	agk::Print(&genstr[0]);

	std::string tstr = TOSTR(timer, 2) + "/" + TOSTR(maxTimer, 2);
	agk::Print(&tstr[0]);
	
}



void GamePlay::updateCarSelection() {
	for (int i = 0; i < topCars.size(); i++) {
		selectSprites[i]->setPosition(topCars[i]->getPos());
	}

}

void GamePlay::updatePlayerCar() {
	playerCar->update(true);

	if (agk::GetRawKeyState(KEY_W)) {
		playerCar->gas();
	}
	else if (agk::GetRawKeyState(KEY_S)) {
		playerCar->back();
	}
	if (agk::GetRawKeyState(KEY_A)) {
		playerCar->turnLeft();
	}
	else if (agk::GetRawKeyState(KEY_D)) {
		playerCar->turnRight();
	}
}


void GamePlay::loadImageMap(const char* mapFilePath) {

	AgkImage img((char*)mapFilePath);
	Sprite* map = new Sprite(&img, glm::vec2(0.0f), glm::vec2(vwdim[0] - 400.0f, vwdim[1]), AgkHAlign::left, AgkVAlign::top);
	map->setShape(AgkSpriteShape::polygon);
	agk::SetSpritePhysicsOn(map->getID() , 1);
	map->setShape(AgkSpriteShape::polygon);
	agk::SetSpritePhysicsRestitution(map->getID(), 0.5f);
	agk::SetSpriteCategoryBit(map->getID(), OBSTACLE_SPRITE_CATEGORY, 1);
	map->setDepth(15);

}

bool GamePlay::loadProject(char* filePath, population_input& popInput) {
	ProjectIO projectIO;
	std::string mapFile, nnFile;
	if (!projectIO.read(filePath, popInput, mapFile, nnFile))
		return false;

	if (!mapFile.empty()){
		if (!loadMap(&mapFile[0]))
			return false;
	}

	if (!nnFile.empty()){
		if (!loadNNfile(&nnFile[0]))
			return false;
	}

	return true;
}
bool GamePlay::saveProject(char* filePath) {	
	std::string projectFilePath = std::string(filePath);
	size_t lastindex = projectFilePath.find_last_of(".");
	std::string tmpStr = projectFilePath.substr(0, lastindex); //no extension

	std::string mapFilePath = tmpStr + "_map.txt";
	std::string nnFilePath = tmpStr + "_nn.txt";

	ProjectIO projectIO;
	population_input popInput = population.getCurrentPopulationInput();
	if (!projectIO.write(projectFilePath, popInput, mapFilePath, nnFilePath))
		return false;
	
	return true;
}
bool GamePlay::saveMap(char* mapFilePath) {
	MapIO mapIO;
	if (!mapIO.write(mapFilePath, vwdim, &obstacleSprites, &foodSprites))
		return false;
	return true;
}
bool GamePlay::loadMap(char* mapFilePath) {
	MapIO mapIO;
	if (!mapIO.read(mapFilePath, vwdim, &obstacleSprites, &foodSprites))
		return false;
	return true;
}
bool GamePlay::loadNNfile(char* filePath) {
	NNIO nnIO;
	std::vector<NN::NNdata>* nndatas;
	int genN;
	if (!nnIO.read(filePath, nndatas, genN))
		return false;
	return true;
}
bool GamePlay::saveNNfile(char* filePath) {
	NNIO nnIO;
	
	std::vector<NN::NNdata*> topNNdatas;
	population.getTopNNdata(topNNdatas, 5);
	int genN = population.getGenerationNumber();

	if (!nnIO.write(filePath, &topNNdatas, genN))
		return false;

	return true;
}


void GamePlay::addBox() {
	glm::vec3 color = randVec3FromTo(0.0f, 1.0f);
	ObstacleBlock* ob = new ObstacleBlock(glm::vec2(vwdim[0]/2, vwdim[1] / 2), glm::vec2(50.0f), color);
	obstacleSprites.push_back(ob);
}
void GamePlay::addCircle() {
	glm::vec3 color = randVec3FromTo(0.0f, 1.0f);
	ObstacleCircle* oc = new ObstacleCircle(glm::vec2(vwdim[0] / 2, vwdim[1] / 2), glm::vec2(50.0f), color);
	obstacleSprites.push_back(oc);
}
void GamePlay::addBorder(double thk) {

	thk *= (float)vwdim[0];

	float SX = (float)vwdim[0];
	float SY = (float)vwdim[1];

	glm::vec2 poss[4];
	glm::vec2 sizes[4];
	poss[0] = glm::vec2(0.0f, 0.0f);			// ------
	poss[1] = glm::vec2(0.0f, thk);				// |
	poss[2] = glm::vec2(0.0f, SY - thk);		//		|
	poss[3] = glm::vec2(SX - thk, thk);		//  -----

	sizes[0] = glm::vec2(SX, thk);
	sizes[1] = glm::vec2(thk, SY - 2*thk);
	sizes[2] = sizes[0];
	sizes[3] = sizes[1];

	for (int i = 0; i < 4; i++) {
		glm::vec3 color = randVec3FromTo(0.0f, 1.0f);
		ObstacleBlock* ob = new ObstacleBlock(poss[i], sizes[i], color);
		obstacleSprites.push_back(ob);
	}
}

void GamePlay::processUserClick() {

	if (agk::GetPointerReleased()) {
		glm::vec2 ppos(agk::GetPointerX(), agk::GetPointerY());
		
		int id = population.hitTest(ppos);

		if (id > -1) {
			carIsClicked(id);
		}

	}

}

void GamePlay::carIsClicked(int id) {
	population.toogleCarSelection(id);
}

void GamePlay::initWorldGridSprites(){
	static const float H = vwdim[0];
	static const float W = vwdim[1];
	static const int ncells = 10;

	worldGridSprites = std::vector<std::vector<Sprite*>>(ncells, std::vector<Sprite*>(ncells));

	glm::vec2 cellsize(H / (float)ncells, W / (float)ncells);
	glm::vec2 pos(0.0f);


	for (int i = 0; i < ncells; i++) {
		for (int j = 0; j < ncells; j++) {
			
			Sprite* s = new Sprite(glm::vec3(0.2f), pos, cellsize, AgkHAlign::left, AgkVAlign::top);
			s->setColorAlpha(0.2f);
			worldGridSprites[i][j] = s;
			pos.y += cellsize.y;
		}
		pos.x += cellsize.x;		
		pos.y = 0.0f;
	}

}
void GamePlay::processWorldGridSprites(std::vector<std::vector<bool>>* wgrid) {
	int N = wgrid->size();

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if ((*wgrid)[i][j]){
				worldGridSprites[i][j]->setColor(glm::vec3(1, 0, 0));
				worldGridSprites[i][j]->setColorAlpha(0.3f);
			}
		}
	}

}

void GamePlay::initSelectSprites() {
	AgkImage* img = new AgkImage("select_image.png");
	for (int i = 0; i < 5; i++) {
		Sprite* s = new Sprite(img, glm::vec2(0), glm::vec2(35.0f));
		s->hide();
		selectSprites.push_back(s);
	}

}

static const float move_speed = 2.5f;
static const float rot_speed = 2.5f;
static const float sc_factor = 0.01f;
void GamePlay::processObstacleSelection() {
	
	if (agk::GetPointerPressed() && !lastPressedObstacleBlock) {
		lastPressedObstacleBlock = agk::GetSpriteHitCategory(OBSTACLE_SPRITE_CATEGORY, agk::GetPointerX(), agk::GetPointerY());		
		if (lastPressedObstacleBlock) {
			int s = lastPressedObstacleBlock;
			agk::SetSpriteColorAlpha(s, 125);
			glm::vec2 pressedPos(agk::GetPointerX(), agk::GetPointerY());
			glm::vec2 objPos(agk::GetSpriteX(s), agk::GetSpriteY(s));
			pressedObstacleBlockOffset = pressedPos - objPos;
			agk::SetSpriteOffset(s, pressedObstacleBlockOffset.x, pressedObstacleBlockOffset.y);
			//agk::SetSpritePhysicsOn(s, 1);			
		}
			
	}

	else if(agk::GetPointerReleased() && lastPressedObstacleBlock){
		int s = lastPressedObstacleBlock;
		if (agk::GetSpriteGroup(s) == OBSTACLE_BLOCK_GROUP)
			agk::SetSpriteShape(s, (int)AgkSpriteShape::box);
		else
			agk::SetSpriteShape(s, (int)AgkSpriteShape::circle);
		agk::SetSpriteColorAlpha(s, 255);
		lastPressedObstacleBlock = 0;
	}
	else if (lastPressedObstacleBlock && agk::GetPointerState()) {
		//glm::vec2 objPos(agk::GetSpriteX(s), agk::GetSpriteY(s));
		//glm::vec2 newPos = glm::vec2(agk::GetPointerX(), agk::GetPointerY());
		agk::SetSpritePositionByOffset(lastPressedObstacleBlock, agk::GetPointerX(), agk::GetPointerY());
	}
	
	if (lastPressedObstacleBlock){
		/*if (agk::GetRawKeyState(KEY_RIGHT))
			agk::SetSpriteX(s, agk::GetSpriteX(s) + move_speed);
		else if (agk::GetRawKeyState(KEY_LEFT))
			agk::SetSpriteX(s, agk::GetSpriteX(s) - move_speed);
		
		if (agk::GetRawKeyState(KEY_UP))
			agk::SetSpriteY(s, agk::GetSpriteY(s) - move_speed);
		else if (agk::GetRawKeyState(KEY_DOWN))
			agk::SetSpriteY(s, agk::GetSpriteY(s) + move_speed);*/
		int s = lastPressedObstacleBlock;
		int g = agk::GetSpriteGroup(s);

		if (agk::GetRawKeyState(KEY_LEFT))
			agk::SetSpriteAngle(s, agk::GetSpriteAngle(s) + rot_speed);
		else if (agk::GetRawKeyState(KEY_RIGHT))
			agk::SetSpriteAngle(s, agk::GetSpriteAngle(s) - rot_speed);

		if (agk::GetRawKeyState(KEY_Z)){			
			agk::SetSpriteScale(s, agk::GetSpriteScaleX(s)*(1.0f - sc_factor), agk::GetSpriteScaleY(s));
		}
		else if (agk::GetRawKeyState(KEY_X))
			agk::SetSpriteScale(s, agk::GetSpriteScaleX(s)*(1.0f + sc_factor), agk::GetSpriteScaleY(s));
		
		if (agk::GetRawKeyState(KEY_C))
			agk::SetSpriteScale(s, agk::GetSpriteScaleX(s), agk::GetSpriteScaleY(s) * (1.0f - sc_factor));
		else if (agk::GetRawKeyState(KEY_V))
			agk::SetSpriteScale(s, agk::GetSpriteScaleX(s), agk::GetSpriteScaleY(s) *(1.0f + sc_factor));


	}


}


void GamePlay::setDisplayedNNdata() {
	std::vector<NN::NNdata*> nndatas;
	population.getTopNNdata(nndatas, 1);

	displayedNNdata = nndatas[0];
	
}

void GamePlay::initMatrixWidgets() {
	NN::Matrix* matrices[7] = {
		&displayedNNdata->X, &displayedNNdata->W1,&displayedNNdata->H1,
		&displayedNNdata->W2, &displayedNNdata->H2,&displayedNNdata->W3,
		&displayedNNdata->Y	
	};

	for (int i = 0; i < 7; i++) {
		int w = matrices[i]->getWidth();
		int h = matrices[i]->getHeight();
		matrixWidgets[i]->setRowCount(h);
		matrixWidgets[i]->setColumnCount(w);

		for (int row = 0; row < h; row++) {
			for (int column = 0; column < w; column++) {
				QTableWidgetItem *item = new QTableWidgetItem(QString::number(0.0f, 'f', 2));
				matrixWidgets[i]->setItem(row, column, item);
			}
		}

	}
}


static const float MAX_NNDATA_DISPLAY_TIMER = 2.0f;
static const float DELTA_NNDATA_DISPLAY_TIMER = 0.25f;
void GamePlay::updateNNmatrixWidgets(){

	nndataDisplayTimer += DELTA_NNDATA_DISPLAY_TIMER;
	if(nndataDisplayTimer > MAX_NNDATA_DISPLAY_TIMER){
		nndataDisplayTimer = 0.0f;
		int c = 0;
		fillMatrixWidget(&displayedNNdata->X, matrixWidgets[c++]);
		fillMatrixWidget(&displayedNNdata->W1, matrixWidgets[c++]);
		fillMatrixWidget(&displayedNNdata->H1, matrixWidgets[c++]);
		fillMatrixWidget(&displayedNNdata->W2, matrixWidgets[c++]);
		fillMatrixWidget(&displayedNNdata->H2, matrixWidgets[c++]);
		fillMatrixWidget(&displayedNNdata->W3, matrixWidgets[c++]);
		fillMatrixWidget(&displayedNNdata->Y, matrixWidgets[c++]);
	}

}


void GamePlay::fillMatrixWidget(NN::Matrix* mat, QTableWidget* widget) {

	int h = mat->getHeight();
	int w = mat->getWidth();

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			widget->item(i, j)->setText(QString::number(mat->at(i, j), 'f', 2));
		}
	}

}
