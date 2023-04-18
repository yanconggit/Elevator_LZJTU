#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QMainWindow>
#include <QButtonGroup>

#define direction_stop 0
#define direction_up 1
#define direction_down 2
#define mode_allLaer 0
#define mode_oneLaer 1
#define mode_twoLaer 2

namespace Ui {
class Elevator;
}

class Elevator : public QMainWindow
{
    Q_OBJECT

private:
    int maxFloor = 13; //最高楼层
    int minFloor = 1; //lowest floor
    int baseFloor= 2; //base floor,can dock in every mode.
    bool inDoorStatus = 0; //0:colsed;1:opened;  the opening and closing status of elevator car doors.
    int currentFloor = baseFloor;  //default dock at baseFloor
    int peopleNum = 0; //the number of passengers in the elevator car.
    int maxPeoleNum = 20; //default maximun of people is 20 users
    int overloadFlag = 0; //0:no overload; 1:overload!!
    int direction = 0; //0:stop; 1:up;  2:down
    int mode = 0; //0:全层可停靠；1：单层停靠； 2：双层停靠

    bool destinationFloor[50] = {0}; //default no destination floor.
public:
    QButtonGroup *m_btnGroup1;

public:
    explicit Elevator(QWidget *parent = 0);
    ~Elevator();

    int getMaxFloor() const;
    void setMaxFloor(int newMaxFloor);

    int getMode() const;
    void setMode(int newMode);

    int getMinFloor() const;
    void setMinFloor(int newMinFloor);

    int getBaseFloor() const;
    void setBaseFloor(int newBaseFloor);

    bool getInDoorStatus() const;
    void setInDoorStatus(bool newInDoorStatus);

    int getCurrentFloor() const;
    void setCurrentFloor(int newCurrentFloor);

    int getPeopleNum() const;
    void setPeopleNum(int newPeopleNum);

    int getMaxPeoleNum() const;
    void setMaxPeoleNum(int newMaxPeoleNum);

    int getOverloadFlag() const;
    void setOverloadFlag(int newOverloadFlag);

    int getDirection() const;
    void setDirection(int newDirection);

    void paintEvent(QPaintEvent *event);
    void displayElevator(int x = 10, int y = 30);
    void setButton(int x = 10, int y = 30);
private:
    Ui::Elevator *ui;
};

#endif // ELEVATOR_H
