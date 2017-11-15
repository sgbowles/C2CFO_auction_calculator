#include "gui.hpp"

#include <QApplication>

int main(int argc, char *argv[]) {
	//Q_INIT_RESOURCE(completer);
	QApplication app(argc, argv);

	Calc_GUI widget;
	widget.show();
	return app.exec();
}