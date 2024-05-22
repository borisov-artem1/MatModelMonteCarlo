#include "wall.h"
#include <string>
#include <memory>
#include "interface.h"
#include <random>
#include <iostream>
#include <cmath>
#define PI 3.14
#define NOT_FOUND 1

extern QVector<Wall*> vector;
QVector<int> indexVector;
double Wall::coordinateZ = 0;
int Wall::indexNumber = 0;
static Generator generator;

Wall::Wall() {
    indexNumber++;
}

Wall::~Wall() {
    // деструктор Wall
}


Disk::Disk(double radiusOutside, double radiusInside): radiusInsideDisk(radiusInside), radiusOutsideDisk(radiusOutside) {
    index = indexNumber;
    name = "Disk";
}

Disk::~Disk() {
    std::cout << "poka";
}


Сylinder::Сylinder(double radiusOutside, double height): radiusOutsideCylinder(radiusOutside), Height(height) {
    index = indexNumber;
    name = "Cylinder";
    coordinateZ += Height;
}

Сylinder::~Сylinder() {}

//берет значения от нуля до высоты нашей модели и находит случайную
//точку по координате z , и именно сечение по этой координате мы будем исследовать
double Generator::GeneratorMonteCarlo_Height()
{
    std::mt19937 generator; // Инициализация генератора
    int z_finish = Wall::coordinateZ;
    generator.seed(std::random_device()()); // Использование случайного устройства для засевания генератора
    std::uniform_int_distribution<int> distribution(0, z_finish); // Равномерное распределение от 1 до 100
    int randomValue = distribution(generator); // Генерация случайного числа
    return randomValue;
}

//далее в сечении из предыдущей функции находим конкретную точку
double Generator::GeneratorMonteCarlo_Fi()
{
    std::mt19937 generator;
    generator.seed(std::random_device()()); // Использование случайного устройства для засевания генератора
    std::uniform_int_distribution<int> distribution(0, 360);
    int randomValue = distribution(generator);
    return randomValue;
}

//это один из 2 углов, который будет необходим для построения луча, по которому молекула будет вылетать
double Generator::GeneratorMonteCarlo_Teta()
{
    std::mt19937 generator;
    generator.seed(std::random_device()());
    std::uniform_int_distribution<int> distribution(0, 180); // Равномерное распределение от 1 до 180
    int fi = distribution(generator);
    return fi;
}

//это 2 угол, который будет необходим для построения луча, по которому молекула будет вылетать
double Generator::GeneratorMonteCarlo_Gamma()
{
    std::mt19937 generator;
    generator.seed(std::random_device()());
    std::uniform_int_distribution<int> distribution(0, 180); // Равномерное распределение от 1 до 180
    int fi = distribution(generator);
    return fi;
}

RandomValues Generator::GeneratorMonteCarlo_Cylinder()
{
    RandomValues cylinderValues;
    cylinderValues.height = GeneratorMonteCarlo_Height();
    cylinderValues.fi = GeneratorMonteCarlo_Fi();
    cylinderValues.teta = GeneratorMonteCarlo_Teta();
    cylinderValues.gamma = GeneratorMonteCarlo_Gamma();
    return cylinderValues;
}

void Generator::LookDiskIndexes()
{
    for (Wall* wall : vector) {
            Disk* disk = dynamic_cast<Disk*>(wall); // Попытка приведения типа
            if (disk && disk->name == "Disk") {
                indexVector.push_back(disk->index);
            }
        }
}

int Generator::GeneratorMonteCarlo_index()
{    // Проверка на пустоту вектора
    if (indexVector.empty()) {
        throw std::runtime_error("Vector is empty");
    }
    LookDiskIndexes();

    // Статические объекты для генерации случайных чисел
    static std::mt19937 gen(time(nullptr)); // генератор случайных чисел, инициализированный системными часами
    std::uniform_int_distribution<size_t> dist(0, indexVector.size() - 1); // распределение для индексов

    // Выбор случайного индекса и возвращение элемента
    return indexVector[dist(gen)];
}

