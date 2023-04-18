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
#include <QMouseEvent>
#include <QVariant>
#include <typeinfo>
#include <QButtonGroup>
//#include "widget.h"

 // 注意 下的人数不能超过轿厢内部总人数  下的人数按钮要补齐

Elevator::Elevator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Elevator)
{
    ui->setupUi(this);
    setFixedSize(1900, 1000);//
    setWindowTitle("兰州交通大学图书馆电梯模拟系统");//窗口标题
    this->update();
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
    setButton();

}

/**
 * @brief 绘制单个电梯的显示和交互
 *
 * @param event
 * @param x 电梯图形的左上角横坐标
 * @param x 电梯图形的左上角纵坐标
 *
 */
void Elevator::paintEvent(QPaintEvent *event)//绘制单个电梯
{
    displayElevator();
}

/**
 * @brief 绘制单个电梯的显示和交互
 *
 * @param event
 * @param x 电梯图形的左上角横坐标
 * @param x 电梯图形的左上角纵坐标
 *
 */
void Elevator::displayElevator(int x,int y)
{
    //绘制固定字符
    QPainter constText(this);
    constText.setPen(Qt::black);
    constText.setFont(QFont("宋体",16,QFont::Bold));
    constText.drawText(x+35,y+30,"电梯内部");
    constText.drawText(x+220,y+30,"运行动画");
    constText.drawText(x+340,y+30,"电梯外部");
    constText.setFont(QFont("宋体",12,QFont::Bold));
    constText.drawText(x+220,y+80,"乘客数：");
    constText.drawText(x+20,y+320,"下来的");
    constText.drawText(x+20,y+340,"乘客数");
    constText.drawText(x+105,y+320,"内部按钮");
    constText.restore();

    //绘制分割线
    QPainter cutoffRule(this);
    cutoffRule.setPen(QPen(Qt::black, 2));
    cutoffRule.drawLine(x+210,y+60,x+210,y+650); //垂直分割线
    cutoffRule.drawLine(x+330,y+60,x+330,y+650); //垂直分割线
    cutoffRule.drawLine(x+0,y+0,x+500,y+0); //外框
    cutoffRule.drawLine(x+500,y+690,x+500,y+0); //外框
    cutoffRule.drawLine(x+500,y+690,x+0,y+690); //外框
    cutoffRule.drawLine(x+0,y+0,x+0,y+690); //外框
    cutoffRule.setPen(QPen(Qt::black,1)); //绘制内部按钮外框
    cutoffRule.drawLine(x+80,y+300,x+200,y+300); //外框
    cutoffRule.drawLine(x+200,y+300,x+200,y+670); //外框
    cutoffRule.drawLine(x+200,y+670,x+80,y+670); //外框
    cutoffRule.drawLine(x+80,y+670,x+80,y+300); //外框
}
/**
 * @brief 绘制按钮  在构造函数中被调用
 *
 * @param event
 * @param x 电梯图形的左上角横坐标
 * @param x 电梯图形的左上角纵坐标
 *
 */
void Elevator::setButton(int x,int y)
{
    // 注意 下的人数不能超过轿厢内部总人数
    //下来人数的按钮
    QRadioButton* out0=new QRadioButton("0",this);
    QRadioButton* out1=new QRadioButton("1",this);
    QRadioButton* out2=new QRadioButton("2",this);
    QRadioButton* out8=new QRadioButton("8",this);
    QRadioButton* out9=new QRadioButton("9",this);
    out0->setGeometry((QRect(x+20,y+340,50,30)));
    out1->setGeometry((QRect(x+20,y+370,50,30)));
    out2->setGeometry((QRect(x+20,y+400,50,30)));
    out8->setGeometry((QRect(x+20,y+580,50,30)));
    out9->setGeometry((QRect(x+20,y+610,50,30)));
    out0->setFont(QFont("宋体",16));
    out1->setFont(QFont("宋体",16));
    out2->setFont(QFont("宋体",16));
    out8->setFont(QFont("宋体",16));
    out9->setFont(QFont("宋体",16));
    m_btnGroup1 = new QButtonGroup(this);
    m_btnGroup1->addButton(out0,0);
    m_btnGroup1->addButton(out1,1);
    m_btnGroup1->addButton(out2,2);
    m_btnGroup1->addButton(out8,3);
    m_btnGroup1->addButton(out9,4);
    out1->setChecked(1);  //默认选项

    //轿厢内部按钮 应该直接用button就可以了
    QPushButton* destinationFloor1_but=new QPushButton("1",this);
    QPushButton* destinationFloor2_but=new QPushButton("2",this);
    QPushButton* destinationFloor3_but=new QPushButton("1",this);
    QPushButton* destinationFloor4_but=new QPushButton("2",this);
    QPushButton* destinationFloor11_but=new QPushButton("11",this);
    QPushButton* destinationFloor12_but=new QPushButton("12",this);
    QPushButton* destinationFloor13_but=new QPushButton("13",this);
    QPushButton* destinationFloorOpen_but=new QPushButton("<||>",this);
    QPushButton* destinationFloorClose_but=new QPushButton(">||<",this);
    destinationFloor1_but->setGeometry(x+90,y+630,40,35);
    destinationFloor2_but->setGeometry(x+150,y+630,40,35);
    destinationFloor3_but->setGeometry(x+90,y+590,40,35);
    destinationFloor4_but->setGeometry(x+150,y+590,40,35);
    destinationFloor11_but->setGeometry(x+90,y+430,40,35);
    destinationFloor12_but->setGeometry(x+150,y+430,40,35);
    destinationFloor13_but->setGeometry(x+90,y+390,40,35);
    destinationFloorOpen_but->setGeometry(x+90,y+330,40,35);
    destinationFloorClose_but->setGeometry(x+150,y+330,40,35);

    // 电梯外部按钮


//    // 如果按钮组内ID为0的单选按钮被选中
//    if(m_btnGroup1->checkedId() == 0)
//    {

//    }
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

