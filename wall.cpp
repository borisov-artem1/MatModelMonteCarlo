#include "wall.h"
#include <string>
#include <memory>
#include "interface.h"
#include <random>
#include <iostream>
#include <cmath>
#define PI 3,14

extern QVector<Wall*> vector;
QVector<int> indexVector;
double Wall::coordinateZ = 0;
int Wall::indexNumber = 0;
Generator generator;

Wall::Wall() {
    indexNumber++;
}

Wall::~Wall() {
    // деструктор Wall
}

Disk::Disk(double radiusOutside, double radiusInside): radiusInsideDisk(radiusInside), radiusOutsideDisk(radiusOutside) {
    indexNumber++;
    name = "Disk";
}

Disk::~Disk() {
    std::cout<<"poka";
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
                area+=height*2*PI*radius;
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
    coeficitions.CylinderCoef = areaCylinders/(areaCylinders+areaDisk);
    coeficitions.DiskCoef = 1 - coeficitions.CylinderCoef;
    return coeficitions;
}

int Generator::Core(int countMoleculs, int iteration)
{
    int exitMolecules = 0;
    Coeficients coeficionts;
    Coordinates NewCoordinates;
    RandomValues randomValues;
    coeficionts = generator.Distribution();
    coeficionts.DiskCoef = generator.Distribution().DiskCoef;
    coeficionts.CylinderCoef = generator.Distribution().CylinderCoef;
    for (int i = 0; i <= countMoleculs* coeficionts.CylinderCoef;i++)
    {
        randomValues = generator.GeneratorMonteCarlo_Cylinder();//написать перегрузку для функции
        int j = 0;
        while (j < iteration)
        {
            NewCoordinates = FlightMolecule(randomValues);//то же самое
            if (isMoleculeExit(NewCoordinates)){
                exitMolecules++;
                break;
            } else {
                j++;
            }
        }
    }
    for (int i = 0; i <= countMoleculs*coeficionts.DiskCoef; i++)
    {
        randomValues = generator.GeneratorMonteCarlo_Disk();//написать перегрузку для функции
        int j = 0;
        while (j < iteration)
        {
            NewCoordinates = FlightMolecule(randomValues);//то же самое
            if (isMoleculeExit(NewCoordinates)){
               exitMolecules++;
                break;
            } else {
                j++;
            }
        }

    }
    return exitMolecules;
}//поменять потом функцию, тут как бы идёт добавление, а нужно сделать функцию, в которую передавать указатели на разные функции.

Coordinates Generator::FlightMolecule(RandomValues coordinates)
{   Coordinates NewCoordinates;
    for (int i = 0; i < vector.size(); i++){//
        if (vector[i]->name == "Disk") {
            NewCoordinates = FlyghtMoleculeDisk(coordinates, i);
            if (NewCoordinates.x == 444.444) {
                continue;
            } else {
                return NewCoordinates;
            }
        } else {
            NewCoordinates = FlyghtMoleculeCylinder(coordinates, i);
            if (NewCoordinates.x == 444.444) {
                continue;
            } else {
                return NewCoordinates;
            }
        }
    }
    // на основании имеющихся строим луч в трехмерном пространстве
}

bool Generator::isMoleculeExit(Coordinates coordinates)
{

}
