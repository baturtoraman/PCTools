#pragma once

#include <QDialog>
#include "ui_WarningClass.h"
#include <qstring.h>

class WarningClass : public QDialog
{
	Q_OBJECT

public:
	WarningClass(QWidget *parent = Q_NULLPTR);
	~WarningClass();

	void setText(QString);
	void setDefaultWarningMessage();
private:
	Ui::WarningClass ui;

private slots:
	void warningOKButtonClicked();
};
