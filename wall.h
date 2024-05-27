#ifndef WALL_H
#define WALL_H

#include <vector>
#include <stack>
#include <iostream>
#include <QObject>
#include <QComboBox>

class Generator;

struct RandomValues {
    int index = 0;
    double height = 0.;
    double fi;
    double teta;
    double gamma;
    double point = 0.;
};

struct pointOfIntersection {
    double x;
    double y;
    double z;
};

struct findingCylinder {
    int index;
    double diff;
};

struct Coeficients {
    int CylinderCoef;
    int DiskCoef;
};

struct Coordinates {
    Coordinates& operator=(const RandomValues& other);
    int flag = 0;
    double p1 = 0.;
    double p2 = 0.;
    double p3 = 0.;
    double x;
    double y;
    double z;
    int index;
};

class Wall: public QObject {

    Q_OBJECT
public:
    static int indexNumber;
    static double coordinateZ;
    QString name = "Wall";
    Wall();
    virtual ~Wall();
    friend double GeneratorMonteCarlo();
};

class Disk : public Wall {

public:
    double radiusInsideDisk;
    double radiusOutsideDisk;
    Disk(double radiusOutside, double radiusInside); // объявляем конструктор
    ~Disk(); // объявляем деструктор
    //const QString name = "Disk";
    int index;
    bool portal;// true если да, false если нет
    bool location; // true если сверху, false если снизу, мои изменения
};



class Сylinder : public Wall {

public:

    double radiusOutsideCylinder;
    double Height;
    Сylinder(double radiusOutside, double height);
    ~Сylinder();
    //const QString name = "Cylinder";
    int index;
};

template <typename T>
class Stack {
private:
  std::stack<T*> elements;
public:
  Stack();
  void push(T* value){
      elements.push(value);
  }

  std::size_t Size() {
      return elements.size();
  }

  T* top() const {
      return elements.top();
  }

  void pop() {
      elements.pop();
  }

  bool empty() const {
      return elements.empty();
  }

  ~Stack() {
      while (!elements.empty()){
          delete elements.top();
          elements.pop();
      }
  }
};

class Generator {
  public:
    double GeneratorMonteCarlo_Height();
    double GeneratorMonteCarlo_Fi();
    double GeneratorMonteCarlo_Teta();
    double GeneratorMonteCarlo_Gamma();
    RandomValues GeneratorMonteCarlo_Cylinder();

    int GeneratorMonteCarlo_index();
    void LookDiskIndexes();
    double GeneratorMonteCarlo_Point(int index);
    RandomValues GeneratorMonteCarlo_Disk();

    RandomValues PlaceForMolecul();

    double CylindersArea();
    double DiskArea();
    Coeficients Distribution();
    findingCylinder FindCylinderIndex(double height);


    std::vector<pointOfIntersection> IntersectionSearch(Wall* wall);

    bool FlightMolecule_Cylinder(pointOfIntersection& point1, pointOfIntersection& point2);
    bool FlightMolecule_Disk();

    Coordinates FlightMolecule(Coordinates& coordinates);
    Coordinates FlightMoleculeCylinder(Coordinates coordinates, int i);
    Coordinates FlightMoleculeDisk(Coordinates coordinates, int i);

    //Coordinates FlightMolecule(RandomValues coordinates);
    //Coordinates FlyghtMoleculeCylinder(RandomValues coordinates, int i);
    //Coordinates FlyghtMoleculeDisk(RandomValues coordinates, int i);

    int Core(int countMoleculs, int iteration);

    bool isMoleculeExit(std::vector<Coordinates>& points);
    void CreatingPortal();
    void DownOrUp(int count);

};

#endif // WALL_H
