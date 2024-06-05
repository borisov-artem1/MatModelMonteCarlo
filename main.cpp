#include <QDebug>
#include "interface.h"
#include "wall.h"
#include <QApplication>

int main(int argc, char *argv[])
{
     QApplication a(argc, argv);
     Interface w;
//     w.move(400, 400);
//     w.show();
     w.testCriostat();
     return a.exec();
}
