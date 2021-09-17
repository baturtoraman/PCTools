#include "WarningClass.h"

//Warning Class Implementation
WarningClass::WarningClass(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(430,150);
	connect(ui.warningOKButton, SIGNAL(clicked()), this, SLOT(warningOKButtonClicked()));
}

WarningClass::~WarningClass()
{
}

void WarningClass::setText(QString warningMessage)
{
	ui.textEdit->setAlignment(Qt::AlignCenter);
	ui.textEdit->setText(warningMessage);
}

void WarningClass::setDefaultWarningMessage()
{	
	ui.textEdit->setAlignment(Qt::AlignCenter);
	ui.textEdit->setText("Please Make Sure That You Entered All The Necessary Information!!");
}

void WarningClass::warningOKButtonClicked() {
	this->close();
}