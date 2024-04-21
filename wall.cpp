#include "wall.h"


double Wall::CoordinateZ = 0;
int Wall::indexNumber = 0;

Wall::Wall() {
    indexNumber++;
}

Wall::~Wall() {
    // деструктор Wall
}

Disk::Disk(double radius1, double radius2) : Wall() {
    RadiusOutside = radius1;
    RadiusInside = radius2;
    index = indexNumber;
}

Disk::~Disk() {
    // деструктор Dick
}

Сylinder::Сylinder(double radius1, double height) : Wall() {
    RadiusOutside = radius1;
    index = indexNumber;
    Height = height;
    CoordinateZ+=Height;
}

Сylinder::~Сylinder() {
    // деструктор Dick
}