double Generator::GeneratorMonteCarlo_Point(int index)
{
    Wall* topWall = vector[index];
    Disk* disk = dynamic_cast<Disk*>(topWall);
    int firstPoint = disk->radiusInsideDisk;
    int twicePoint = disk->radiusOutsideDisk;
    // Создаем генератор случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(firstPoint, twicePoint);

    // Генерируем и возвращаем случайное число из заданного диапазона
    return dist(gen);
}

RandomValues Generator::GeneratorMonteCarlo_Disk()
{
    RandomValues diskValues;
    diskValues.index = GeneratorMonteCarlo_index();
    diskValues.fi = GeneratorMonteCarlo_Fi();
    diskValues.point = GeneratorMonteCarlo_Point(diskValues.index);
    diskValues.teta = GeneratorMonteCarlo_Teta();
    diskValues.gamma = GeneratorMonteCarlo_Gamma();
    return diskValues;
}

double Generator::CylindersArea()
{
    double area = 0;
    for (Wall* wall : vector) {
            Сylinder* cylinder = dynamic_cast<Сylinder*>(wall); // Попытка приведения типа
            if (cylinder && cylinder->name == "Сylinder") {
                double height = cylinder->Height;
                double radius = cylinder->radiusOutsideCylinder;
                area += height * 2 * PI * radius;
            }
        }
    return area;
}

double Generator::DiskArea()
{
    double area = 0;
    for (Wall* wall : vector) {
            Disk* disk = dynamic_cast<Disk*>(wall); // Попытка приведения типа
            if (disk && disk->name == "Disk") {
                double radiusInside = disk->radiusInsideDisk;
                double radiusOutside = disk->radiusOutsideDisk;
                area += 2 * PI * (pow(radiusOutside, 2) - pow(radiusInside, 2));
            }
        }
    return area;
}

Coeficients Generator::Distribution()
{
    Coeficients coeficitions;
    double areaCylinders = generator.CylindersArea();
    double areaDisk = generator.DiskArea();
    coeficitions.CylinderCoef = areaCylinders/(areaCylinders + areaDisk);
    coeficitions.DiskCoef = 1 - coeficitions.CylinderCoef;
    return coeficitions;
}


findingCylinder Generator::FindCylinderIndex(double height) {
    findingCylinder coord;
    coord.diff = 0.;
    coord.index = 0;
    for (Wall* wall: vector) {
        Сylinder* cylinder = dynamic_cast<Сylinder*>(wall);
        if (cylinder != nullptr && cylinder->name == "Cylinder") {
            if (height < cylinder->coordinateZ) {
                coord.diff = cylinder->Height - (cylinder->coordinateZ - height);
                break;
            }
        }
        ++(coord.index);
    }
    return coord;
}

Coordinates& Coordinates::operator=(const RandomValues& other) {
    findingCylinder coord = generator.FindCylinderIndex(other.height);
    double p1 = sin((other.teta * PI) / 180) * cos((other.gamma * PI) / 180);
    double p2 = sin((other.teta * PI) / 180) * sin((other.gamma * PI) / 180);
    double p3 = cos((other.teta * PI) / 180);
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    if (other.height != 0.) {
        Сylinder* cylinder = dynamic_cast<Сylinder*>(vector[coord.index]);
        double x0 = cylinder->radiusOutsideCylinder * cos((other.fi * PI) / 180);
        double y0 = cylinder->radiusOutsideCylinder * sin((other.fi * PI) / 180);
        double z0 = other.height;
        this->x = x0;
        this->y = y0;
        this->z = z0;
    } else {
        this->x = other.point * cos(other.fi);
        this->y = other.point * sin(other.fi);
        this->z = vector[other.index - 1]->coordinateZ;
    }
    return *this;
}

