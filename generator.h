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


    void IntersectionSearch(Coordinates& NewCoordinates, int k);


    bool FlightMolecule_Disk();

    Coordinates FlightMolecule(Coordinates& coordinates);
    Coordinates FlightMoleculeCylinder(Coordinates& coordinates, int i);
    Coordinates FlightMoleculeDisk(Coordinates& coordinates, int i);

    void IterationForCylinder(Coordinates& coordinates);
    void IterationForDisk(Coordinates& coordinates);

    int Core(int countMoleculs, int iteration);

    void CreatingPortal(int val2 = 0, bool flag = false);
    void DownOrUp(int count);


    double FindDiskCoordZ(int index);
    bool CheckForBoundCondition(Coordinates coordinates, Сylinder* cylinder);
    void Iteration(Coordinates& NewCoordinates, int iteration);

    void test(){};
};

#endif // GENERATOR_H
