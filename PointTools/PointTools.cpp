#include "PointTools.h"
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <string>
#include <QtCore>
#include <QtSql>
#include <qfiledialog.h>
#include <opencv2/imgcodecs.hpp>


namespace fs = boost::filesystem;

PointTools::PointTools(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
        
    //Button connections
    connect(ui.geoReferencingButton, SIGNAL(clicked()), this, SLOT(georeferencingButtonClicked()));
    connect(ui.georeferencingStartButton, SIGNAL(clicked()), this, SLOT(georeferencingStartButtonClicked()));
    connect(ui.georeferencingStopButton, SIGNAL(clicked()), this, SLOT(georeferencingStopButtonClicked()));

    connect(ui.depthButton, SIGNAL(clicked()), this, SLOT(depthButtonClicked()));
    connect(ui.depthStartButton, SIGNAL(clicked()), this, SLOT(depthStartButtonClicked()));
    connect(ui.depthStopButton, SIGNAL(clicked()), this, SLOT(depthStopButtonClicked()));
    connect(ui.depthPcReadButton, SIGNAL(clicked()), this, SLOT(depthPcReadButtonClicked()));
    connect(ui.depthCoorReadButton, SIGNAL(clicked()), this, SLOT(depthCoorReadButtonClicked()));

    connect(ui.colorizationButton, SIGNAL(clicked()), this, SLOT(colorizationButtonClicked()));
    connect(ui.colorizationStartButton, SIGNAL(clicked()), this, SLOT(colorizationStartButtonClicked()));
    connect(ui.colorizationStopButton, SIGNAL(clicked()), this, SLOT(colorizationStopButtonClicked()));
    connect(ui.colorizationLocalPcButton, SIGNAL(clicked()), this, SLOT(colorizationPcReadButtonClicked()));
    connect(ui.colorizationLocalCoorButton, SIGNAL(clicked()), this, SLOT(colorizationCoorReadButtonClicked()));
    connect(ui.colorizationLocalImgButton, SIGNAL(clicked()), this, SLOT(colorizationImageReadButtonClicked()));

    connect(ui.dbUploaderButton, SIGNAL(clicked()), this, SLOT(dbUploaderButtonClicked()));
    connect(ui.dbUploaderStartButton, SIGNAL(clicked()), this, SLOT(dbUploaderStartButtonClicked()));
    connect(ui.dbUploaderStopButton, SIGNAL(clicked()), this, SLOT(dbUploaderStopButtonClicked()));
        
}


//Config Data Should Be in x(double),y(double),z(double),fileName(related Point Cloud File),360 image Name---->x,y,z,fileName,360 Image Name----->Config File Extension Must Be ".txt"
//1 Config File For All Point Cloud Files in A Single .txt File
std::vector<std::vector<std::string>> readDepthConfigFile(std::string path)
{
    std::vector<std::vector<std::string>> data;
    std::vector<std::string> temp_data;

    std::ifstream myreadfile;
    myreadfile.open(path);
    std::string strs;

    if (myreadfile)
    {
        std::ostringstream sss;
        sss << myreadfile.rdbuf(); // reading data
        strs = sss.str();
    }
    int j = 0, k = 0, count = 0;
    std::string x, y, z, pcFileName, image360name;
    std::string tmp;

    for (int i = 0; i < strs.length(); i++) {

        if (strs.at(i) == ',') {

            if (count % 4 == 0) {
                x = strs.substr(k, i - k);
                temp_data.push_back(x);
                k = i + 1;

            }
            if (count % 4 == 1) {
                y = strs.substr(k, i - k);
                temp_data.push_back(y);
                k = i + 1;
            }
            if (count % 4 == 2) {
                z = strs.substr(k, i - k);
                temp_data.push_back(z);
                k = i + 1;
            }
            count++;
        }

        else if (strs.at(i) == '\n') {
            count++;
            pcFileName = strs.substr(k, i - k);
            temp_data.push_back(pcFileName);
            data.push_back(temp_data);
            temp_data.clear();
            k = i + 1;
        }
    }
    return data;
}