int Generator::Core(int countMoleculs, int iteration)
{
    RandomValues rand;
    int exitMolecules = 0;
    Coeficients coeficionts = {};
    Coordinates NewCoordinates = {};
    coeficionts = generator.Distribution();
    coeficionts.DiskCoef = generator.Distribution().DiskCoef;
    coeficionts.CylinderCoef = generator.Distribution().CylinderCoef;

    for (int i = 0; i <= countMoleculs * coeficionts.CylinderCoef; ++i) {
        rand = generator.GeneratorMonteCarlo_Cylinder(); //написать перегрузку для функции
        NewCoordinates = rand;
        int j = 0;
        while (j < iteration) {
            FlightMoleculeCylinder(NewCoordinates, i); //то же самое
            if (isMoleculeExit(NewCoordinates)) {
                exitMolecules++;
                break;
            } else {
                j++;
            }
        }
    }

    for (int i = 0; i <= countMoleculs * coeficionts.DiskCoef; ++i) {// мои изменения!!!!!!!!!!!!
        rand = generator.GeneratorMonteCarlo_Disk(); //написать перегрузку для функции
        NewCoordinates = rand;
        int j = 0;
        Disk* disk = dynamic_cast<Disk*>(vector[rand.index]);
        if (disk->location) {
            while (j < iteration) {
                for (int i = rand.index; i < vector.size(); ++i) {
                    if (vector[i]->name == "Disk" && vector[i] != vector[rand.index]) {
                        NewCoordinates = FlightMoleculeDisk(NewCoordinates, i);
                        if (NewCoordinates.flag == NOT_FOUND) {
                            continue;
                        }
                    } else if (vector[i]->name == "Cylinder"){
                        NewCoordinates = FlightMoleculeCylinder(NewCoordinates, i);
                        if (NewCoordinates.flag == NOT_FOUND) {
                            continue;
                        }
                    }
                }
                if (isMoleculeExit(NewCoordinates)) {
                    ++exitMolecules;
                    break;
                } else {
                    ++j;
                }
            }
        } else {
            while (j < iteration) {
                for (int i = rand.index; i > 0; --i) {
                    if (vector[i]->name == "Disk" && vector[i] != vector[rand.index]) {
                        NewCoordinates = FlightMoleculeDisk(NewCoordinates, i);
                        if (NewCoordinates.flag == NOT_FOUND) {
                            continue;
                        }
                    } else if (vector[i]->name == "Cylinder"){
                        NewCoordinates = FlightMoleculeCylinder(NewCoordinates, i);
                        if (NewCoordinates.flag == NOT_FOUND) {
                            continue;
                        }
                    }
                }
            }
        }

        /*while (j < iteration) {
            FlightMoleculeDisk(NewCoordinates, disk); //то же самое
            if (isMoleculeExit(NewCoordinates)) {
                exitMolecules++;
                break;
            } else {
                j++;
            }
        }*/


    }
    return exitMolecules;
}//поменять потом функцию, тут как бы идёт добавление, а нужно сделать функцию, в которую передавать указатели на разные функции.




bool Generator::isMoleculeExit(Coordinates coordinates) {
    return true;
}

Coordinates Generator::FlightMoleculeDisk(Coordinates coordinates, int i) {
    Disk* disk = dynamic_cast<Disk*>(vector[i]);
    if (sqrt(pow(coordinates.x, 2) + pow(coordinates.y, 2)) > disk->radiusInsideDisk &&
        sqrt(pow(coordinates.x, 2) + pow(coordinates.x, 2)) < disk->radiusOutsideDisk) {
        double t1 = (vector[disk->index - 1]->coordinateZ - coordinates.z) / coordinates.p3;
        coordinates.x = coordinates.x + coordinates.p1 * t1;
        coordinates.y = coordinates.y + coordinates.p2 * t1;
        coordinates.z = coordinates.z + coordinates.p3 * t1;
    } else {
        coordinates.flag = NOT_FOUND;
    }
    return coordinates;
}

