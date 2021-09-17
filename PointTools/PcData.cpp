#include "PcData.h"

//Reading Pc Data From Local
void PcData::readPoints(std::string path)
{
	std::vector<std::vector<double>> data;
	std::vector<double> temp_data;
	double d_tmp;

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
	double x = 0, y = 0, z = 0;
	std::string tmp;

	for (int i = 0; i < strs.length(); i++) {

		if (strs.at(i) == ',') {

			if (count % 3 == 0) {
				x = std::stod(strs.substr(k, i - k));
				k = i + 1;

			}
			if (count % 3 == 1) {
				y = std::stod(strs.substr(k, i - k));
				k = i + 1;
			}
			count++;
		}

		else if (strs.at(i) == '\n') {
			count++;
			z = std::stod(strs.substr(k, i - k));
			k = i + 1;
			pc.push_back(new LidarPoint(x, y, z));
		}
	}
}

//Compare Distances
inline bool comparator (InterfaceLidarPoint*& point1, InterfaceLidarPoint*& point2)
{
	return (point1->getDistance() > point2->getDistance());
}
//Sorting Points
void PcData::sortPoints()
{
	std::sort(pc.begin(), pc.end(), comparator);
}

//Returns Size of Point Cloud Data
int PcData::getSize()
{
	return pc.size();
}

//Setting Car Position For Each Pc Data
void PcData::setCarPos(double x, double y, double z)
{
	carPos[0] = x;
	carPos[1] = y;
	carPos[2] = z;
}

//Setting Car Position to Origin
void PcData::carPosToOrigin()
{
	for (int i = 0; i < pc.size(); i++)
	{
		pc[i]->setX(pc[i]->getX() - carPos[0]);
		pc[i]->setY(pc[i]->getY() - carPos[1]);
		pc[i]->setZ(pc[i]->getZ() - carPos[2]);
	}
}

//Calculating azimuth and elevation angles and storing
void PcData::computeAngle()
{
	std::vector<double> temp;

	double pitch, yaw, param, distance;

	for (int i = 0; i < pc.size(); i++)
	{
		distance = sqrt(pc[i]->getX() * pc[i]->getX() + pc[i]->getY() * pc[i]->getY() + pc[i]->getZ() * pc[i]->getZ());
		param = pc[i]->getZ() / distance;
		pitch = 90.0 + asin(param) * 180.0 / PI - 1;

		if (pitch < 0.0) {
			pitch += 180.0;
		}

		pitch = fmod(pitch, 180);

		yaw = atan2(pc[i]->getX(), pc[i]->getY()) * 180.0 / PI + 180;

		while (yaw < 0.0) {
			yaw += 360.0;
		}

		yaw = fmod(yaw, 360);

		pc[i]->setImagePix(yaw, pitch, distance);
	}
}

//Calculating Pixel Values of Azimuth and Elevation Angles 
void PcData::angle2Pix()
{
	for (int i = 0; i < pc.size(); i++) {
		pc[i]->setImagePix(pc[i]->getWidth() * 2048 / 360, 1024 - pc[i]->getHeight() * 1024 / 180, pc[i]->getDistance());
	}
}
void PcData::freePC()
{
	for (int i = 0; i < pc.size(); i++) {
		delete pc[i];
		pc[i] = NULL;
	}
	pc.clear();

}
//Rotate Around X Axis
void PcData::rotationMatX(double pitch)
{

	for (int i = 0; i < pc.size(); i++) {

		if (pc.size() <= 0)
			break;
		else
		{
			pc[i]->setY(pc[i]->getY() * cos(pitch * PI / 180) - pc[i]->getZ() * sin(pitch * PI / 180));
			pc[i]->setZ(pc[i]->getY() * sin(pitch * PI / 180) + pc[i]->getZ() * cos(pitch * PI / 180));

		}
	}
}

//Rotate Around Y Axis
void PcData::rotationMatY(double heading)
{
	for (int i = 0; i < pc.size(); i++) {

		if (pc.size() <= 0)
			break;
		else
		{
			pc[i]->setX(pc[i]->getX() * cos(heading * PI / 180) + pc[i]->getZ() * sin(heading * PI / 180));
			pc[i]->setZ(pc[i]->getZ() * cos(heading * PI / 180) - pc[i]->getX() * sin(heading * PI / 180));

		}
	}
}

//Rotate Around Z Axis
void PcData::rotationMatZ(double roll) {

	for (int i = 0; i < pc.size(); i++) {

		if (pc.size() <= 0)
			break;
		else
		{
			pc[i]->setX(pc[i]->getX() * cos(roll / PI) - pc[i]->getY() * sin(roll / PI));
			pc[i]->setY(pc[i]->getX() * sin(roll / PI) + pc[i]->getX() * cos(roll / PI));

		}
	}
}