std::vector<std::vector<std::string>> readConfigFile(std::string path)
{
    std::vector<std::vector<std::string>> data;
    std::vector<std::string> temp_data;

    std::ifstream myreadfile;
    myreadfile.open(path);
    std::string strs;

    if (myreadfile)
    {
        std::ostringstream sss;
        sss << myreadfile.rdbuf(); // reading data
        strs = sss.str();
    }
    int j = 0, k = 0, count = 0;
    std::string x , y, z , pcFileName, image360name;
    std::string tmp;

    for (int i = 0; i < strs.length(); i++) {

        if (strs.at(i) == ',') {

            if (count % 5 == 0) {
                x = strs.substr(k, i - k);
                temp_data.push_back(x);
                k = i + 1;

            }
            if (count % 5 == 1) {
                y = strs.substr(k, i - k);
                temp_data.push_back(y);
                k = i + 1;
            }
            if (count % 5 == 2) {
                z = strs.substr(k, i - k);
                temp_data.push_back(z);
                k = i + 1;
            }
            if (count % 5 == 3) {
                image360name = strs.substr(k, i - k);
                temp_data.push_back(image360name);
                k = i + 1;
            }
            count++;
        }

        else if (strs.at(i) == '\n') {
            count++;
            pcFileName = strs.substr(k, i - k);
            temp_data.push_back(pcFileName);
            data.push_back(temp_data);
            temp_data.clear();
            k = i + 1;
        }
    }
    return data;
}

//Read Files From The System By Checking the Extension(.txt)
std::vector<fs::path> getAllFiles(fs::path const& root, std::string const& ext)
{
    std::vector<fs::path> pathList;

    if (fs::exists(root) && fs::is_directory(root))
    {
        for (auto const& entry : fs::recursive_directory_iterator(root))
        {
            if (fs::is_regular_file(entry) && entry.path().extension() == ext)
                pathList.emplace_back(entry.path().filename());
        }
    }

    else {
        throw std::runtime_error("Empty Path");
    }

    return pathList;
}

void PointTools::georeferencingButtonClicked()
{
    ui.projectPage->setCurrentIndex(0);
}

void PointTools::georeferencingStartButtonClicked()
{
    //Disable All The Buttons
    ui.georeferencingExportType->setEnabled(false);
    ui.geoReferencingButton->setEnabled(false);
    ui.depthButton->setEnabled(false);
    ui.colorizationButton->setEnabled(false);
    ui.dbUploaderButton->setEnabled(false);
    ui.georeferencingCoorFileSelection->setEnabled(false);
    ui.georeferencingInfoBox->setEnabled(false);
    ui.georeferencingLazerType->setEnabled(false);
    ui.georeferencingStartButton->setEnabled(false);
    ui.georeferencingLidarFileSelection->setEnabled(false);
    ui.georeferencingRollPitchYaw->setEnabled(false);
    ui.filterGroup->setEnabled(false);
    ui.georeferencingLidarFileLW->setEnabled(false);
    ui.georeferencingLaserTypeCB->setEnabled(false);
    ui.georeferencingLidarfileButton->setEnabled(false);
    ui.georeferencingInputCoorFileSelected->setEnabled(false);
    ui.georeferencingOutputCoorFileSelected->setEnabled(false);
    ui.georeferencingCoorOutputButton->setEnabled(false);
    ui.georeferencingCoorInputButton->setEnabled(false);
}

void PointTools::georeferencingStopButtonClicked()
{
    this->isStopped = true;

    //Enable All the Buttons
    ui.georeferencingExportType->setEnabled(true);
    ui.geoReferencingButton->setEnabled(true);
    ui.depthButton->setEnabled(true);
    ui.colorizationButton->setEnabled(true);
    ui.dbUploaderButton->setEnabled(true);
    ui.georeferencingCoorFileSelection->setEnabled(true);
    ui.georeferencingInfoBox->setEnabled(true);
    ui.georeferencingLazerType->setEnabled(true);
    ui.georeferencingStartButton->setEnabled(true);
    ui.georeferencingLidarFileSelection->setEnabled(true);
    ui.georeferencingRollPitchYaw->setEnabled(true);
    ui.filterGroup->setEnabled(true);
    ui.georeferencingLidarFileLW->setEnabled(true);
    ui.georeferencingLaserTypeCB->setEnabled(true);
    ui.georeferencingLidarfileButton->setEnabled(true);
    ui.georeferencingInputCoorFileSelected->setEnabled(true);
    ui.georeferencingOutputCoorFileSelected->setEnabled(true);
    ui.georeferencingCoorOutputButton->setEnabled(true);
    ui.georeferencingCoorInputButton->setEnabled(true);

}

void PointTools::depthButtonClicked() {
    //Setting Depth Mapping Page
    ui.projectPage->setCurrentIndex(1);

}

