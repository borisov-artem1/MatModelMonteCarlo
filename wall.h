#ifndef WALL_H
#define WALL_H

class Wall {
public:
    double RadiusOutside;
    static int indexNumber;
    static double CoordinateZ;
    double Coordinate;
    Wall(double radius);
    ~Wall();
private:
};


class Dick : public Wall {
public:
    int index;
    double RadiusInside;
    Dick(double radius1, double radius2); // объявляем конструктор
    ~Dick(); // объявляем деструктор
};

class Сylinder : public Wall {
public:
    int index;
    double Height;
    double RadiusOutside;
    Cylinder(double radius1, double height);
    ~Cylinder();
};

#endif // WALL_H
