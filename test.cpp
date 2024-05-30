#include "wall.h"
#include "interface.h"

extern QVector<Wall*> vector;
static Generator generator;

void test()
{
    Сylinder* cylinder = new Сylinder(30, 60);
    vector.push_back(cylinder);
    generator.CreatingPortal();
    Coordinates NewCoordinates;
    RandomValues rand;
    rand.height = 15;
    rand.fi = 30;
    rand.gamma = 15;
    rand.teta = 15;
    NewCoordinates = rand;
    generator.FlightMoleculeCylinder(NewCoordinates);
}