void PointTools::depthStartButtonClicked() {

    //--------------------------PROCESS ALGORITHMS WILL BE ADDED HERE-----------------------------------
    // -------------------------------------------------------------------------------------------------
    // -------------------------------------------------------------------------------------------------
    //Selecting Read From Local on Depth Mapping Page
    if (ui.depthTB->currentIndex() == 0) {
        QString plainTextInfo = "";
        isStopped = false;
        if (!ui.depthCoorFilePath->text().isEmpty() && !ui.depthPcFilePath->text().isEmpty())
        {
            ////Disable All The Buttons
            //ui.geoReferencingButton->setEnabled(false);
            //ui.depthButton->setEnabled(false);
            //ui.colorizationButton->setEnabled(false);
            //ui.dbUploaderButton->setEnabled(false);
            //ui.depthStartButton->setEnabled(false);
            //ui.filterGroup->setEnabled(false);
            //ui.depthTB->setEnabled(false);
            //ui.depthInfoBox->setEnabled(false);
            //////////////////////////

            double carPosX = 0, carPosY = 0, carPosZ = 0;

            QString pcFilePath = ui.depthPcFilePath->text();
            QString coordinateFilePath = ui.depthCoorFilePath->text();

            fs::path path_pcFile(pcFilePath.toStdString());

            fs::path path_coordinateFile(coordinateFilePath.toStdString());
            std::vector<fs::path> path_pcFileList;

            if (fs::exists(path_coordinateFile) && fs::exists(path_pcFile)) {

                path_pcFileList = getAllFiles(path_pcFile, ".txt");

            }

            else {
                WarningClass* warning = new WarningClass();
                QString warningMessage = "No File or Directory Available ";
                warning->setWindowTitle("Warning!!");
                warning->setText(warningMessage);
                warning->show();
                warning->exec();

                delete warning;
                warning = NULL;

                return;
            }

            //Create Black 1024x2048 Image
            cv::Mat imgb(1024, 2048, CV_8UC3, cv::Scalar(0, 0, 0));
            DepthMappingSolution* solution = new DepthMappingSolution();



            for (int i = 0; i < path_pcFileList.size(); i++) {


                std::string depthImageName = path_pcFileList[i].string();
                depthImageName = depthImageName.substr(0, depthImageName.length() - 4);//Delete .txt at the to create depth image name with the the file name
                depthImageName = depthImageName.append(".png");//Creating Depth Image Name

                InterfaceDepthImage* depthImage = new DepthImage(imgb, depthImageName);
                InterfacePcData* pcData = new PcData();

                QApplication::processEvents();
                if (isStopped) {
                    ui.depthInfoBox->document()->setPlainText(plainTextInfo+"Process Stopped\n");
                    break;
                }

                if (fs::exists(pcFilePath.toStdString()) && fs::is_directory(pcFilePath.toStdString())) {

                    std::vector<std::vector<string>> configList = readDepthConfigFile(coordinateFilePath.toStdString());

                    std::string temp_pcFilePath = pcFilePath.toStdString().append("/").append(path_pcFileList[i].string());

                    bool isFileFoundOnConfigList = false;

                    for (int j = 0; j < configList.size(); j++) {
                        if (configList[j][3] == path_pcFileList[i].string()) {

                            carPosX = std::stod(configList[j][0]);
                            carPosY = std::stod(configList[j][1]);
                            carPosZ = std::stod(configList[j][2]);

                            isFileFoundOnConfigList = true;
                            break;
                        }
                    }

                    if (!isFileFoundOnConfigList) {
                        break;
                    }

                    pcData->readPoints(temp_pcFilePath);
                    pcData->setCarPos(carPosX, carPosY, carPosZ);

                    solution->Init(pcData, depthImage);
                    solution->Run();
                    solution->Exit();

                    
                }
                else {

                    WarningClass* warning = new WarningClass();
                    QString warningMessage = "No File or Directory Available ";
                    warning->setWindowTitle("Warning!!");
                    warning->setText(warningMessage);
                    warning->show();
                    warning->exec();

                    delete warning;
                    warning = NULL;
                }
                
                plainTextInfo += QString::fromStdString(depthImageName + " was written\n");
                ui.depthInfoBox->document()->setPlainText(plainTextInfo);
            }
        }
        else {
            //Warning Screen Display
            WarningClass* warning = new WarningClass();
            warning->setWindowTitle("Warning!!");
            warning->setDefaultWarningMessage();
            warning->show();
            warning->exec();

            delete warning;
            warning = NULL;

        }

        plainTextInfo += "Finished...";
        ui.depthInfoBox->document()->setPlainText(plainTextInfo);
        isStopped = false;
    }

    //Selecting read From Database Section on Depth Mapping Page
    else if (ui.depthTB->currentIndex() == 1) {

        if (!ui.depthPanoGPSdbName->text().isEmpty() && !ui.depthPanoGPSHost->text().isEmpty() && !ui.depthPanoGPSPassword->text().isEmpty() && !ui.depthPanoGPSPort->text().isEmpty() && 
            !ui.depthPanoGPSUserName->text().isEmpty() && !ui.depthDBdbName->text().isEmpty() && !ui.depthDBHost->text().isEmpty() && !ui.depthDBPassword->text().isEmpty() && 
            !ui.depthDBPort->text().isEmpty() && !ui.depthDBuserName->text().isEmpty()) 
        {
                
            QString lon, lat;

            QString panoGPShost = ui.depthPanoGPSHost->text();
            QString panoGPSdbName = ui.depthPanoGPSdbName->text();
            QString panoGPSuserName = ui.depthPanoGPSUserName->text();
            QString panoGPSpassword = ui.depthPanoGPSPassword->text();
            QString panoGPSport = ui.depthPanoGPSPort->text();
            QString panoGPSconName = "panoGPS";

            db* panoGPSdb = new db(panoGPShost, panoGPSdbName, panoGPSuserName, panoGPSpassword, panoGPSport.toInt(), panoGPSconName);

            QString depthHost = ui.depthDBHost->text();
            QString depthDBname = ui.depthDBdbName->text();
            QString depthUserName = ui.depthPanoGPSUserName->text();
            QString depthPassword = ui.depthDBPassword->text();
            QString depthPort = ui.depthDBPort->text();
            QString depthConName = "depth";

            db* pcCloudDB = new db(depthHost, depthDBname, depthUserName, depthPassword, depthPort.toInt(), depthConName);

            //--------------------------------
            //!!!!!!!!!!!!Set Car Positions, Image Name, Lon, Lat From DB!!!!!!!!!!!!!-------------------
            //--------------------------------
            QString depthImageName;//////Will be taken from DB
            /// <summary>
            /// //////////////
            /// </summary>
            cv::Mat imgb(1024, 2048, CV_8UC3, cv::Scalar(0, 0, 0));
            DepthMappingSolution* solution = new DepthMappingSolution();
            InterfaceDepthImage* depthImage = new DepthImage(imgb, depthImageName.toStdString());
            InterfacePcData* pcData = new PcData();

            pcCloudDB->getData(lon, lat);
               
            solution->Init(pcData, depthImage);
            solution->Run();
            solution->Exit();
        }

        else {
            //Warning Screen Display
            WarningClass * warning = new WarningClass();
            warning->setWindowTitle("Warning!!");
            warning->setDefaultWarningMessage();
            warning->show();
            warning->exec();

            delete warning;
            warning = NULL;
        }
    }

    else {
        throw std::runtime_error("Tab Not Selected");
    }
    //----------------------------PROCESS ALGORITHMS END HERE--------------------------------
    // --------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------
}

