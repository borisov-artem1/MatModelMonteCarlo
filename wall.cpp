#include "wall.h"
#include <string>
#include <memory>

double Wall::CoordinateZ = 0;
int Wall::indexNumber = 0;

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
    CoordinateZ+=Height;
}

Сylinder::~Сylinder() {}

void Create3DModel::contactingTheUser() {
    Stack<Wall*> stack;
    std::string userChoice;
    double radius1, radius2, height;

    while (true) {
        std::cout << "Выберите объект для создания ('cylinder', 'disk', 'base' для диска с нулевым радиусом, 'exit' для выхода): ";
        std::cin >> userChoice;

        if (userChoice == "exit") {
            break;
        }

        std::cout << "Введите внешний радиус: ";
        std::cin >> radius1;

        if (userChoice == "cylinder") {
            std::cout << "Введите высоту: ";
            std::cin >> height;
            Сylinder* cylinder = new Сylinder(radius1, height);
            stack.push(cylinder);
        } else if (userChoice == "disk" || userChoice == "base") {
            radius2 = (userChoice == "disk") ? std::cin >> radius2, radius2 : 0;
            Disk* disk = new Disk(radius1, radius2);
            stack.push(disk);
        } else {
            std::cout << "Некорректный ввод. Попробуйте снова." << std::endl;
            continue;
        }
    }

    // Демонстрация содержимого стека
    while (!stack.empty()) {
        Wall* item = stack.top();
        stack.pop();
        // Для реального использования тут должна быть логика, которая что-то делает с объектами
        delete item; // Удаляем объект после использования
    }
}
