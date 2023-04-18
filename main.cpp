#include "elevator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Elevator w;
    w.show();
    w.update();
    return a.exec();
}
