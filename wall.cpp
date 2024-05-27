#include "wall.h"
#include <string>
#include <memory>
#include "interface.h"
#include <random>
#include <iostream>
#include <cmath>
#define PI 3.14
enum {
    FOUND = 1,
    NOT_FOUND = 0
};

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

Coordinates Generator::GeneratorMonteCarlo_GVector(Coordinates& coordinates) {
    double teta = GeneratorMonteCarlo_Teta();
    double gamma = GeneratorMonteCarlo_Gamma();
    coordinates.p1 = sin((teta * PI) / 180) * cos((gamma * PI) / 180);
    coordinates.p2 = sin((teta * PI) / 180) * sin((gamma * PI) / 180);
    coordinates.p3 = cos((teta * PI) / 180);
    return coordinates;
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
        this->index = coord.index;
    } else {
        this->x = other.point * cos(other.fi);
        this->y = other.point * sin(other.fi);
        this->z = vector[other.index - 1]->coordinateZ;
        this->index = other.index;
    }
    return *this;
}

void Generator::IntersectionSearch(Coordinates& NewCoordinates, std::vector<Coordinates>& vectorOfPoints, const RandomValues& rand, int k) {
    if (vector[k]->name == "Disk" && vector[k] != vector[rand.index]) {
        NewCoordinates = FlightMoleculeDisk(NewCoordinates, k);
        if (NewCoordinates.flag == FOUND) {
            generator.GeneratorMonteCarlo_GVector(NewCoordinates);
            vectorOfPoints.push_back(NewCoordinates);
        }
    } else if (vector[k]->name == "Cylinder" &&
               vector[k] != vector[(generator.FindCylinderIndex(rand.height)).index]){
        NewCoordinates = FlightMoleculeCylinder(NewCoordinates, k);
        if (NewCoordinates.flag == FOUND) {
            generator.GeneratorMonteCarlo_GVector(NewCoordinates);
            vectorOfPoints.push_back(NewCoordinates);
        }
    }
}

void Generator::IterationForCylinder(int iteration) {
    RandomValues rand = generator.GeneratorMonteCarlo_Cylinder();
    Coordinates NewCoordinates = {};
    std::vector<Coordinates> vectorOfPoints(iteration);
    NewCoordinates = rand;
    int j = 0;
    int count = 1;
    while (j < iteration) {
        while (NewCoordinates.flag != FOUND) {
            for (int k = (generator.FindCylinderIndex(rand.height)).index; k < k + count; ++k) {
                generator.IntersectionSearch(NewCoordinates, vectorOfPoints, rand, k);
                if (NewCoordinates.flag == FOUND) {
                    break;
                }
            }
            if (NewCoordinates.flag != FOUND) {
                for (int k = (generator.FindCylinderIndex(rand.height)).index; k > k - count; --k) {
                    generator.IntersectionSearch(NewCoordinates, vectorOfPoints, rand, k);
                    if (NewCoordinates.flag == FOUND) {
                        break;
                    }
                }
                ++count;
            }
        }
    }
}

void Generator::IterationForDisk(int iteration){
    RandomValues rand = generator.GeneratorMonteCarlo_Disk();
    Coordinates NewCoordinates = {};
    std::vector<Coordinates> vectorOfPoints(iteration);
    int j = 0;
    int count = 1;
    while (j < iteration) {
        while (NewCoordinates.flag != FOUND) {
            Disk* disk = dynamic_cast<Disk*>(vector[generator.GeneratorMonteCarlo_Disk().index]);
            if (disk->location==true) {
                for (int k = generator.GeneratorMonteCarlo_Disk().index; k >= 0; k--){
                    generator.IntersectionSearch(NewCoordinates, vectorOfPoints, rand, k-1);
                }
            } else {
                for (int k = generator.GeneratorMonteCarlo_Disk().index; k < vector.size(); k++) {
                    generator.IntersectionSearch(NewCoordinates, vectorOfPoints, rand, k+1);
                }
            }
        }
    }
}