void PointTools::depthStopButtonClicked()
{
    this->isStopped = true;
    ////Enable All the Buttons
    //ui.geoReferencingButton->setEnabled(true);
    //ui.depthButton->setEnabled(true);
    //ui.colorizationButton->setEnabled(true);
    //ui.dbUploaderButton->setEnabled(true);
    //ui.depthStartButton->setEnabled(true);
    //ui.homogenizeCB->setEnabled(true);
    //ui.noiseReductionCB->setEnabled(true);
    //ui.groundClassificationCB->setEnabled(true);
    //ui.depthTB->setEnabled(true);
    //ui.depthInfoBox->setEnabled(true);
}
void PointTools::depthPcReadButtonClicked() {
    QString selectedFileName = QFileDialog::getExistingDirectory(this, "Select a directory...", QDir::homePath());
    ui.depthPcFilePath->setText(selectedFileName);
}

void PointTools::depthCoorReadButtonClicked() {
    QString selectedFileName = QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath());
    ui.depthCoorFilePath->setText(selectedFileName);
}
void PointTools::colorizationButtonClicked()
{
    ui.projectPage->setCurrentIndex(2);
}

void PointTools::colorizationStartButtonClicked()
{

    if (!ui.colorizationLocalCoorFilePath->text().isEmpty() && !ui.colorizationLocalImgFilePath->text().isEmpty() && !ui.colorizationLocalPcFilePath->text().isEmpty()) {

        //Disable All The Buttons
   /*     ui.geoReferencingButton->setEnabled(false);
        ui.depthButton->setEnabled(false);
        ui.colorizationButton->setEnabled(false);
        ui.dbUploaderButton->setEnabled(false);
        ui.depthStartButton->setEnabled(false);
        ui.filterGroup->setEnabled(false);
        ui.colorizationTB->setEnabled(false);
        ui.colorizationTE->setEnabled(false);
        ui.colorizationStartButton->setEnabled(false);*/

        if (ui.colorizationTB->currentIndex() == 0) {

            if (!ui.colorizationLocalPcFilePath->text().isEmpty() && !ui.colorizationLocalCoorFilePath->text().isEmpty() && !ui.colorizationLocalImgFilePath->text().isEmpty()) {

                QString plainTextInfo = "";

                QString pcFilePath = ui.colorizationLocalPcFilePath->text();
                QString coordinateFilePath = ui.colorizationLocalCoorFilePath->text();
                QString imageFilePath = ui.colorizationLocalImgFilePath->text();

                
                double carPosX = 0, carPosY = 0, carPosZ = 0;

                fs::path path_pcFile(pcFilePath.toStdString());
                fs::path path_coordinateFile(coordinateFilePath.toStdString());
                fs::path path_imageFile(imageFilePath.toStdString());

                std::vector<fs::path> path_pcFileList;
                std::vector<fs::path> path_imageFileList;
                std::string temp_imageFilePath;
                if (fs::exists(path_coordinateFile) && fs::exists(path_pcFile)) {

                    path_pcFileList = getAllFiles(path_pcFile, ".txt");
                }

                else {
                    WarningClass* warning = new WarningClass();
                    QString warningMessage = "Selected Path For Point Cloud File Is Not Available";
                    warning->setWindowTitle("Warning!!");
                    warning->setText(warningMessage);
                    warning->show();
                    warning->exec();

                    delete warning;
                    warning = NULL;
                }

                if (fs::exists(path_coordinateFile) && fs::exists(path_imageFile)) {

                    path_imageFileList = getAllFiles(path_imageFile, ".jpeg");
                }
                else {
                    WarningClass* warning = new WarningClass();
                    QString warningMessage = "Selected Path For Image Path Is Not Available";
                    warning->setWindowTitle("Warning!!");
                    warning->setText(warningMessage);
                    warning->show();
                    warning->exec();

                    delete warning;
                    warning = NULL;

                    return;
                }

                for (int i = 0; i < path_pcFileList.size(); i++) {

                    std::string colorizedPcName ="colorized"+ path_pcFileList[i].string();
                    colorizedPcName = colorizedPcName.substr(0, colorizedPcName.length() - 4);//Delete .txt at the to create colorized pc name with the the file name
                    colorizedPcName = colorizedPcName.append(".txt");//Creating Colorized PC Name
                    
                    QApplication::processEvents();
                    if (isStopped) {
                        ui.colorizationTE->document()->setPlainText("After " + QString::fromStdString(colorizedPcName) + " process stopped.");
                        break;
                    }

                    if (fs::exists(pcFilePath.toStdString()) && fs::is_directory(pcFilePath.toStdString())) {

                        std::vector<std::vector<string>> configList = readConfigFile(coordinateFilePath.toStdString());

                        std::string temp_pcFilePath = pcFilePath.toStdString().append("/").append(path_pcFileList[i].string());
                        
                        bool isFileFoundOnConfigList = false;

                        for (int j = 0; j < configList.size(); j++) {
                            if (configList[j][3] == path_pcFileList[i].string()) {

                                carPosX = std::stod(configList[j][0]);
                                carPosY = std::stod(configList[j][1]);
                                carPosZ = std::stod(configList[j][2]);

                                temp_imageFilePath = imageFilePath.toStdString().append("/").append(configList[j][4]);
                                isFileFoundOnConfigList = true;

                                break;
                            }
                        }

                        if (!isFileFoundOnConfigList) {
                            WarningClass* warning = new WarningClass();
                            QString warningMessage = "No related Config File Found For " + QString::fromStdString(path_pcFileList[i].string());
                            warning->setWindowTitle("Warning!!");
                            warning->setText(warningMessage);
                            warning->show();
                            warning->exec();

                            delete warning;
                            warning = NULL;

                            break;
                        }



                        if (fs::exists(temp_imageFilePath) && fs::exists(temp_imageFilePath)) {

                            InterfacePcData* pcData = new PcData();
                            pcData->readPoints(temp_pcFilePath);
                            pcData->setCarPos(carPosX, carPosY, carPosZ);

                            cv::Mat img360 = cv::imread(temp_imageFilePath, cv::IMREAD_COLOR);
                            cv::resize(img360, img360, cv::Size(2048, 1024), cv::INTER_LINEAR);
                            ColoredPcData* coloredPc = new ColoredPcData(img360, 0, 0, 0, temp_pcFilePath, imageFilePath.toStdString(), colorizedPcName);
                            EncolorPcDataSolution* solution = new EncolorPcDataSolution();

                            solution->Init(pcData, coloredPc);
                            solution->Run();
                            solution->Exit();
                        }

                        else {
                            WarningClass* warning = new WarningClass();

                            std::string temp_imageNamePNG = path_pcFileList[i].string();
                            temp_imageNamePNG = temp_imageNamePNG.substr(0, temp_imageNamePNG.length() - 4);
                            temp_imageNamePNG = temp_imageNamePNG.append(".png");

                            QString warningMessage = "Image File Path For " + QString::fromStdString(temp_imageNamePNG) + " Is Not Found";
                            warning->setWindowTitle("Warning!!");
                            warning->setText(warningMessage);
                            warning->show();
                            warning->exec();

                            delete warning;
                            warning = NULL;

                            break;
                        }
                    }

                    else {

                        WarningClass* warning = new WarningClass();
                        QString warningMessage = "No File or Directory Available ";
                        warning->setWindowTitle("Warning!!");
                        warning->setText(warningMessage);
                        warning->show();
                        warning->exec();

                        delete warning;
                        warning = NULL;
                    }
                    plainTextInfo += QString::fromStdString(colorizedPcName + " was written\n");
                    ui.colorizationTE->document()->setPlainText(plainTextInfo);
                    
                    
                }
                plainTextInfo += "Finished...";
                ui.colorizationTE->setPlainText(plainTextInfo);
            }

            else {
                WarningClass* warning = new WarningClass();
                QString warningMessage = "No File or Directory Available ";
                warning->setWindowTitle("Warning!!");
                warning->setText(warningMessage);
                warning->show();
                warning->exec();

                delete warning;
                warning = NULL;

                return;
            }
            isStopped = false;
        }

        //Selecting read From Database Section on Depth Mapping Page
        else if (ui.colorizationTB->currentIndex() == 1) {
            if (!ui.colorizationPanoGPSHost->text().isEmpty() && !ui.colorizationPanoGPSdbName->text().isEmpty() && !ui.colorizationPanoGPSUserName->text().isEmpty() && !ui.colorizationPanoGPSPassword->text().isEmpty()
                && !ui.colorizationPanoGPSPort->text().isEmpty() && ui.colorizationPanoGPSPort->text().isEmpty() && !ui.colorizationConfigHost->text().isEmpty() && !ui.colorizationConfigDbName->text().isEmpty()
                && !ui.colorizationConfigUserName->text().isEmpty() && !ui.colorizationConfigPassword->text().isEmpty() && !ui.colorizationConfigPort->text().isEmpty()) {

                QString panoGPShost = ui.colorizationPanoGPSHost->text();
                QString panoGPSdbName = ui.colorizationPanoGPSdbName->text();
                QString panoGPSuserName = ui.colorizationPanoGPSUserName->text();
                QString panoGPSpassword = ui.colorizationPanoGPSPassword->text();
                QString panoGPSport = ui.colorizationPanoGPSPort->text();
                QString panoGPSconName = "panoGPS";

                db* panoGPSdb = new db(panoGPShost, panoGPSdbName, panoGPSuserName, panoGPSpassword, panoGPSport.toInt(), panoGPSconName);

                QString colorizationPChost = ui.colorizationConfigHost->text();
                QString colorizationPCdbName = ui.colorizationConfigDbName->text();
                QString colorizationPCuserName = ui.colorizationConfigUserName->text();
                QString colorizationPCpassword = ui.colorizationConfigPassword->text();
                QString colorizationPCport = ui.colorizationConfigPort->text();
                QString depthConName = "pcCloud";

                db* pcCloudDB = new db(colorizationPChost, colorizationPCdbName, colorizationPCuserName, colorizationPCpassword, colorizationPCport.toInt(), depthConName);
            }

            else {
                //Warning Screen Display
                WarningClass* warning = new WarningClass();
                warning->setWindowTitle("Warning!!");
                warning->setDefaultWarningMessage();
                warning->show();
                warning->exec();

                delete warning;
                warning = NULL;
            } 
        }

        else {
            //Warning Screen Display
            WarningClass* warning = new WarningClass();
            QString warningMessage = "No tab Selected";
            warning->setWindowTitle("Warning!!");
            warning->setText(warningMessage);
            warning->show();
            warning->exec();

            delete warning;
            warning = NULL;
        }
    }

    else 
    {
        //Warning Screen Display
        WarningClass* warning = new WarningClass();
        warning->setWindowTitle("Warning!!");
        warning->setDefaultWarningMessage();
        warning->show();
        warning->exec();

        delete warning;
        warning = NULL;
    }
   
}

