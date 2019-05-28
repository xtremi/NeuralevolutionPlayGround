#pragma once
#include <QTableWidget>
#include "glm/glm.hpp"

void setRowBackground(
	const QBrush& brush, QAbstractItemModel* model, 
	int row, const QModelIndex& parent = QModelIndex()
);

class QNumberTableItem : public QTableWidgetItem {
public:
	QNumberTableItem() :QTableWidgetItem() {}
	void setFloat(float v, int p, char type) {
		setText(QString::number(v, type, p));
	}
	void setInt(int v) {
		setText(QString::number(v));
	}
	void setPercentage(float val) {
		setText(QString::number(val*100.0f, 'f', 0) + "%");
	}
};

class QPopulationTableRow : public QTableWidgetItem
{
public:
	QPopulationTableRow(int row, QTableWidget* table);

	void reset();
	void setId(int id);
	void setLife(float life);
	void setDistance(float dist);
	void setAvgCheckPointDist(float cpdist);
	void setVisitedCells(int n);
	void wflash();
	void sflash();
	void aflash();
	void dflash();
	void toogleSelected();
	void markDead();
	bool hitTest(const glm::vec2& pos);

private:
	QNumberTableItem* idItem		= NULL;
	QNumberTableItem* distItem		= NULL;
	QNumberTableItem* avgCpItem		= NULL;
	QNumberTableItem* cellsItem		= NULL;
	QNumberTableItem* fitnessItem	= NULL;
	QNumberTableItem* lifeItem		= NULL;
	bool isSelected = false;

	void setColor(const glm::vec3& c);
	void setBold(bool state);
	void setTextColor(const glm::vec3& c);
};

class QPopulationTableWidget : public QTableWidget
{
public:
	QPopulationTableWidget(QWidget* parent = NULL) : QTableWidget(parent){
		this->setColumnCount(5);
		static const int colws[5] = { 10, 30, 30, 30, 30 };
		for (int i = 0; i < 5; i++)
			this->setColumnWidth(i, colws[i]);
		
		QStringList headerList = { "ID", "Life", "Fist", "Cp", "Cells" };

		this->setHorizontalHeaderLabels(headerList);

		/*this->setRowCount(5);
		setRowBackground(QColor(250, 0, 0), this->model(), 2);*/
	}

	QPopulationTableRow* newRow();




};