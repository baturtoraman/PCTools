#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMap>
#include <QObject>
#include <QQueue>
#include <QVector>
#include <math.h>
#include <time.h>
#include <QVariant>
#include <qstring.h>

#include "PcData.h"

using namespace std;
#pragma once

class db
{
public:

	struct panoGPSvar {
		QStringList imgNameList;
		QStringList coordXList;
		QStringList coordYList;
		QList<double> st_XList;
		QList<double> st_YList;
		QList<double> altitudeList;
	};

	struct getDataVar {
		vector<double> x_list;
		vector<double> y_list;
		vector<double> z_list;
	};

	db(QString host, QString dbname, QString username, QString password, int port, QString conName);
	~db();

	void Init(InterfacePcData*);
	panoGPSvar getPanoGPS(QString fileName, QString dirName);
	QStringList getFileName(QString dirNames);
	QStringList getDirName();
	void getData(QString _lon, QString _lat);

private:

	QSqlDatabase db1;
	InterfacePcData* pcData = nullptr;

};