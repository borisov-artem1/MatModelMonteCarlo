#include "wall.h"
#include "interface.h"

extern QVector<Wall*> vector;
static Generator generator;

void test1()
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
    generator.FlightMoleculeCylinder(NewCoordinates,1);//передаем параметры такие как я указал на карте Miro , просмотри
                                                       // заведомо известно что цилиндр обладает индексом 1, т.к. индексом 0 и 2 обладают
                                                       // автоматически созданные диски с portal == true
}

void testForLocation()
{

}

void test2()
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
    generator.FlightMoleculeCylinder(NewCoordinates,1);//передаем параметры такие как я указал на карте Miro , просмотри
                                                       // заведомо известно что цилиндр обладает индексом 1, т.к. индексом 0 и 2 обладают
                                                       // автоматически созданные диски с portal == true
}