void PointTools::colorizationStopButtonClicked()
{
    this->isStopped = true;

    //Enable All the Buttons
    //ui.geoReferencingButton->setEnabled(true);
    //ui.depthButton->setEnabled(true);
    //ui.colorizationButton->setEnabled(true);
    //ui.dbUploaderButton->setEnabled(true);
    //ui.depthStartButton->setEnabled(true);
    //ui.filterGroup->setEnabled(true);
    //ui.colorizationTB->setEnabled(true);
    //ui.colorizationTE->setEnabled(true);
    //ui.colorizationStartButton->setEnabled(true);

}

void PointTools::colorizationPcReadButtonClicked(){
    QString selectedFileName = QFileDialog::getExistingDirectory(this, "Select a directory...", QDir::homePath());
    ui.colorizationLocalPcFilePath->setText(selectedFileName);
}
void PointTools::colorizationCoorReadButtonClicked(){
    QString selectedFileName = QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath());
    ui.colorizationLocalCoorFilePath->setText(selectedFileName);
}
void PointTools::colorizationImageReadButtonClicked(){
    QString selectedFileName = QFileDialog::getExistingDirectory(this, "Select a file...", QDir::homePath());
    ui.colorizationLocalImgFilePath->setText(selectedFileName);
}

void PointTools::dbUploaderButtonClicked()
{
    ui.projectPage->setCurrentIndex(3);
}

