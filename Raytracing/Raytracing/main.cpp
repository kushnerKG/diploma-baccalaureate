#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include "Controller.h"
#include <fstream>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	Controller* controller = new Controller();
	return a.exec();
}