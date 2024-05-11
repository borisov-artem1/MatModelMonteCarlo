#ifndef WALL_H
#define WALL_H

#include <vector>
#include <stack>
#include <iostream>
#include <QObject>
#include <QComboBox>

struct RandomValues {
    int index = 0;
    double height = 0;
    double fi;
    double teta;
    double gamma;
    double point = 0;
};

struct Coeficients {
    int CylinderCoef;
    int DiskCoef;
};

class Wall: public QObject {

    Q_OBJECT
public:
    double radiusInside;
    double radiusOutside;
    double Height;
    static int indexNumber;
    static double coordinateZ;
    QString name = "Wall";
    Wall();
    virtual ~Wall();
    friend double GeneratorMonteCarlo();
};

class Disk : public Wall {

public:
    double radiusInside;
    double radiusOutside;
    Disk(double radiusOutside, double radiusInside); // объявляем конструктор
    ~Disk(); // объявляем деструктор
    //const QString name = "Disk";
    int index;
};



class Сylinder : public Wall {

public:

    double radiusOutside;
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

    void FlightMolecule(bool Disk);
    int Core(int countMoleculs, int iteration);
};

#endif // WALL_H
