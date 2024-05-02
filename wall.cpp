#include "wall.h"
#include <string>
#include <memory>
#include "interface.h"
#include <random>
#include <vector>

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

//берет значения от нуля до высоты нашей модели и находит случайную
//точку по координате z , и именно сечение по этой координате мы будем исследовать
double GeneratorMonteCarlo_Height()
{
    std::mt19937 generator; // Инициализация генератора
    int z_finish = Wall::CoordinateZ;
    generator.seed(std::random_device()()); // Использование случайного устройства для засевания генератора
    std::uniform_int_distribution<int> distribution(0, z_finish); // Равномерное распределение от 1 до 100
    int randomValue = distribution(generator); // Генерация случайного числа
    return randomValue;
}

//далее в сечении из предыдущей функции находим конкретную точку
double GeneratorMonteCarlo_Fi()
{
    std::mt19937 generator;
    generator.seed(std::random_device()()); // Использование случайного устройства для засевания генератора
    std::uniform_int_distribution<int> distribution(0, 360);
    int randomValue = distribution(generator);
    return randomValue;
}

//это один из 2 углов, который будет необходим для построения луча, по которому молекула будет вылетать
double GeneratorMonteCarlo_Teta()
{
    std::mt19937 generator;
    generator.seed(std::random_device()());
    std::uniform_int_distribution<int> distribution(0, 180); // Равномерное распределение от 1 до 180
    int fi = distribution(generator);
    return fi;
}

//это 2 угол, который будет необходим для построения луча, по которому молекула будет вылетать
double GeneratorMonteCarlo_Gamma()
{
    std::mt19937 generator;
    generator.seed(std::random_device()());
    std::uniform_int_distribution<int> distribution(0, 180); // Равномерное распределение от 1 до 180
    int fi = distribution(generator);
    return fi;
}

CylinderValues GeneratorMonteCarlo_Cylinder()
{
    CylinderValues cylinderValues;
    cylinderValues.height = GeneratorMonteCarlo_Height();
    cylinderValues.fi = GeneratorMonteCarlo_Fi();
    cylinderValues.teta = GeneratorMonteCarlo_Teta();
    cylinderValues.gamma = GeneratorMonteCarlo_Gamma();
    return cylinderValues;
}


