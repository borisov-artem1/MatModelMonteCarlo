#include "wall.h"


double Wall::CoordinateZ = 0;
int Wall::indexNumber = 0;

Wall::Wall(double radius) {
    RadiusOutside = radius;
    indexNumber++;
}

Wall::~Wall() {
    // деструктор Wall
}

Dick::Dick(double radius1, double radius2) : Wall(radius1) {
    RadiusInside = radius2;
    index = indexNumber;
}

Dick::~Dick() {
    // деструктор Dick
}

Сylinder::Сylinder(double radius1, double height) : Wall(radius1) {
    RadiusOutside = radius1;
    index = indexNumber;
    Height = height;
}

Сylinder::~Сylinder() {
    // деструктор Dick
}