void PointTools::dbUploaderStartButtonClicked()
{
    //Disable All The Buttons
    ui.geoReferencingButton->setEnabled(false);
    ui.depthButton->setEnabled(false);
    ui.colorizationButton->setEnabled(false);
    ui.dbUploaderButton->setEnabled(false);
    ui.depthStartButton->setEnabled(false);
    ui.homogenizeCB->setEnabled(false);
    ui.noiseReductionCB->setEnabled(false);
    ui.groundClassificationCB->setEnabled(false);
    ui.depthTB->setEnabled(false);
    ui.depthInfoBox->setEnabled(false);
    ui.dbUploaderGB->setEnabled(false);
    ui.dbUploaderStartButton->setEnabled(false);
    ui.dbUploaderPcFileButton->setEnabled(false);
    ui.dbUploaderLidarFile->setEnabled(false);
    ui.dbUploaderBlog->setEnabled(false);
}

void PointTools::dbUploaderStopButtonClicked()
{
    this->isStopped = true;

    //Enable All the Buttons
    ui.geoReferencingButton->setEnabled(true);
    ui.depthButton->setEnabled(true);
    ui.colorizationButton->setEnabled(true);
    ui.dbUploaderButton->setEnabled(true);
    ui.depthStartButton->setEnabled(true);
    ui.homogenizeCB->setEnabled(true);
    ui.noiseReductionCB->setEnabled(true);
    ui.groundClassificationCB->setEnabled(true);
    ui.depthTB->setEnabled(true);
    ui.depthInfoBox->setEnabled(true);
    ui.dbUploaderGB->setEnabled(true);
    ui.dbUploaderStartButton->setEnabled(true);
    ui.dbUploaderPcFileButton->setEnabled(true);
    ui.dbUploaderLidarFile->setEnabled(true);
    ui.dbUploaderBlog->setEnabled(true);
}