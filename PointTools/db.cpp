#include "db.h"
#include<QtSql>
#include <QDebug>
#include <QSqlDatabase>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlError>
#include <QThread>
#include <QSqlError>
#include<windows.h>
#include <qsqldatabase.h>

db::db(QString host, QString dbname, QString username, QString password, int port, QString conName)
{
	db1 = QSqlDatabase::addDatabase("QPSQL", conName);
	db1.setHostName(host);
	db1.setPort(port);
	db1.setDatabaseName(dbname);
	db1.setUserName(username);
	db1.setPassword(password);

}

db::~db() {}


void db::Init(InterfacePcData* pcData)
{
	this->pcData = pcData;
}

QStringList db::getDirName() {

	QString dirName = QString("select dirname from panogps group by dirname");

	QSqlQuery query(db1);
	query.setForwardOnly(true);

	QStringList dirnamess;

	if (!db1.open())
	{
		qInfo() << "Could not open connection!";
		qInfo() << db1.lastError().text();
	}

	else {

		if (!query.exec(dirName))
		{
			qInfo() << db1.lastError().text();
			qInfo() << query.lastError().text();
		}

		else {

			while (query.next())
			{
				dirnamess.append(QString(query.value(0).toString()));
			}
		}
	}
	return dirnamess;
}


QStringList db::getFileName(QString dirNames) {

	dirNames = "'" + dirNames + "'";

	QString getFileName = QString("select filename from  panogps where dirname = %1 group by filename").arg(dirNames);


	QSqlQuery query(db1);
	query.setForwardOnly(true);

	QStringList fileName;

	if (!db1.open())
	{
		qInfo() << "Could not open connection!";
		qInfo() << db1.lastError().text();
	}

	else {

		if (!query.exec(getFileName))
		{
			qInfo() << db1.lastError().text();
			qInfo() << query.lastError().text();
		}

		else {

			while (query.next())
			{
				fileName.append(QString(query.value(0).toString()));
			}
		}
	}
	return fileName;
}


db::panoGPSvar db::getPanoGPS(QString dirNames, QString fileNames) {

	dirNames = "'" + dirNames + "'";
	fileNames = "'" + fileNames + "'";

	QString getPanoPoints = QString("select imgname, coordx, coordy, st_x(st_transform(geom,32636)),st_y(st_transform(geom,32636)), altitude from panogps where dirname = %1 and filename = %2").arg(dirNames, fileNames);

	QSqlQuery query(db1);
	query.setForwardOnly(true);

	db::panoGPSvar panGPS;

	if (!db1.open())
	{
		qInfo() << "Could not open connection!";
		qInfo() << db1.lastError().text();
	}

	else {

		if (!query.exec(getPanoPoints))
		{
			qInfo() << db1.lastError().text();
			qInfo() << query.lastError().text();
		}

		else {

			while (query.next())
			{
				panGPS.imgNameList.append(QString(query.value(0).toString()));
				panGPS.coordXList.append(QString(query.value(1).toString()));
				panGPS.coordYList.append(QString(query.value(2).toString()));
				panGPS.st_XList.append(query.value(3).toDouble());
				panGPS.st_YList.append(query.value(4).toDouble());
				panGPS.altitudeList.append(query.value(5).toDouble());
			}
		}
	}

	return panGPS;
}

void db::getData(QString _lon, QString _lat)
{

	db::getDataVar gdVar;
	QString argument = "' Point(" + _lon + " " + _lat + ")'";

	QString qStr = QString("WITH cte_transform_points AS (\
                WITH cte_patches AS (\
                    SELECT\
                        PC_Explode(pa)::geometry as geo_point\
                    FROM\
                        pc_table\
                    WHERE\
                        PC_Intersects(ST_Transform(ST_Buffer(ST_Transform(ST_GeomFromText(%1,4326), 32636), 15), 4326), pa)\
                )\
                SELECT \
                    ST_Transform(geo_point, 32636) as transform_geo_point\
                FROM\
                    cte_patches\
                WHERE\
                        ST_Intersects(ST_Transform(ST_EXPAND(ST_Transform(ST_GeomFromText(%1,4326), 32636), 15), 4326), geo_point)\
            )\
            SELECT\
            ST_X(transform_geo_point) as x,\
            ST_Y(transform_geo_point) as y,\
            ST_Z(transform_geo_point) as z\
            FROM cte_transform_points").arg(argument);
	qDebug() << qStr;
	QSqlQuery query(db1);
	query.setForwardOnly(true);

	if (!db1.open())
	{
		qInfo() << "Could not open connection!";
		qInfo() << db1.lastError().text();
	}

	else {

		if (!query.exec(qStr))
		{
			qInfo() << db1.lastError().text();
			qInfo() << query.lastError().text();
		}

		else {

			while (query.next())
			{
				/*	gdVar.x_list.push_back(query.value(0).toDouble());
					gdVar.y_list.push_back(query.value(1).toDouble());
					gdVar.z_list.push_back(query.value(2).toDouble());*/

				pcData->pc.push_back(new LidarPoint(query.value(0).toDouble(), query.value(1).toDouble(), query.value(2).toDouble()));

			}
		}
	}
}