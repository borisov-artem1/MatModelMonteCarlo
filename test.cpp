#include "wall.h"
#include "interface.h"

extern QVector<Wall*> vector;
static Generator generator;

void Interface::test() {
    Сylinder* cylinder = new Сylinder(30, 60);
    vector.push_back(cylinder);
    generator.CreatingPortal();
    Coordinates NewCoordinates;
    RandomValues rand = {};
    rand.height = 15;
    rand.fi = 30;
    rand.gamma = 15;
    rand.teta = 15;
    NewCoordinates = rand;
    generator.FlightMoleculeCylinder(NewCoordinates,1);//передаем параметры такие как я указал на карте Miro , просмотри
                                                       // заведомо известно что цилиндр обладает индексом 1, т.к. индексом 0 и 2 обладают
                                                       // автоматически созданные диски с portal == true
    NewCoordinates = generator.FlightMoleculeCylinder(NewCoordinates, 1);
    std::cout << "Направляющий вектор с координатами (p1; p2; p3) = " << "(" << NewCoordinates.p1 << ";" <<
                 NewCoordinates.p2 << ";" << NewCoordinates.p3 << ")" << std::endl;
    std::cout << "Координаты (x; y; z) найденной точки:" << "(" << NewCoordinates.x << ";" << NewCoordinates.y
              << ";" << NewCoordinates.z << ")" << std::endl;

    if (NewCoordinates.flag == FOUND) {
        std::cout << "FOUND";
    } else if (NewCoordinates.flag == NOT_FOUND) {
        std::cout << "NOT_FOUND";

    } else if (NewCoordinates.flag == EXIT) {
        std::cout << "EXIT";
    }
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

