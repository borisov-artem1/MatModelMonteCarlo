#include "wall.h"
#include "interface.h"
#include <QDebug>

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

void Interface::testForLocation()
{
    Disk* disk = new Disk(12,0);
    disk->portal=true;
    vector.push_back(disk);
    Сylinder* cylinder = new Сylinder(12,12);
    vector.push_back(cylinder);
    Disk* disk1 = new Disk(12,0);
    disk1 ->portal=true;
    vector.push_back(disk1);
    Coordinates NewC;
    NewC.p1=0.2201;
    NewC.p2=0.2234;
    NewC.p3=-0.949;
    NewC.x=1.98;
    NewC.y=-10.41;
    NewC.z=11.17;
    generator.FlightMoleculeCylinder(NewC, 1);
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

void Interface::testCriostat()
{
    Disk* disk = new Disk(34,0);
    disk->location=true;
    vector.push_back(disk);
    Сylinder* cylinder = new Сylinder(34,305);
    vector.push_back(cylinder);
    Disk* disk1 = new Disk(305, 34);
    disk1->location=true;
    vector.push_back(disk1);
    Сylinder* cylinder1 = new Сylinder(305, 210);
    vector.push_back(cylinder1);
    Disk* disk3 = new Disk(305, 51);
    disk3->location=false;
    vector.push_back(disk3);
    //Disk* disk4 = new Disk(51,0);
    //disk4->portal=true;
    //vector.push_back(disk4);
    qDebug()<< (generator.Core(1000, 2) / 1000.) * 100.;
}

/*void Interface::testIndexes()
{
    Disk* disk1 = new Disk(34,0);
    disk1->location=true;
    vector.push_back(disk1);
    Сylinder* cylinder = new Сylinder(34,305);
    vector.push_back(cylinder);
    Disk* disk2 = new Disk(305, 34);
    disk2->location=true;
    vector.push_back(disk2);
    Сylinder* cylinder1 = new Сylinder(305, 210);
    vector.push_back(cylinder1);
    Disk* disk3 = new Disk(305, 51);
    disk3->location=false;
    vector.push_back(disk3);
    Disk* disk4 = new Disk(51,0);
    disk4->portal=true;
    vector.push_back(disk4);
    qDebug() <<generator.FindDiskCoordZ(2)<< Qt::endl;
}
*/
