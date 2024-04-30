#include "wall.h"
#include <string>
#include <memory>
#include "interface.h"

int Wall::indexNumber = 0;
double Wall::CoordinateZ = 0;

Wall::Wall(double radius) {
    indexNumber++;
}

Wall::~Wall() {
    // деструктор Wall
}

Disk::Disk(double radius1, double radius2) : Wall(radius1) {
    RadiusOutside = radius1;
    RadiusInside = radius2;
    index = indexNumber;
}

Disk::~Disk() {
}

Сylinder::Сylinder(double radius1, double height) : Wall(radius1) {
    RadiusOutside = radius1;
    index = indexNumber;
    Height = height;
    CoordinateZ += Height;
}

Сylinder::~Сylinder() {}

double GeneratorMonteCarlo(int choice, Wall object)
{
    if (choice == 1) {
        int z_first = 0;
        int z_finish = object.CoordinateZ;
        }
    return 0;
}


