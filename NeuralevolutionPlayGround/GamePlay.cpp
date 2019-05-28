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
}

void GamePlay::clearCarSelection() {
	population.clearCarSelection();
}
void GamePlay::setNextGen() {
	population.setNextGen();
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
}


void GamePlay::initGUI() {
	resetGenBtn = new UISpriteAndTextButton_T1(glm::vec3(0, 1, 0), glm::vec3(1.0, 1.0, 0.0), 
		glm::vec3(0.2f), "Reset Gen", glm::vec3(0), 
		glm::vec2(100.0f, 30.0f), 26.0f, glm::vec2(70.0f, 750.0f));
	resetGenBtn->realign();

	nextGenBtn = new UISpriteAndTextButton_T1(glm::vec3(0, 1, 0), glm::vec3(1.0, 1.0, 0.0),
		glm::vec3(0.2f), "Next Gen", glm::vec3(0),
		glm::vec2(100.0f, 30.0f), 26.0f, glm::vec2(180.0f, 750.0f));
	nextGenBtn->realign();

	nextGenAutoBtn = new UISpriteAndTextButton_T1(glm::vec3(0, 1, 0), glm::vec3(1.0, 1.0, 0.0),
		glm::vec3(0.2f), "Next Get Auto", glm::vec3(0),
		glm::vec2(100.0f, 30.0f), 26.0f, glm::vec2(290.0f, 750.0f));
	nextGenAutoBtn->realign();
	
	clearCarSelectionBtn = new UISpriteAndTextButton_T1(glm::vec3(0, 1, 0), glm::vec3(1.0, 1.0, 0.0),
		glm::vec3(0.2f), "Clear sel.", glm::vec3(0),
		glm::vec2(100.0f, 30.0f), 26.0f, glm::vec2(400.0f, 750.0f));
	clearCarSelectionBtn->realign();
}

void GamePlay::update() {
	bool ppressed = agk::GetPointerPressed();
	bool preleased = agk::GetPointerReleased();
	bool pstate = agk::GetPointerState();

	//carInfoFrame->postProcess();
	population.update();
	updatePlayerCar();
	updateCarSelection();
	processUserClick();
	resetGenBtn->processButtonState(ppressed, preleased, pstate);
	nextGenBtn->processButtonState(ppressed, preleased, pstate);
	nextGenAutoBtn->processButtonState(ppressed, preleased, pstate);
	clearCarSelectionBtn->processButtonState(ppressed, preleased, pstate);

	processObstacleSelection();

	//processWorldGridSprites(playerCar->getWorldGrid());

	agk::StepPhysics(1.0f / 60.0f);

	if (resetGenBtn->getButtonState()) {
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
	}

	std::string genstr = "Gen: " + TOSTR(population.getGenerationNumber(), 0);
	agk::Print(&genstr[0]);

	std::string tstr = TOSTR(timer, 2) + "/" + TOSTR(maxTimer, 2);
	
	timer += 0.1f;
	agk::Print(&tstr[0]);
	if (timer > maxTimer || population.getNaliveMembers() == 0) {
		timer = 0.0f;
		setNextGenAuto();
		maxTimer+=1.0f;
	}

	/*std::vector<float>* d = playerCar->getSensorRayDistances();
	for (float dist : (*d)) {
		agk::Print(agk::Str(dist,2));
	}*/

}

void GamePlay::updateCarSelection() {
	for (int i = 0; i < topCars.size(); i++) {
		selectSprites[i]->setPosition(topCars[i]->getPos());
	}

}

void GamePlay::updatePlayerCar() {
	playerCar->update();

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

bool GamePlay::loadMap(char* mapFilePath) {

	std::fstream file;
	file.open(mapFilePath);
	if (!file.is_open())
		return false;

	std::string type;
	std::string px, py, sx, sy, alphastr;
	glm::vec2 pos, size;
	float alpha;

	float dimx = (float)vwdim[0];// -400.0f;
	float dimy = (float)vwdim[1];

	while (!file.eof()) {
	
		//BOX FOOD OR CIRCLE
		file >> type;
		if(type[0] == 'B' || type[0] == 'C' || type[0] == 'F'){
			file >> px >> py;
			pos = glm::vec2(std::stof(px) * dimx, std::stof(py)* dimy);

			if(type[0] == 'B'){
				file >> sx >> sy >> alphastr;
			
				size = glm::vec2(std::stof(sx) * dimx, std::stof(sy) * dimy);
				alpha = std::stof(alphastr);
				glm::vec3 color = randVec3FromTo(0.0f, 1.0f);

				ObstacleBlock* ob = new ObstacleBlock(pos, size, color);
				ob->setRotation(alpha);
				obstacleSprites.push_back(ob);
			}
			else if (type[0] == 'F') {
				FoodBlock* food = new FoodBlock(pos);
				foodSprites.push_back(food);
			}
			else if (type[0] == 'C') {
				file >> sx;

				size = glm::vec2(std::stof(sx) * dimx, -1);
				glm::vec3 color = randVec3FromTo(0.0f, 1.0f);
				ObstacleCircle* oc = new ObstacleCircle(pos, size, color);
				obstacleSprites.push_back(oc);
			}
		}


	}


	file.close();
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
			agk::SetSpriteShape(s, (int)AgkSpriteShape::box);
		}
			
	}

	else if(agk::GetPointerReleased() && lastPressedObstacleBlock){
		agk::SetSpriteColorAlpha(lastPressedObstacleBlock, 255);
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
		if (agk::GetRawKeyState(KEY_LEFT))
			agk::SetSpriteAngle(s, agk::GetSpriteAngle(s) + rot_speed);
		else if (agk::GetRawKeyState(KEY_RIGHT))
			agk::SetSpriteAngle(s, agk::GetSpriteAngle(s) - rot_speed);

		if (agk::GetRawKeyState(KEY_Z))
			agk::SetSpriteScale(s, agk::GetSpriteScaleX(s)*(1.0f - sc_factor), agk::GetSpriteScaleY(s));
		else if (agk::GetRawKeyState(KEY_X))
			agk::SetSpriteScale(s, agk::GetSpriteScaleX(s)*(1.0f + sc_factor), agk::GetSpriteScaleY(s));
		
		if (agk::GetRawKeyState(KEY_C))
			agk::SetSpriteScale(s, agk::GetSpriteScaleX(s), agk::GetSpriteScaleY(s) * (1.0f - sc_factor));
		else if (agk::GetRawKeyState(KEY_V))
			agk::SetSpriteScale(s, agk::GetSpriteScaleX(s), agk::GetSpriteScaleY(s) *(1.0f + sc_factor));


	}


}


