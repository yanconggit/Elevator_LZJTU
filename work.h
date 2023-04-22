#ifndef WORK_H
#define WORK_H

#include <QObject>
#include <QWidget>
#include "elevator.h"

class work
{

    Elevator elevator_work;
public:
    work();
public slots:
    void elevatorWork(void );
};

#endif // WORK_H
