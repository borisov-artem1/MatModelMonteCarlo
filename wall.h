#ifndef WALL_H
#define WALL_H

#include <stack>
#include <iostream>
#include <QObject>
#include <QComboBox>

class Wall {
public:
    static int indexNumber;
    static double CoordinateZ;
    Wall(double RadiusOutside);
    ~Wall();
    friend double GeneratorMonteCarlo();
};


class Disk : public Wall {
public:
    int index;
    double RadiusOutside;
    double RadiusInside;
    double CoordinateOfTheBegining = CoordinateZ;
    Disk(double radius1, double radius2); // объявляем конструктор
    ~Disk(); // объявляем деструктор
};

class Сylinder : public Wall {
public:
    int index;
    double Height;
    double RadiusOutside;
    double CoordinateOfTheBegining = CoordinateZ;
    Сylinder(double radius1, double height);
    ~Сylinder();
};

template <typename T>
class Stack {
private:
  std::stack<T*> elements;
public:
  void push( T* value){
      elements.push(value);
  }

  T top() const {
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

#endif // WALL_H
