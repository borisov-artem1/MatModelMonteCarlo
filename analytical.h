#ifndef ANALYTICAL_H
#define ANALYTICAL_H
#include <QVector>

class Analytics
{
public:
    void MatModeling();
    double DispCalc(QVector<double>& XiVec, QVector<double>& DispVec, int molecules, int iteration);
    void checkingReproducibility(const QVector<double>& DispVec);
    QVector<double> polinomialCoefCalc(const QVector<double>& avgVec);
    int significanceOfTheCoefficients(const QVector<double>& polinomCoef, double& ReproducibilityDisp,const QVector<double>& DispVec);
    void adequacyOfTheModel(int alphaSig, const QVector<double>& polinomCoef, const double& ReproducibilityDisp, const QVector<double>& avgVec);
};


#endif // ANALYTICAL_H
