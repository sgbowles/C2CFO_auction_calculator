#include "gui.hpp"

#include <QDebug>

Calc_GUI::Calc_GUI(QWidget * parent):QWidget(parent) {
	auto parentLayout = new QVBoxLayout;

	parentLayout->addWidget(createPlayerSearch());
	parentLayout->addWidget(createCalc());
	parentLayout->addWidget(createValue());

	this->setLayout(parentLayout);
}

QGroupBox * Calc_GUI::createPlayerSearch() {
	completer = new QCompleter(this);
	completer->setModel(createTable());
	completer->setCompletionMode(QCompleter::PopupCompletion);
	completer->setFilterMode(Qt::MatchContains);

	completer->setWrapAround(true);
	playerSearch = new QLineEdit(this);
	playerLabel = new QLabel("Player Name:");
	market = new QDoubleSpinBox(this);
	marketLabel = new QLabel("Market Value:");

	playerSearch->setPlaceholderText("Enter Player Name");
	playerSearch->setCompleter(completer);

	connect(playerSearch, static_cast<void (QLineEdit::*)(void)>(&QLineEdit::editingFinished),
		this, static_cast<void (Calc_GUI::*)(void)>(&Calc_GUI::playerSearched));

	market->setDecimals(0);
	market->setReadOnly(true);
	market->setRange(0.0, 1000);
	market->setButtonSymbols(QAbstractSpinBox::NoButtons);
	market->setValue(curMarket);

	auto searchLayout = new QGridLayout;

	searchLayout->addWidget(playerLabel, 0, 0);
	searchLayout->addWidget(playerSearch, 0, 1);
	searchLayout->addWidget(marketLabel, 1, 0);
	searchLayout->addWidget(market, 1, 1);

	auto searchBox = new QGroupBox("Enter Player Name to Search");

	searchBox->setLayout(searchLayout);

	return searchBox;
}

QGroupBox * Calc_GUI::createCalc() {
	salaryLabel = new QLabel("Enter the salary:");
	salary = new QDoubleSpinBox(this);
	yearsLabel = new QLabel("Enter the number of years:");
	years = new QDoubleSpinBox(this);

	salary->setDecimals(0);
	salary->setRange(0, 10000);

	years->setDecimals(0);
	years->setRange(1, 5);
	years->setValue(1);

	connect(salary, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
		this, static_cast<void (Calc_GUI::*)(double)>(&Calc_GUI::salaryChanged));

	connect(years, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
		this, static_cast<void (Calc_GUI::*)(double)>(&Calc_GUI::yearsChanged));

	auto calcLayout = new QGridLayout;

	calcLayout->addWidget(salaryLabel, 0, 0);
	calcLayout->addWidget(salary, 0, 1);
	calcLayout->addWidget(yearsLabel, 1, 0);
	calcLayout->addWidget(years, 1, 1);

	auto calcBox = new QGroupBox("Enter Salary and Years to Calculate");

	calcBox->setLayout(calcLayout);

	return calcBox;
}

QGroupBox * Calc_GUI::createValue() {
	valueLabel = new QLabel("Contract value:");
	value = new QDoubleSpinBox(this);

	value->setReadOnly(true);
	value->setDecimals(2);
	value->setRange(0, 100000);
	value->setButtonSymbols(QAbstractSpinBox::NoButtons);

	auto valueLayout = new QGridLayout;

	valueLayout->addWidget(valueLabel, 0, 0);
	valueLayout->addWidget(value, 0, 1);

	auto valueBox = new QGroupBox;

	valueBox->setLayout(valueLayout);

	return valueBox;
}

double Calc_GUI::calculate() {
	double value = 0;
	double salaryVal = curSalary - curMarket;
	double cubedSal = pow(salaryVal, 3);

	if (curYears == 1) {
		value = .00015*cubedSal + curMarket;
	}
	else if (curYears == 2) {
		value = .00017*cubedSal + curMarket * 1.03;
	}
	else if (curYears == 3) {
		value = .00019*cubedSal + curMarket * 1.06;
	}
	else if (curYears == 4) {
		value = .00022*cubedSal + curMarket * 1.1;
	}
	else if (curYears == 5) {
		value = .00025*cubedSal + curMarket * 1.15;
	}
	return value;
}

void Calc_GUI::salaryChanged(double num) {
	curSalary = num;
	double calc_num = calculate();
	value->setValue(calc_num);
}

void Calc_GUI::yearsChanged(double num) {
	curYears = num;
	double calc_num = calculate();
	value->setValue(calc_num);
}
void Calc_GUI::playerSearched() {
	QString name = playerSearch->text();
	//qDebug() << name;
	for (int i = 0; i < model->rowCount(); ++i) {
		QString temp;
		QStandardItem * tempItem = model->item(i, 0);
		temp = tempItem->text();
		if (temp == name) {
			QStandardItem * tempItemSal = model->item(i, 1);
			temp = tempItemSal->text();
			double number = temp.toDouble();
			curMarket = number;
			market->setValue(curMarket);
		}
		//qDebug() << temp;
	}
}

QAbstractItemModel * Calc_GUI::createTable() {
	QFile file("calc_full.csv");
	QStandardItemModel *m;
	if (file.open(QFile::ReadOnly)) {
		QStringList words;

		while (!file.atEnd()) {
			QByteArray line = file.readLine();
			//qDebug(line);
			if (!line.isEmpty())
				words << line.trimmed();
		}
		m = new QStandardItemModel(words.count(), 2, completer);
		for (int i = 0; i < words.count(); ++i) {
			QModelIndex playerName = m->index(i, 0);
			QModelIndex salaryString = m->index(i, 1);
			int delim = words[i].lastIndexOf(',');
			QString country = words[i].mid(1, delim -2);
			
			//qDebug() << country;
			QString symbol = words[i].mid(delim + 1, words[i].length());
			//qDebug() << symbol;
			m->setData(playerName, country);
			m->setData(salaryString, symbol);
		}
	}
	model = m;
	return m;
}