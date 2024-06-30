#include <QDebug>
#include "interface.h"
#include "wall.h"
#include <QApplication>
#include <pthread.h>
#include "analytical.h"


int main(int argc, char *argv[])
{
     QApplication a(argc, argv);
     Interface w;
     Analytics an;
     w.move(400, 400);
     w.show();
    // an.MatModeling();
    // w.testCriostat();
     return a.exec();
}
