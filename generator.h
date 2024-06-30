#ifndef GENERATOR_H
#define GENERATOR_H
#include "wall.h"

class Generator {
  public:
    bool IsDiskInVector();
    double GeneratorMonteCarlo_Height();
    double GeneratorMonteCarlo_Fi();
    double GeneratorMonteCarlo_Teta();
    double GeneratorMonteCarlo_Gamma();
    RandomValues GeneratorMonteCarlo_Cylinder();
    Coordinates GeneratorMonteCarlo_GVector(Coordinates& coordinates);

    int GeneratorMonteCarlo_index();
    void LookDiskIndexes();
    double GeneratorMonteCarlo_Point(int index);
    RandomValues GeneratorMonteCarlo_Disk();

    RandomValues PlaceForMolecul();

    double CylindersArea();
    double DiskArea();
    Coeficients Distribution();
    findingCylinder FindCylinderIndex(double height);
};

#endif // GENERATOR_H
