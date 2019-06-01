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
	ui.popSize_spinBox->setMaximum(1000);
	ui.popSize_spinBox->setValue(24);

	QDoubleSpinBox* spbxs[5] = {
		ui.startPosX_doubleSpinBox , ui.startPosY_doubleSpinBox,
		ui.startDirX_doubleSpinBox, ui.startDirY_doubleSpinBox,
		ui.borderW_doubleSpinBox
	};

	for (int i = 0; i < 5; i++) {
		spbxs[i]->setMinimum(0.0f);
		spbxs[i]->setMaximum(1.0f);
		spbxs[i]->setSingleStep(0.05f);
	}

	ui.startPosX_doubleSpinBox->setValue(0.5f);
	ui.startPosY_doubleSpinBox->setValue(0.5f);
	ui.startDirX_doubleSpinBox->setValue(1.0f);
	ui.borderW_doubleSpinBox->setDecimals(3);
	ui.borderW_doubleSpinBox->setValue(0.005f);
	ui.borderW_doubleSpinBox->setSingleStep(0.05f);

	
	ui.showSensorRays_checkBox->click();
}

void AGK_QT::connectSlotsSignals() {
	connect(ui.pause_pushButton, SIGNAL(released()), this, SLOT(pauseButtonPressed()));
	connect(ui.go_pushButton, SIGNAL(released()), this, SLOT(goButonPressed()));
	connect(ui.clearAll_pushButton, SIGNAL(released()), this, SLOT(clearAllButtonPressed()));
	
	connect(ui.addBox_pushButton, SIGNAL(released()), this, SLOT(addBoxButtonPressed()));
	connect(ui.addCircle_pushButton, SIGNAL(released()), this, SLOT(addCircleButtonPressed()));
	connect(ui.addBorder_pushButton, SIGNAL(released()), this, SLOT(addBorderButtonPressed()));

	connect(ui.nextGenAuto_pushButton, SIGNAL(released()), this, SLOT(nextGenAutoButtonPressed()));
	connect(ui.nextGenSel_pushButton, SIGNAL(released()), this, SLOT(nextGenSelButtonPressed()));
	connect(ui.resetGen_pushButton, SIGNAL(released()), this, SLOT(resetGenButtonPressed()));
	connect(ui.clearSel_pushButton, SIGNAL(released()), this, SLOT(clearSelButtonPressed()));

	connect(ui.action_saveProject, SIGNAL(triggered()), this, SLOT(clickOnSaveProject()));
	connect(ui.action_loadProject, SIGNAL(triggered()), this, SLOT(clickOnLoadProject()));

	connect(ui.saveNN_pushButton, SIGNAL(released()), this, SLOT(saveNNButtonPressed()));
	connect(ui.loadNN_pushButton, SIGNAL(released()), this, SLOT(loadNNButtonPressed()));
	connect(ui.saveMap_pushButton, SIGNAL(released()), this, SLOT(saveMapButtonPressed()));
	connect(ui.loadMap_pushButton, SIGNAL(released()), this, SLOT(loadMapButtonPressed()));

	connect(ui.showSensorRays_checkBox, SIGNAL(clicked(bool)), this, SLOT(showSensorRaysCheckBoxStateChanged(bool)));
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
	
	std::vector<QTableWidget*> matrixWidgets = { 
		ui.xNN_tableWidget, ui.w1NN_tableWidget, 
		ui.h1NN_tableWidget, ui.w2NN_tableWidget, 
		ui.h2NN_tableWidget, ui.w3NN_tableWidget, ui.yNN_tableWidget };

	App.gamePlay.setMatrixWidgets(matrixWidgets);
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
void AGK_QT::addBorderButtonPressed() {
	App.gamePlay.addBorder(ui.borderW_doubleSpinBox->value());
}
void AGK_QT::nextGenAutoButtonPressed(){
	App.gamePlay.setNextGenAuto();
}
void AGK_QT::nextGenSelButtonPressed() {
	App.gamePlay.setNextGen();
}
void AGK_QT::resetGenButtonPressed() {
	App.gamePlay.resetCurrentGen();
}
void AGK_QT::clearSelButtonPressed() {
	App.gamePlay.clearCarSelection();
}

void AGK_QT::saveNNButtonPressed() {
	QString filepath = QFileDialog::getSaveFileName(this, "Save Neural Network Matrix file", "", "txt Files (*.txt)");
	if(!filepath.isEmpty()){
		if (!App.gamePlay.saveNNfile(filepath.toUtf8().data()))
			int failed = 1;
	}
}
void AGK_QT::loadNNButtonPressed() {
	QString filepath = QFileDialog::getOpenFileName(this, "Open Neural Network Matrix file", "", "txt Files (*.txt)");
	if(!filepath.isEmpty()){
		if (!App.gamePlay.loadNNfile(filepath.toUtf8().data()))
			int failed = 1;
	}
}
void AGK_QT::saveMapButtonPressed(){
	QString filepath = QFileDialog::getSaveFileName(this, "Save Map file", "", "txt Files (*.txt)");
	if (!filepath.isEmpty()) {
		if (!App.gamePlay.saveMap(filepath.toUtf8().data()))
			int failed = 1;
	}
}
void AGK_QT::loadMapButtonPressed(){
	QString filepath = QFileDialog::getOpenFileName(this, "Open Map file", "", "txt Files (*.txt)");
	if (!filepath.isEmpty()) {
		if (!App.gamePlay.loadMap(filepath.toUtf8().data()))
			int failed = 1;
	}
}

void AGK_QT::clickOnSaveProject() {
	QString filepath = QFileDialog::getSaveFileName(this, "Save Project file", "", "txt Files (*.txt)");
	if (!filepath.isEmpty()) {
		if (!App.gamePlay.saveProject(filepath.toUtf8().data()))
			int failed = 1;
	}
}

void AGK_QT::clickOnLoadProject() {
	QString filepath = QFileDialog::getOpenFileName(this, "Open Project file", "", "txt Files (*.txt)");

	population_input popInput;

	if (!filepath.isEmpty()){
		if (!App.gamePlay.loadProject(filepath.toUtf8().data(), popInput))
			int failed = 1;
		else
			setGUIPopulationInput(popInput);
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


void AGK_QT::changeEvent(QEvent *event) {
	if (event->type() == QEvent::WindowStateChange) {
		if (!isMinimized()) {
			setAttribute(Qt::WA_Mapped);
		}
	}
}

void AGK_QT::showSensorRaysCheckBoxStateChanged(bool checked) {
	App.gamePlay.showSensorRays(checked);
}
