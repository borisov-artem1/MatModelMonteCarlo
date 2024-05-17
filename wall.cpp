#include "wall.h"
#include <string>
#include <memory>
#include "interface.h"
#include <random>
#include <iostream>
#include <cmath>
#define PI 3.14

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

Disk::Disk(double radiusOutside, double radiusInside): radiusInside(radiusInside), radiusOutside(radiusOutside) {
    index = indexNumber;
    name = "Disk";
}

Disk::~Disk() {
    std::cout << "poka";
}


Сylinder::Сylinder(double radiusOutside, double height): radiusOutside(radiusOutside), Height(height) {
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
    int firstPoint = vector[index]->radiusInside;
    int twicePoint = vector[index]->radiusOutside;
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
                double radius = cylinder->radiusOutside;
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
                double radiusInside = disk->radiusInside;
                double radiusOutside = disk->radiusOutside;
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

int Generator::Core(int countMoleculs, int iteration)
{
    pointOfIntersection point1;
    pointOfIntersection point2;
    int exitMolecules = 0;
    Coeficients coeficionts;
    coeficionts = generator.Distribution();
    coeficionts.DiskCoef = generator.Distribution().DiskCoef;
    coeficionts.CylinderCoef = generator.Distribution().CylinderCoef;
    for (int i = 0; i <= countMoleculs * coeficionts.DiskCoef; ++i)
    {
        if (generator.FlightMolecule_Disk()) {
            ++exitMolecules;
        }
    }
    for (int i = 0; i <= countMoleculs * coeficionts.CylinderCoef; ++i)
    {
        if (generator.FlightMolecule_Cylinder(point1, point2)) {
            ++exitMolecules;
        }
    }
    return exitMolecules;
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

std::vector<pointOfIntersection> Generator::IntersectionSearch(Wall* wall) {

}

bool Generator::FlightMolecule_Cylinder(pointOfIntersection& pointBegin, pointOfIntersection& pointEnd) {
/*
 * Эта функция находит две точки пересечения направляющего вектора
 * и цилиндра. Для этого нам необходимо задать три координаты напр
 * авляющего вектора p1,p2,p3 и начальную точку, всё это выбираетс
 * я рандомным образом по методу Монте-Карло. Далее по известным ф
 * ормулам находим точки пересечения прямой и бесконечного цилиндр
 * а, обозначаем точки как точки начала и конца, если выходим за в
 * ысоту цилиндра(выше или ниже), необходимо сделать проверку на в
 * ылет молекулы из всей модели. Для этого пробегаемся по вектору
 * моделей и находим толчки пересечения луча с другим цилиндром ил
 * и диском, если таковых не найдено, то необходимо вывести true,
 * это означает, что молекула покинула всю модель.
*/
    RandomValues initialСoordinates;
    pointOfIntersection tmp_point;
    initialСoordinates = generator.GeneratorMonteCarlo_Cylinder();
    findingCylinder coord = FindCylinderIndex(initialСoordinates.height);
    double p1 = sin((initialСoordinates.teta * PI) / 180) * cos((initialСoordinates.gamma * PI) / 180);
    double p2 = sin((initialСoordinates.teta * PI) / 180) * sin((initialСoordinates.gamma * PI) / 180);
    double p3 = cos((initialСoordinates.teta * PI) / 180);
    double x0 = vector[coord.index]->radiusOutside * cos((initialСoordinates.fi * PI) / 180);
    double y0 = vector[coord.index]->radiusOutside * sin((initialСoordinates.fi * PI) / 180);
    double z0 = initialСoordinates.height;
    double A = pow(p1, 2) + pow(p2, 2);
    double B = x0 * p1 + y0 * p2;
    double C = pow(x0, 2) + pow(y0, 2) - pow(vector[coord.index]->radiusOutside, 2);
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
            pointEnd.z < (vector[coord.index]->coordinateZ - vector[coord.index]->Height)) { // если это так, то необходимо найти другую фигуру, с которой пересечется луч
            bool breakCondition = false;
            for (int i = 0; i < vector.size(); ++i) {
                if (vector[i]->name == "Cylinder" && vector[i] != vector[coord.index]) { // это неправильно потому что мы ищем на
                        C = pow(x0, 2) + pow(y0, 2) - pow(vector[i]->radiusOutside, 2);  // бесконечных цилиндрах
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
                        if (sqrt(pow(x0, 2) + pow(y0, 2)) > vector[i]->radiusInside &&
                            sqrt(pow(x0, 2) + pow(y0, 2)) < vector[i]->radiusOutside) {
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


