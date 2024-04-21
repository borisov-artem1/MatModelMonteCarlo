#ifndef WALL_H
#define WALL_H

#include <stack>
#include <iostream>

class Wall {
public:
    static int indexNumber;
    static double CoordinateZ;
    Wall();
    ~Wall();
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
  std::stack<T> elements;
public:
  void push(const T& value){
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
};

class Create3DModel {
public:
    void contactingTheUser();
    bool isBuildingCorrectly();
};

#endif // WALL_H