int Generator::Core(int countMoleculs, int iteration)
{
    generator.CreatingPortal();
    RandomValues rand;
    int exitMolecules = 0;
    Coeficients coeficionts = {};
    Coordinates NewCoordinates = {};
    coeficionts = generator.Distribution();
    coeficionts.DiskCoef = generator.Distribution().DiskCoef;
    coeficionts.CylinderCoef = generator.Distribution().CylinderCoef;
    std::vector<Coordinates> vectorOfPoints(iteration);

    for (int i = 0; i < countMoleculs * coeficionts.CylinderCoef; ++i) {
        rand = generator.GeneratorMonteCarlo_Cylinder(); //написать перегрузку для функции
        NewCoordinates = rand;
        int j = 0;
        int count = 1;
        while (j < iteration) { // Нужно сделать генератор направляющих векторов в цикле
            while (NewCoordinates.flag != FOUND) {
                for (int k = (generator.FindCylinderIndex(rand.height)).index; k < k + count; ++k) {
                    if (vector[k]->name == "Disk") {
                        NewCoordinates = FlightMoleculeDisk(NewCoordinates, k);
                        if (NewCoordinates.flag == NOT_FOUND) {
                            //NewCoordinates.flag = 0;
                            continue;
                        } else {
                            vectorOfPoints.push_back(NewCoordinates);
                            break;
                        }
                    } else if (vector[k]->name == "Cylinder" &&
                               vector[k] != vector[(generator.FindCylinderIndex(rand.height)).index]){
                        NewCoordinates = FlightMoleculeCylinder(NewCoordinates, k);
                        if (NewCoordinates.flag == NOT_FOUND) {
                            //NewCoordinates.flag = 0;
                            continue;
                        } else {
                            vectorOfPoints.push_back(NewCoordinates);
                            break;
                        }
                    }
                }

                if (NewCoordinates.flag != FOUND) {
                    for (int k = (generator.FindCylinderIndex(rand.height)).index; k > k - count; --k) {
                        if (vector[k]->name == "Disk") {
                            NewCoordinates = FlightMoleculeDisk(NewCoordinates, k);
                            if (NewCoordinates.flag == NOT_FOUND) {
                                //NewCoordinates.flag = 0;
                                continue;
                            } else {
                                vectorOfPoints.push_back(NewCoordinates);
                                break;
                            }
                        } else if (vector[k]->name == "Cylinder" &&
                                   vector[k] != vector[(generator.FindCylinderIndex(rand.height)).index]) {
                            NewCoordinates = FlightMoleculeCylinder(NewCoordinates, k);
                            if (NewCoordinates.flag == NOT_FOUND) {
                                //NewCoordinates.flag = 0;
                                continue;
                            } else {
                                vectorOfPoints.push_back(NewCoordinates);
                                break;
                            }
                        }
                    }
                    ++count;
                }

            }
            if (isMoleculeExit(vectorOfPoints)) {
                exitMolecules++;
                break;
            } else {
                j++;
            }
        }
    }

    for (int i = 0; i < countMoleculs * coeficionts.DiskCoef; ++i) {
        rand = generator.GeneratorMonteCarlo_Disk();
        NewCoordinates = rand;
        int j = 0;
        Disk* disk = dynamic_cast<Disk*>(vector[rand.index]);
        while(j < iteration) {
            while (NewCoordinates.flag != FOUND) {

            }
        }
    }

    for (int i = 0; i < countMoleculs * coeficionts.DiskCoef; ++i) {// мои изменения!!!!!!!!!!!!
        rand = generator.GeneratorMonteCarlo_Disk(); //написать перегрузку для функции
        NewCoordinates = rand;
        int j = 0;
        Disk* disk = dynamic_cast<Disk*>(vector[rand.index]);
        if (disk->location) {
            while (j < iteration) { // здесь не рандомиться направляющий вектор, а должен

                for (int k = rand.index; k < vector.size(); ++k) {
                    if (vector[k]->name == "Disk" && vector[k] != vector[rand.index]) {
                        NewCoordinates = FlightMoleculeDisk(NewCoordinates, k);
                        if (NewCoordinates.flag == NOT_FOUND) {
                            //NewCoordinates.flag = 0;
                            continue;
                        } else {
                            break;
                        }
                    } else if (vector[k]->name == "Cylinder"){
                        NewCoordinates = FlightMoleculeCylinder(NewCoordinates, k);
                        if (NewCoordinates.flag == NOT_FOUND) {
                            //NewCoordinates.flag = 0;
                            continue;
                        } else {
                            break;
                        }
                    }
                }
                if (0/*isMoleculeExit(NewCoordinates)*/) {
                    ++exitMolecules;
                    break;
                } else {
                    ++j;
                }
            }
        } else {
            while (j < iteration) {
                for (int k = rand.index; k > 0; --k) {
                    if (vector[k]->name == "Disk" && vector[k] != vector[rand.index]) {
                        NewCoordinates = FlightMoleculeDisk(NewCoordinates, k);
                        if (NewCoordinates.flag == NOT_FOUND) {
                            continue;
                        } else {
                            break;
                        }
                    } else if (vector[k]->name == "Cylinder") {
                        NewCoordinates = FlightMoleculeCylinder(NewCoordinates, k);
                        if (NewCoordinates.flag == NOT_FOUND) {
                            continue;
                        } else {
                            break;
                        }
                    }
                }
            }
        }
    }
    return exitMolecules;
}//поменять потом функцию, тут как бы идёт добавление, а нужно сделать функцию, в которую передавать указатели на разные функции.




bool Generator::isMoleculeExit(std::vector<Coordinates>& points) {
    for (std::size_t i = 0; i < points.size(); ++i) {
        if (vector[points[i].index]->name == "Disk") {
            Disk* disk = dynamic_cast<Disk*>(vector[points[i].index]);
            if (disk->portal) {
                return true;
            }
        }
    }
    return false;
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

Coordinates Generator::FlightMoleculeCylinder(Coordinates coordinates, int i) { // ИСПРАВИТЬ СРОЧНО!!!!!!!
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

