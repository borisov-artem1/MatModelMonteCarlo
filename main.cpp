
#include "interface.h"
#include "wall.h"
#include <QApplication>

int main(int argc, char *argv[])
{
     Create3DModel create;
     create.contactingTheUser();
     QApplication a(argc, argv);
     Interface w;
     w.show();
     return a.exec();
}
