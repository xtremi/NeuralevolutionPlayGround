#include "AGK_QT.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	AGK_QT w;
	w.show();

	a.installNativeEventFilter(new MyEventFilter(w.getAgkWidget()));

	return a.exec();
}
