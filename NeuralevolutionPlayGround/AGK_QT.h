#pragma once

#include <QtWidgets/QMainWindow>
#include <QAbstractNativeEventFilter>
#include <QFileDialog>
#include "ui_AGK_QT.h"

#include "AgkTemplate.h"


class MyEventFilter : public QAbstractNativeEventFilter
{
public:
	MyEventFilter(QWidget* _widget) { agkWidget = _widget; }

	QWidget* agkWidget = NULL;

	bool nativeEventFilter(const QByteArray &eventType, void *message, long* res) override
	{
		if (!agkWidget)
			int tmp = 1;

		if(agkWidget->underMouse()){
			if (eventType == "windows_generic_MSG") {
				res = (long*)WndProc(((MSG*)message)->hwnd, ((MSG*)message)->message, ((MSG*)message)->wParam, ((MSG*)message)->lParam);
			}
		}
		return false;
	}
};



class AGK_QT : public QMainWindow
{
	Q_OBJECT

private slots:
	void pauseButtonPressed();
	void goButonPressed();
	void afterWindowShown();

	void addBoxButtonPressed();
	void addCircleButtonPressed();
	void addBorderButtonPressed();

	void clearAllButtonPressed();

	void saveNNButtonPressed();
	void loadNNButtonPressed();
	void saveMapButtonPressed();
	void loadMapButtonPressed();
	void clickOnLoadProject();
	void clickOnSaveProject();

	void nextGenAutoButtonPressed();
	void nextGenSelButtonPressed();
	void resetGenButtonPressed();
	void clearSelButtonPressed();

	void showSensorRaysCheckBoxStateChanged(bool checked);

public:
	AGK_QT(QWidget *parent = Q_NULLPTR);
	QWidget* getAgkWidget() { return ui.widget; }

private:
	Ui::AGK_QTClass ui;

	void closeEvent(QCloseEvent *event);
	void connectSlotsSignals();
	void setupUI();

	void initAGK();	
	void stopAGK();

	void setGUIPopulationInput(const population_input& popInput);
	population_input getGUIPopulationInput();

	void showEvent(QShowEvent* ev);
	void changeEvent(QEvent *event);


};