Coordinates Generator::FlightMoleculeCylinder(Coordinates coordinates, int i) {
    findingCylinder coord = FindCylinderIndex(coordinates.z);
    pointOfIntersection pointBegin;
    pointOfIntersection pointEnd;
    Сylinder* cylinder = dynamic_cast<Сylinder*>(vector[i]);
    double A = pow(coordinates.p1, 2) + pow(coordinates.p2, 2);
    double B = coordinates.x * coordinates.p1 + coordinates.y * coordinates.p2;
    double C = pow(coordinates.x, 2) + pow(coordinates.y, 2) - pow(cylinder->radiusOutsideCylinder, 2);
    double Dis = pow(B, 2) - 4 * A * C;
    if (Dis > 0) {
        double t1 = (-B + pow(Dis, 0.5)) / (2 * A);
        double t2 = (-B - pow(Dis, 0.5)) / (2 * A);
        pointBegin.x = coordinates.x + coordinates.p1 * t1;
        pointBegin.y = coordinates.y + coordinates.p2 * t1;
        pointBegin.z = coordinates.z + coordinates.p3 * t1;
        pointEnd.x = coordinates.x + coordinates.p1 * t2;
        pointEnd.y = coordinates.y + coordinates.p2 * t2;
        pointEnd.z = coordinates.z + coordinates.p3 * t2;
        if (pointBegin.x != coordinates.x && pointBegin.y != coordinates.y && pointBegin.z != coordinates.z) {
            coordinates.x = pointBegin.x;
            coordinates.y = pointBegin.y;
            coordinates.z = pointBegin.z;
        } else {
            coordinates.x = pointEnd.x;
            coordinates.y = pointEnd.y;
            coordinates.z = pointEnd.z;
        }
        if (pointEnd.z > vector[coord.index]->coordinateZ ||
            pointEnd.z < (cylinder->coordinateZ - cylinder->Height)) {
            coordinates.flag = NOT_FOUND;
        }
        return coordinates;

    } else {
        throw std::exception();
    }
}

Coordinates Generator::FlightMolecule(Coordinates& NewCoordinates) {
    for (int i = 0; i < vector.size(); i++) {
        if (vector[i]->name == "Disk") {
            NewCoordinates = FlightMoleculeDisk(NewCoordinates, i);
            if (NewCoordinates.flag == NOT_FOUND) {
                continue;
            } else {
                return NewCoordinates;
            }
        } else {
            NewCoordinates = FlightMoleculeCylinder(NewCoordinates, i);
            if (NewCoordinates.flag == NOT_FOUND) {
                continue;
            } else {
                return NewCoordinates;
            }
        }
    } 
}

/*
std::vector<pointOfIntersection> Generator::IntersectionSearch(Wall* wall) {

}

*/

