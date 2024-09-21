#ifndef CALCULATE_H
#define CALCULATE_H
#include "wall.h"

class Calculate {
public:
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

#endif // CALCULATE_H
