#ifndef WALL_H
#define WALL_H

class Wall {
public:
    double Radius;
    static int indexNumber;
    static double CoordinateZ;
    double Coordinate;
    Wall(double radius);
    ~Wall();
private:
};

double Wall::CoordinateZ = 0;
int Wall::indexNumber = 0;

Wall::Wall(double radius) {
    Radius = radius;
    indexNumber++;
}

Wall::~Wall() {
    // деструктор Wall
}

class Dick : public Wall {
public:
    int index = indexNumber;
    Dick(double radius); // объявляем конструктор
    ~Dick(); // объявляем деструктор
};

Dick::Dick(double radius) : Wall(radius) {
    // конструктор Dick вызывает конструктор Wall
}

Dick::~Dick() {
    // деструктор Dick
}
#endif // WALL_H
