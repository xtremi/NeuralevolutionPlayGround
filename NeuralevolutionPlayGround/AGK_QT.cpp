#include "AGK_QT.h"
#include "agk.h"

AGK_QT::AGK_QT(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);	
	connectSlotsSignals();
	setupUI();
}
void AGK_QT::setupUI() {
	ui.popSize_spinBox->setMinimum(1);
	ui.startPosX_doubleSpinBox->setMinimum(0.0f);
	ui.startPosX_doubleSpinBox->setMaximum(1.0f);
	ui.startPosY_doubleSpinBox->setMinimum(0.0f);
	ui.startPosY_doubleSpinBox->setMaximum(1.0f);

	ui.startDirX_doubleSpinBox->setMinimum(0.0f);
	ui.startDirX_doubleSpinBox->setMaximum(1.0f);
	ui.startDirY_doubleSpinBox->setMinimum(0.0f);
	ui.startDirY_doubleSpinBox->setMaximum(1.0f);
}

void AGK_QT::connectSlotsSignals() {
	connect(ui.pause_pushButton, SIGNAL(released()), this, SLOT(pauseButtonPressed()));
	connect(ui.go_pushButton, SIGNAL(released()), this, SLOT(goButonPressed()));
	connect(ui.clearAll_pushButton, SIGNAL(released()), this, SLOT(clearAllButtonPressed()));
	connect(ui.addBox_pushButton, SIGNAL(released()), this, SLOT(addBoxButtonPressed()));
	connect(ui.addCircle_pushButton, SIGNAL(released()), this, SLOT(addCircleButtonPressed()));
	connect(ui.saveNN_pushButton, SIGNAL(released()), this, SLOT(saveNNButtonPressed()));
	connect(ui.loadNN_pushButton, SIGNAL(released()), this, SLOT(loadNNButtonPressed()));
	connect(ui.action_loadWorld, SIGNAL(triggered()), this, SLOT(clickOnLoadWorld()));
}
void AGK_QT::showEvent(QShowEvent* ev) {
	QMainWindow::showEvent(ev);
	QMetaObject::invokeMethod(this, "afterWindowShown", Qt::ConnectionType::QueuedConnection);
}
void AGK_QT::afterWindowShown() {
	initAGK();
}
void AGK_QT::initAGK() {
	agkMain((HWND)ui.widget->winId());
}
void AGK_QT::stopAGK() {
	App.stop();
}
void AGK_QT::closeEvent(QCloseEvent *event){
	stopAGK();
	QMainWindow::closeEvent(event);
}


bool AGK_QT::loadWorld(const QString& filePath) {
	std::string fpstr = filePath.toUtf8().data();

	std::fstream file;
	file.open(fpstr);

	if (!file.is_open())
		return false;

	std::string type, arg1, arg2, arg3;
	population_input popInput;

	while (!file.eof()) {
		file >> type >> arg1;
		if (type == "PSIZE") {
			popInput.size = std::stoi(arg1);
		}
		else if (type == "PSTART" || type == "PDIR") {
			file >> arg2;
			glm::vec2 tmpvec(std::stof(arg1), std::stof(arg2));
			if (type == "PSTART")
				popInput.startpos = tmpvec;
			else
				popInput.startdir = tmpvec;
		}
		else if (type == "MAP") {
			App.gamePlay.loadMap(&arg1[0]);
		}
	}

	this->setGUIPopulationInput(popInput);

	return true;
}


void AGK_QT::pauseButtonPressed() {
	App.tooglePause();

	if (App.currentGameState() == game_state::PAUSE)
		ui.pause_pushButton->setText("Unpause");
	else
		ui.pause_pushButton->setText("Pause");

}

void AGK_QT::goButonPressed() {
	population_input popInput = getGUIPopulationInput();
	popInput.startpos.x *= (float)DEVICE_WIDTH;
	popInput.startpos.y *= (float)DEVICE_HEIGHT;
	App.gamePlay.createPopulation(popInput, ui.overview_tableWidget);	
	pauseButtonPressed();
}

void AGK_QT::clearAllButtonPressed() {
	App.gamePlay.resetPopulation();
	ui.overview_tableWidget->clear();
	ui.overview_tableWidget->setRowCount(0);
	pauseButtonPressed();
	
}
void AGK_QT::addBoxButtonPressed(){
	App.gamePlay.addBox();
}
void AGK_QT::addCircleButtonPressed(){
	App.gamePlay.addCircle();
}
void AGK_QT::saveNNButtonPressed() {
	if (!App.gamePlay.writeTopNNToFile("NN.txt", 5))
		int failed = 1;
}
void AGK_QT::loadNNButtonPressed() {
	if (!App.gamePlay.readNNFromFile("NN.txt"))
		int failed = 1;
}


void AGK_QT::clickOnLoadWorld() {
	QString filepath = QFileDialog::getOpenFileName(this, "Open World file", "", "txt Files (*.txt)");

	if (!filepath.isEmpty()){
		if (!loadWorld(filepath)) {

		}
	}
}


void AGK_QT::setGUIPopulationInput(const population_input& popInput) {
	ui.popSize_spinBox->setValue(popInput.size);
	ui.startPosX_doubleSpinBox->setValue(popInput.startpos.x);
	ui.startPosY_doubleSpinBox->setValue(popInput.startpos.y);
	ui.startDirX_doubleSpinBox->setValue(popInput.startdir.x);
	ui.startDirY_doubleSpinBox->setValue(popInput.startdir.y);
}

population_input AGK_QT::getGUIPopulationInput() {
	population_input popInput;
	popInput.size = ui.popSize_spinBox->value();
	popInput.startpos.x = ui.startPosX_doubleSpinBox->value();
	popInput.startpos.y = ui.startPosY_doubleSpinBox->value();
	popInput.startdir.x = ui.startDirX_doubleSpinBox->value();
	popInput.startdir.y = ui.startDirY_doubleSpinBox->value();

	

	return popInput;
}