/*bool Generator::FlightMolecule_Cylinder(pointOfIntersection& pointBegin, pointOfIntersection& pointEnd) {

    RandomValues initialСoordinates;
    pointOfIntersection tmp_point;
    initialСoordinates = generator.GeneratorMonteCarlo_Cylinder();
    findingCylinder coord = FindCylinderIndex(initialСoordinates.height);
    Сylinder* cylinder = dynamic_cast<Сylinder*>(vector[coord.index]);
    double p1 = sin((initialСoordinates.teta * PI) / 180) * cos((initialСoordinates.gamma * PI) / 180);
    double p2 = sin((initialСoordinates.teta * PI) / 180) * sin((initialСoordinates.gamma * PI) / 180);
    double p3 = cos((initialСoordinates.teta * PI) / 180);
    double x0 = cylinder->radiusOutsideCylinder * cos((initialСoordinates.fi * PI) / 180);
    double y0 = cylinder->radiusOutsideCylinder * sin((initialСoordinates.fi * PI) / 180);
    double z0 = initialСoordinates.height;
    double A = pow(p1, 2) + pow(p2, 2);
    double B = x0 * p1 + y0 * p2;
    double C = pow(x0, 2) + pow(y0, 2) - pow(cylinder->radiusOutsideCylinder, 2);
    double Dis = pow(B, 2) - 4 * A * C;
    double t1 = 0.;
    double t2 = 0.;
    if (Dis > 0) {
        t1 = (-B + pow(Dis, 0.5)) / (2 * A);
        t2 = (-B - pow(Dis, 0.5)) / (2 * A);
        pointBegin.x = x0 + p1 * t1;
        pointBegin.y = y0 + p2 * t1;
        pointBegin.z = z0 + p3 * t1;
        pointEnd.x = x0 + p1 * t2;
        pointEnd.y = y0 + p2 * t2;
        pointEnd.z = z0 + p3 * t2;
        if (pointBegin.x != x0 && pointBegin.y != y0 && pointBegin.z != z0) {
            tmp_point = pointBegin;
            pointBegin = pointEnd;
            pointEnd = tmp_point;
        }
        if (pointEnd.z > vector[coord.index]->coordinateZ ||
            pointEnd.z < (cylinder->coordinateZ - cylinder->Height)) { // если это так, то необходимо найти другую фигуру, с которой пересечется луч
            bool breakCondition = false;
            for (int i = 0; i < vector.size(); ++i) {
                if (vector[i]->name == "Cylinder" && vector[i] != cylinder) { // это неправильно потому что мы ищем на
                        C = pow(x0, 2) + pow(y0, 2) - pow(cylinder->radiusOutsideCylinder, 2);  // бесконечных цилиндрах
                        Dis = pow(B, 2) - 4 * A * C;
                        t1 = (-B + pow(Dis, 0.5)) / (2 * A);
                        t2 = (-B - pow(Dis, 0.5)) / (2 * A);
                        if (z0 + p3 * t1 > z0 + p3 * t2) { // тоже ошибка
                            pointEnd.x = x0 + p1 * t1;
                            pointEnd.y = y0 + p2 * t1;
                            pointEnd.z = z0 + p3 * t1;
                            breakCondition = true;
                            break;
                        } else {
                            pointEnd.x = x0 + p1 * t2;
                            pointEnd.y = y0 + p2 * t2;
                            pointEnd.z = z0 + p3 * t2;
                            breakCondition = true;
                            break;
                        }
                } else if (vector[i]->name == "Disk") {
                    Disk* disk = dynamic_cast<Disk*>(vector[i]);
                    if (sqrt(pow(x0, 2) + pow(y0, 2)) > disk->radiusInsideDisk &&
                        sqrt(pow(x0, 2) + pow(y0, 2)) < disk->radiusOutsideDisk) {
                        t1 = (vector[i - 1]->coordinateZ - z0) / p3;
                        pointEnd.x = x0 + p1 * t1;
                        pointEnd.y = y0 + p2 * t1;
                        pointEnd.z = z0 + p3 * t1;
                        breakCondition = true;
                        break;
                    }
                }
            }
            if (breakCondition) {
                return true;
            }
        }
    } else if (Dis == 0) {
        throw std::exception();
    }
    return true;
}

bool Generator::FlightMolecule_Disk() {
    RandomValues initialСoordinates;
    initialСoordinates = generator.GeneratorMonteCarlo_Disk();
    double p1 = sin((initialСoordinates.teta * PI) / 180) * cos((initialСoordinates.gamma * PI) / 180);
    double p2 = sin((initialСoordinates.teta * PI) / 180) * sin((initialСoordinates.gamma * PI) / 180);
    double p3 = cos((initialСoordinates.teta * PI) / 180);
    // на основании имеющихся строим луч в трехмерном пространстве
}

bool Generator::isMoleculeExit(Coordinates coordinates)
{

}
*/

