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

struct Angles {
   double Teta;
   double Gamma;
};

double GeneratorMonteCarlo(int choice, Wall object)
{
    std::mt19937 generator; // Инициализация генератора
    if (choice == 1) {
        int z_finish = object.CoordinateZ;
        generator.seed(std::random_device()()); // Использование случайного устройства для засевания генератора
        std::uniform_int_distribution<int> distribution(0, z_finish); // Равномерное распределение от 1 до 100
        int randomValue = distribution(generator); // Генерация случайного числа
        return randomValue;
    }
    if (choice == 2) {
        generator.seed(std::random_device()()); // Использование случайного устройства для засевания генератора
        std::uniform_int_distribution<int> distribution(0, 180);
        int randomValue = distribution(generator);
        return randomValue;
    }
}

Angles GeneratorMonteCarloRadian()
{
    Angles angles;
    std::vector<double> arr;  // пустой вектор строк
    std::mt19937 generator;
    for (int i = 0; i < 2; i++){
        generator.seed(std::random_device()());
        std::uniform_int_distribution<int> distribution(0, 180); // Равномерное распределение от 1 до 100
        arr.push_back(distribution(generator));
    }
    angles.Teta = arr[0];
    angles.Gamma = arr[1];
    return angles;
}


