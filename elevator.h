#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QTimer>
#include "qpushbutton.h"
#include "qlcdnumber.h"
#include "qpixmap.h"
#include "qpainter.h"
#include "qlabel.h"
#include "qradiobutton.h"

#define direction_stop 0
#define direction_up 1
#define direction_down 2
#define mode_allLaer 0
#define mode_oneLaer 1
#define mode_twoLaer 2
#define doorStatus_closed  0
#define doorStatus_opening  1
#define doorStatus_opened  2
#define doorStatus_closing  3



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
    int inDoorStatus = 0; //0:colsed;1:opening; 2:opened ;3:closing; the opening and closing status of elevator car doors.
    int currentFloor = baseFloor;  //default dock at baseFloor
    int peopleNum = 0; //the number of passengers in the elevator car.
    int maxPeoleNum = 20; //default maximun of people is 20 users
    int overloadFlag = 0; //0:no overload; 1:overload!!
    int direction = 0; //0:stop; 1:up;  2:down
    int mode = 0; //0:全层可停靠；1：单层停靠； 2：双层停靠

    bool destinationFloor[50] = {0}; //destination[1] is first floor.default no destination floor.
    bool upRequest[50]={0},downRequest[50]={0};// [1]is first floor 0:no request;1:have request

    volatile bool flashFlag = 0;
    int begin_x=10,begin_y=30;
public:
    QButtonGroup *m_btnGroup1;
    QTimer *Timer_cartoon,*runTimeBaseTimer;
    QPushButton fullLoadIndicator,FxUp[13],FxDown[13],destinationFloorBut[13],destinationFloorOpen_but,
    destinationFloorClose_but;
    QLCDNumber floorIndicator,floorIndicatorOut;
    QPixmap up_red,up_gray,down_red,down_gray;
    QLabel doorStatus,peopleNumLabel,waitingUpNum[13],waitingDownNum[13],carLocation[2];
    QLabel up_indoor,down_indoor,up_outdoor,down_outdoor;
    QRadioButton leavePeopleNum[10];


    volatile int doorOffset = 65,runTimeBase = 0;

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

    int getInDoorStatus() const;
    void setInDoorStatus(int newInDoorStatus);

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

    void setDestinationFloor(int destination_Floor);
    void resetDestinationFloor(int destination_Floor);
    bool getDestinationFloor(int destination_Floor);

    void setupRequest(int );
    void resetupRequest(int );
    bool getupRequest(int );

    void setdownRequest(int request_floor);
    void resetdownRequest(int request_floor);
    bool getdownRequest(int request_floor);

    void paintEvent(QPaintEvent *event);
    void displayElevator();
    void setButton();
    void showDirection();
    void showCarLocation();
public:

    Ui::Elevator *ui;
    void carDoorCartoon(volatile int action);

    void runAnimation();
    void destinationFloorButFun(int pushedBut);
    bool demandQuery(int direction);

    int OthersDemandQuery(int direction);
public slots:
    void handleCartoon();  //超时处理函数
    void flashTimeBase();
    void leaveNumConfirm(void);
    void downRequestFun(int requestFool);
    void upRequestFun(int requestFool);
    void workControl();
    void runTimeBaseFun();
    void openDoorButFun();
    void closeDoorButFun();
private:


};

#endif // ELEVATOR_H