bool GamePlay::writeTopNNToFile(const std::string& filepath, int ntop) {
	std::vector<NN::NNdata*> topnns;
	population.getTopNNdata(topnns, ntop);

	std::ofstream file;
	file.open(filepath);
	if (!file.is_open())
		return false;

	file << "SIZE      " << topnns.size() << std::endl;
	file << "LAYERS    " << 2 << std::endl;
	file << "INEURONS  " << topnns[0]->W1.getHeight() << std::endl;
	file << "ONEURONS  " << topnns[0]->W3.getWidth() << std::endl;
	file << "HNEURONS  " << topnns[0]->W2.getWidth() << std::endl;

	int c = 0;
	for (NN::NNdata* nndata : topnns) {
		file << "NNDATA    " << c++ << std::endl;
		file << "W1\n";	nndata->W1.print(file);
		file << "W2\n";	nndata->W2.print(file);
		file << "W3\n";	nndata->W3.print(file);
		file << "B1\n";	nndata->B1.print(file);
		file << "B2\n";	nndata->B2.print(file);
		file << "B3\n";	nndata->B3.print(file);
	}

	file.close();
	return true;
}

bool GamePlay::readNNFileHeader(std::ifstream& file, NN_header& header) {

	std::string key;
	while (!file.eof()) {

		file >> key;
		std::string argstr; file >> argstr;
		int val = std::stoi(argstr);
		if (key == "SIZE")
			header.nNNs = val;
		else if (key == "LAYERS")
			header.nlayers = val;
		else if (key == "INEURONS")
			header.nIneurons = val;
		else if (key == "ONEURONS")
			header.nOneurons = val;
		else if (key == "HNEURONS")
			header.nHneurons = val;

		if (header.nNNs && header.nlayers && header.nIneurons && header.nHneurons && header.nOneurons)
			return true;
	}
	return false;
}

bool GamePlay::readNNmatrices(std::ifstream& file, const NN_header& header, NN::NNdata* nndata) {
	static std::map<std::string, int> keyMap = {
		{"W1", 0},{"W2", 1},{"W3", 2},{"B1", 3},{"B2", 4},{"B3", 5}
	};

	int matHeights[6] = { header.nIneurons, header.nHneurons, header.nHneurons, 1,1,1 };
	int matWidth[6] = { header.nHneurons, header.nHneurons, header.nOneurons, header.nHneurons,header.nHneurons,header.nOneurons};
	bool matsFound[6] = { false, false, false, false, false, false };
	NN::Matrix* matrices[6] = { &nndata->W1, &nndata->W2, &nndata->W3, &nndata->B1, &nndata->B2, &nndata->B3 };
	
	std::string key;
	while(!file.eof()){	
		file >> key;
		if (keyMap.find(key) != keyMap.end()) {
			int index = keyMap[key];

			int h = matHeights[index];
			int w = matWidth[index];
			matsFound[index] = true;
			
			if (!readNNmatrix(file, h, w, matrices[index]))
				return false;
		}

		bool allFound = true;
		for (int i = 0; i < 6; i++) {
			if (!matsFound[i]){
				allFound = false;
				break;
			}
		}
		if (allFound) return true;
		
	}
	return false;
}
bool GamePlay::readNNmatrix(std::ifstream& file, int height, int width, NN::Matrix* mat) {
	std::vector<std::vector<double>> data;
	std::string vstr;
	double v;
	for (int i = 0; i < height; i++) {
		std::vector<double> row;
		for (int j = 0; j < width; j++) {
			if (file.eof()) {
				file.close();
				return false;
			}
			file >> vstr;
			v = std::stod(vstr);
			row.push_back(v);
		}
		data.push_back(row);
	}

	(*mat) = NN::Matrix(data);
	return true;
}

bool GamePlay::readNNFromFile(const std::string& filepath) {
	std::vector<NN::NNdata> topnns;
	std::ifstream file;
	file.open(filepath);
	if (!file.is_open())
		return false;
	
	NN_header header;
	if (!readNNFileHeader(file, header))
		return false;

	for(int i = 0; i < header.nNNs; i++){
		NN::NNdata nndata;
		if (!readNNmatrices(file, header, &nndata))
			return false;
		topnns.push_back(nndata);
	}	
	return true;
}