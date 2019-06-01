#include "QPopulationTableWidget.h"

void setRowBackground(const QBrush& brush, QAbstractItemModel* model, int row, const QModelIndex& parent) {
	if (!model || row < 0 || row >= model->rowCount(parent))
		return;
	if (parent.isValid() && parent.model() != model) {
		return;
		for (int i = 0; i < model->columnCount(parent); ++i)
			Q_ASSUME(model->setData(model->index(row, i, parent), brush, Qt::BackgroundRole));
	}
}

QPopulationTableRow::QPopulationTableRow(int row, QTableWidget* table) : QTableWidgetItem() {
	idItem = new QNumberTableItem();
	lifeItem = new QNumberTableItem();
	distItem = new QNumberTableItem();
	avgCpItem = new QNumberTableItem();
	cellsItem = new QNumberTableItem();
	fitnessItem = new QNumberTableItem();

	table->setItem(row, 0, idItem);
	table->setItem(row, 1, lifeItem);
	table->setItem(row, 2, distItem);
	table->setItem(row, 3, avgCpItem);
	table->setItem(row, 4, cellsItem);
}




void QPopulationTableRow::reset(){
	setColor(glm::vec3(1.0f));
}
void QPopulationTableRow::toogleSelected() {
	glm::vec3 col;
	if (!isSelected) {
		isSelected = true;
		col = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else {
		isSelected = false;
		col = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	setBold(isSelected);
	setTextColor(col);
}
void QPopulationTableRow::markDead() {
	setColor(glm::vec3(0.2f));
}
void QPopulationTableRow::setId(int id) {
	idItem->setInt(id);
}
void QPopulationTableRow::setLife(float life) {
	lifeItem->setPercentage(life);
}
void QPopulationTableRow::setDistance(float dist) {
	distItem->setFloat(dist, 0, 'f');
}
void QPopulationTableRow::setVisitedCells(int n) {
	cellsItem->setInt(n);
}
void QPopulationTableRow::setAvgCheckPointDist(float cpdist) {
	avgCpItem->setFloat(cpdist, 0, 'f');
}
void QPopulationTableRow::wflash() {}
void QPopulationTableRow::sflash() {}
void QPopulationTableRow::aflash() {}
void QPopulationTableRow::dflash() {}

bool QPopulationTableRow::hitTest(const glm::vec2& pos) {
	return false;
}

void QPopulationTableRow::setColor(const glm::vec3& c) {
	QTableWidgetItem* items[5] = { idItem, distItem, avgCpItem,cellsItem, lifeItem };
	QColor col = QColor(255 * c.x, 255 * c.y, 255 * c.z);
	for (int i = 0; i < 5; i++) {
		items[i]->setData(Qt::BackgroundRole, col);
	}

}
void QPopulationTableRow::setBold(bool state) {
	QTableWidgetItem* items[5] = { idItem, distItem, avgCpItem,cellsItem, lifeItem };
	QFont font;
	font.setBold(state);
	for (int i = 0; i < 5; i++) {
		items[i]->setFont(font);
	}
}
void QPopulationTableRow::setTextColor(const glm::vec3& c) {
	QTableWidgetItem* items[5] = { idItem, distItem, avgCpItem,cellsItem, lifeItem };
	QColor col = QColor(255 * c.x, 255 * c.y, 255 * c.z);
	for (int i = 0; i < 5; i++) {
		items[i]->setForeground(QBrush(col));
	}
}


QPopulationTableRow*  QPopulationTableWidget::newRow() {
	this->insertRow(this->rowCount());
	int currRow = this->rowCount() - 1;
	QPopulationTableRow* newRow = new QPopulationTableRow(currRow, this);
	this->setVerticalHeaderItem(currRow, newRow);

	setRowBackground(QColor(250, 0, 0), this->model(), currRow);

	return newRow;
}