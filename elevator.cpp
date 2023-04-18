// 20230417
// Design By 王彦崇
// URL: https://github.com/yanconggit/Elevator.git
//
#include "elevator.h"
#include "ui_elevator.h"
#include "qpainter.h"
#include "qsize.h"
#include "QVBoxLayout"
#include <QDebug>
#include<QMouseEvent>
#include <QVariant>
#include <typeinfo>

Elevator::Elevator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Elevator)
{
    ui->setupUi(this);

//    this-> maxFloor = 13; //最高楼层
//    this-> minFloor = 1; //lowest floor
//    this-> baseFloor= 2; //base floor,can dock in every mode.
//    this-> inDoorStatus = 0; //0:colsed;1:opened;  the opening and closing status of elevator car doors.
//    this-> currentFloor = baseFloor;  //default dock at baseFloor
//    this-> peopleNum = 0; //the number of passengers in the elevator car.
//    this-> maxPeoleNum = 20; //default maximun of people is 20 users
//    this-> overloadFlag = 0; //0:no overload; 1:overload!!
//    this-> destinationFloor[50] = {0}; //default no destination floor.
//    this-> direction = 0; //0:stop; 1:up;  2:down
//    this-> mode = 0; //0:全层可停靠；1：单层停靠； 2：双层停靠

    setFixedSize(1600, 1200);//
    setWindowTitle("兰州交通大学图书馆电梯模拟系统");//窗口标题
}

/**
 * @brief 绘制单个电梯的显示和交互
 *
 * @param event
 * @param x 电梯图形的左上角横坐标
 * @param x 电梯图形的左上角纵坐标
 *
 */
void Elevator::paintEvent(QPaintEvent *event,int x,int y)//绘制单个电梯
{
    QPainter constText(this);   //绘制固定字符
    constText.setPen(Qt::black);
    constText.setFont(QFont("宋体",20,QFont::Bold));
    constText.drawText(35,10,"电梯内部");

    constText.drawText(15, 430, "天津方向");
    constText.drawText(10, 500, "河");
    constText.drawText(10, 520, "南");
    constText.drawText(160, 540, "方");
    constText.drawText(103, 560, "向");
    constText.restore();

}

Elevator::~Elevator()
{
    delete ui;
}

int Elevator::getMode() const
{
    return mode;
}

void Elevator::setMode(int newMode)
{
    mode = newMode;
}

int Elevator::getMaxFloor() const
{
    return maxFloor;
}

void Elevator::setMaxFloor(int newMaxFloor)
{
    maxFloor = newMaxFloor;
}



int Elevator::getMinFloor() const
{
    return minFloor;
}

void Elevator::setMinFloor(int newMinFloor)
{
    minFloor = newMinFloor;
}

int Elevator::getBaseFloor() const
{
    return baseFloor;
}

void Elevator::setBaseFloor(int newBaseFloor)
{
    baseFloor = newBaseFloor;
}

bool Elevator::getInDoorStatus() const
{
    return inDoorStatus;
}

void Elevator::setInDoorStatus(bool newInDoorStatus)
{
    inDoorStatus = newInDoorStatus;
}

int Elevator::getCurrentFloor() const
{
    return currentFloor;
}

void Elevator::setCurrentFloor(int newCurrentFloor)
{
    currentFloor = newCurrentFloor;
}

int Elevator::getPeopleNum() const
{
    return peopleNum;
}

void Elevator::setPeopleNum(int newPeopleNum)
{
    peopleNum = newPeopleNum;
}

int Elevator::getMaxPeoleNum() const
{
    return maxPeoleNum;
}

void Elevator::setMaxPeoleNum(int newMaxPeoleNum)
{
    maxPeoleNum = newMaxPeoleNum;
}

int Elevator::getOverloadFlag() const
{
    return overloadFlag;
}

void Elevator::setOverloadFlag(int newOverloadFlag)
{
    overloadFlag = newOverloadFlag;
}

int Elevator::getDirection() const
{
    return direction;
}

void Elevator::setDirection(int newDirection)
{
    direction = newDirection;
}

