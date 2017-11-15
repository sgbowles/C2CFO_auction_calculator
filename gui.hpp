#ifndef GUI_HPP
#define GUI_HPP

#include <fstream>
#include <string>

#include <QWidget>
#include <QLayout>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QCompleter>
#include <QAbstractItemModel>
#include <QFile>
#include <QStandardItemModel>

class Calc_GUI :public QWidget {
Q_OBJECT

public:
	Calc_GUI(QWidget * parent = nullptr);
	QGroupBox * createPlayerSearch();
	QGroupBox * createCalc();
	QGroupBox * createValue();
	double calculate();
	QAbstractItemModel * createTable();

private slots:
	void salaryChanged(double num);
	void yearsChanged(double num);
	void playerSearched();

private:
	double curYears = 1;
	double curSalary = 0;
	double curMarket = 0;

	QLineEdit * playerSearch;
	QLabel * playerLabel;

	QDoubleSpinBox * market;
	QLabel * marketLabel;

	QDoubleSpinBox * salary;
	QLabel * salaryLabel;

	QDoubleSpinBox * years;
	QLabel * yearsLabel;

	QDoubleSpinBox * value;
	QLabel * valueLabel;

	QStandardItemModel * model;
	QCompleter * completer;
};

#endif // !GUI_HPP


