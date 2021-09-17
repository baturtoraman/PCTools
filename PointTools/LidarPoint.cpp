#include "LidarPoint.h"

LidarPoint::LidarPoint(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
{
}

LidarPoint::~LidarPoint()
{
}

/// <summary>
//----->Getters and Setters
/// </summary>

void LidarPoint::setX(double x)
{
    this->x = x;
}

void LidarPoint::setY(double y)
{
    this->y = y;
}

void LidarPoint::setZ(double z)
{
    this->z = z;
}

double LidarPoint::getX()
{
    return x;
}

double LidarPoint::getY()
{
    return y;
}

double LidarPoint::getZ()
{
    return z;
}

void LidarPoint::setRedColor(int r)
{
    this->r = r;
}

void LidarPoint::setBlueColor(int b)
{
    this->b = b;
}

void LidarPoint::setGreenColor(int g)
{
    this->g = g;
}

int LidarPoint::getRedColor()
{
    return r;
}

int LidarPoint::getBlueColor()
{
    return b;
}

int LidarPoint::getGreenColor()
{
    return g;
}

void LidarPoint::setImagePix(double width, double height, double distance) {


    this->width = width;

    this->height = height;

    this->distance = distance;
}

double LidarPoint::getWidth()
{
    return width;
}

double LidarPoint::getHeight()
{
    return height;
}

double LidarPoint::getDistance()
{
    return distance;
}
