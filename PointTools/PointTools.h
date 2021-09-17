#pragma once
#include <iostream>
#include "DepthImage.h"
#include "DepthMappingSolution.h"
#include "PcData.h"
#include "db.h"
#include "EncolorPcDataSolution.h"
#include "WarningClass.h"

#include <QtWidgets/QMainWindow>
#include "ui_PointTools.h"

class PointTools : public QMainWindow
{
    Q_OBJECT

public:
    PointTools(QWidget *parent = Q_NULLPTR);

private:
    Ui::PointToolsClass ui;
    db::panoGPSvar panoGPS;
    db::getDataVar pointCloudDb;

    void disableButtons();
    void enableButtons();

    bool isStopped = false;

private slots:

    void georeferencingButtonClicked();
    void georeferencingStartButtonClicked();
    void georeferencingStopButtonClicked();

    void depthButtonClicked();
    void depthStartButtonClicked();
    void depthStopButtonClicked();
    void depthPcReadButtonClicked();
    void depthCoorReadButtonClicked();


    void colorizationButtonClicked();
    void colorizationStartButtonClicked();
    void colorizationStopButtonClicked();
    void colorizationPcReadButtonClicked();
    void colorizationCoorReadButtonClicked();
    void colorizationImageReadButtonClicked();

    void dbUploaderButtonClicked();
    void dbUploaderStartButtonClicked();
    void dbUploaderStopButtonClicked();
};
