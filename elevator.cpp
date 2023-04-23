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
#include <QTimer>
#include <qfont.h>
#include "work.h"

#define TEST

#ifdef TEST
#define runTimerCycle 200
#else
#define runTimerCycle 200
#endif




 // 注意 下的人数不能超过轿厢内部总人数  下的人数按钮要补齐
//carDoorCartoon 防止被重复调用，一定要注意时机

Elevator::Elevator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Elevator)
{
    ui->setupUi(this);
    setFixedSize(1900, 800);//
    setWindowTitle("兰州交通大学图书馆电梯模拟系统");//窗口标题
    this->update();
    Timer_cartoon = new QTimer();
    connect(Timer_cartoon,SIGNAL(timeout()),this,SLOT(handleCartoon()));
    runTimeBaseTimer = new QTimer();
    connect(runTimeBaseTimer, SIGNAL(timeout()), this, SLOT(runTimeBaseFun()));

    QTimer *Tclock1 = new QTimer(this);//定时器 提供灯光闪烁时间基准
    Tclock1->setTimerType(Qt::PreciseTimer);
    connect(Tclock1, SIGNAL(timeout()), this, SLOT(flashTimeBase()));
    Tclock1->start(200);
    //    this-> destinationFloor[50] = {0}; //default no destination floor.
//    this-> direction = 0; //0:stop; 1:up;  2:down
//    this-> mode = 0; //0:全层可停靠；1：单层停靠； 2：双层停靠

    //up_red.load("E:/system/Document/!project/qt/Elevator-LZJTU/elevator/upRed.png");
    up_red.load((":/upRed.png"));
    up_gray.load(":/upGray.png");
    down_red.load((":/downRed.png"));
    down_gray.load(":/downGray.png");
    setButton();
    //setInDoorStatus(1); //测试用的
//    QTimer *control = new QTimer(this);
//    control->setTimerType(Qt::PreciseTimer);
//    connect(control,SIGNAL(timeout()),this,SLOT(work::elevatorWork()));
//    control->start(200);

    QTimer *control = new QTimer(this);
    control->setTimerType(Qt::PreciseTimer);
    connect(control,SIGNAL(timeout()),this,SLOT(workControl()));
    control->start(200);

}

void Elevator::setMode()
{
    if(mode==mode_allLaer)
    {}
    else if(mode==mode_oneLaer) //单层运行
    {
        for(int i=0;i<13;i++)
        {
            if(i%2) //单数 双层
            {
                destinationFloorBut[i].setDisabled(true);
                FxUp[i].setDisabled(true);
                FxDown[i].setDisabled(true);
            }
        }
    }

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
    carDoorCartoon(getInDoorStatus());
    runAnimation();
}

/**
 * @brief 轿厢门和厅门开关动画，在paint Event中调用
 *
 * @param action 0:门已经关好,1:开门ing 2门已经打开 3：关门ing  ，
 */
void Elevator::carDoorCartoon(volatile int action)
{
    QPainter cardoor(this);
    QPainter indoorDraw(this);
    cardoor.setPen(QPen(Qt::black, 2));
    cardoor.drawRect(begin_x+40,begin_y+180,130,110);
    indoorDraw.setPen(Qt::black);
    indoorDraw.setPen(3);
    indoorDraw.setBrush(Qt::SolidPattern);
    indoorDraw.setBrush(Qt::gray);

    if(action == doorStatus_closed) //closed
    {
        doorStatus.setStyleSheet("color:black;");
        doorStatus.setText("门已关好");
        indoorDraw.drawRect(begin_x+40,begin_y+180,65,110);
        indoorDraw.drawRect(begin_x+105,begin_y+180,65,110);
        //当前楼层的厅门
        indoorDraw.drawRect(begin_x+225,begin_y+155+(13-getCurrentFloor())*40,35,31);
        indoorDraw.drawRect(begin_x+260,begin_y+155+(13-getCurrentFloor())*40,35,31);
    }
    if(action == doorStatus_opening) //opening
    {
        doorStatus.setStyleSheet("color:red;");
        doorStatus.setText("正在开门");
        if(Timer_cartoon->isActive() ==false)
            Timer_cartoon->start(100);
        indoorDraw.eraseRect(begin_x+40,begin_y+180,65,110);
        indoorDraw.eraseRect(begin_x+105,begin_y+180,65,110);
        indoorDraw.drawRect(begin_x+40,begin_y+180,doorOffset,110);
        indoorDraw.drawRect(begin_x+170-doorOffset,begin_y+180,doorOffset,110);
        //当前楼层的厅门
        indoorDraw.eraseRect(begin_x+225,begin_y+155+(13-getCurrentFloor())*40,70,31);
        indoorDraw.drawRect(begin_x+225,begin_y+155+(13-getCurrentFloor())*40,qRound(doorOffset*0.54),31);
        indoorDraw.drawRect(qRound(begin_x+295-doorOffset*0.54),begin_y+155+(13-getCurrentFloor())*40,qRound(doorOffset*0.54),31);

    }
    else if(action == doorStatus_opened) //opened
    {
        if(Timer_cartoon->isActive() ==false)
            Timer_cartoon->start(100);

        doorStatus.setStyleSheet("color:black;");
        doorStatus.setText("门已打开");
//        indoorDraw.eraseRect(begin_x+40,begin_y+180,65,110);
//        indoorDraw.eraseRect(begin_x+105,begin_y+180,65,110);
//        //当前楼层的厅门
//        indoorDraw.eraseRect(begin_x+225,begin_y+155+(13-getCurrentFloor())*40,35,31);
//        indoorDraw.eraseRect(begin_x+260,begin_y+155+(13-getCurrentFloor())*40,35,31);
    }

    else if(action == doorStatus_closing) //closing
    {
        doorStatus.setStyleSheet("color:red;");
        doorStatus.setText("正在关门");
        if(Timer_cartoon->isActive() ==false)
            Timer_cartoon->start(100);
        indoorDraw.eraseRect(begin_x+40,begin_y+180,65,110);
        indoorDraw.eraseRect(begin_x+105,begin_y+180,65,110);
        indoorDraw.drawRect(begin_x+40,begin_y+180,doorOffset,110);
        indoorDraw.drawRect(begin_x+170-doorOffset,begin_y+180,doorOffset,110);
        //当前楼层的厅门
        indoorDraw.eraseRect(begin_x+225,begin_y+155+(13-getCurrentFloor())*40,70,31);
        indoorDraw.drawRect(begin_x+225,begin_y+155+(13-getCurrentFloor())*40,qRound(doorOffset*0.54),31);
        indoorDraw.drawRect(qRound(begin_x+295-doorOffset*0.54),begin_y+155+(13-getCurrentFloor())*40,qRound(doorOffset*0.54),31);

    }
    if(doorOffset <= 0&&(action==doorStatus_opening))
    {
        int temp = action;
        temp +=1;
        if(temp >=4) temp = 0;
        setInDoorStatus(temp);
        doorOffset = 0;
        Timer_cartoon->stop();
    }
    else if(doorOffset >= 65&&(action==doorStatus_closing))
    {
        int temp = action;
        temp +=1;
        if(temp >=4) temp = 0;
        setInDoorStatus(temp);
        doorOffset = 65;
        Timer_cartoon->stop();
    }
#ifdef TEST
    else if(doorOffset >= 50&&(action==doorStatus_opened))  //开门时间够了 关门
#else
    else if(doorOffset >= 150&&(action==doorStatus_opened))  //开门时间够了 关门
#endif
    {
        int temp = action;
        temp +=1;
        if(temp >=4) temp = 0;
        setInDoorStatus(temp);
        doorOffset = 0;
        Timer_cartoon->stop();
    }
}

void Elevator::runAnimation()
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black,2));
    painter.drawRect(begin_x+225,begin_y+155,70,513); //运行图的外框
    painter.setPen(QPen(Qt::black,8));
    for(int i=1;i<=12;i++) //绘制楼层分割线
    {
        painter.drawLine(begin_x+230,begin_y+151+i*40,begin_x+290,begin_y+151+i*40);
    }
    painter.setPen(1);
    painter.setBrush(Qt::gray);
    for(int i=13;i>0;i--)  //绘制厅门
    {
        if(i == getCurrentFloor())
        {
            continue;
        }
        painter.drawRect(begin_x+225,begin_y+155+(13-i)*40,35,31);
        painter.drawRect(begin_x+260,begin_y+155+(13-i)*40,35,31);
    }
}

/**
 * @brief 绘制单个电梯的显示和交互
 *
 * @param event
 * @param x 电梯图形的左上角横坐标
 * @param x 电梯图形的左上角纵坐标
 *
 */
void Elevator::displayElevator()
{
    //绘制固定字符
    QPainter constText(this);
    constText.setPen(Qt::black);
    constText.setFont(QFont("宋体",16,QFont::Bold));
    constText.drawText(begin_x+25,begin_y+30,"电梯内部");
    constText.drawText(begin_x+210,begin_y+30,"运行动画");
    constText.drawText(begin_x+330,begin_y+30,"电梯外部");

    constText.setFont(QFont("宋体",12,QFont::Bold));
    constText.drawText(begin_x+210,begin_y+80,"乘客数：");
    constText.drawText(begin_x+10,begin_y+320,"下来的");
    constText.drawText(begin_x+10,begin_y+340,"乘客数");
    constText.drawText(begin_x+95,begin_y+320,"内部按钮");

    constText.setFont(QFont("宋体",10));
    constText.drawText(begin_x+10,begin_y+77,"超载指示灯");
    //constText.restore();

    //绘制分割线
    QPainter cutoffRule(this);
    cutoffRule.setPen(QPen(Qt::black, 2));
    cutoffRule.drawLine(begin_x+200,begin_y+60,begin_x+200,begin_y+650); //垂直分割线
    cutoffRule.drawLine(begin_x+320,begin_y+60,begin_x+320,begin_y+650); //垂直分割线
    cutoffRule.drawRect(begin_x,begin_y,460,690);  //外框
    cutoffRule.setPen(QPen(Qt::black,1)); //绘制内部按钮外框
    cutoffRule.drawRect(begin_x+70,begin_y+300,120,370);  //外框

    //超载指示灯
    if(getOverloadFlag()) //超载
    {
        if(flashFlag)
            fullLoadIndicator.setStyleSheet("QPushButton{border-radius:20px;border:1px solid rgb(50, 50, 50);background-color:red}");
        else
            fullLoadIndicator.setStyleSheet("QPushButton{border-radius:20px;border:7px solid rgb(240, 240, 240);background-color:red}");
    }
    else
        fullLoadIndicator.setStyleSheet("QPushButton{border-radius:20px;border:1px solid rgb(50, 50, 50);background-color:green;}");

    // 楼层指示
    floorIndicator.display(QString::number(getCurrentFloor()));
    floorIndicatorOut.display(QString::number(getCurrentFloor()));



    // 电梯外部框线绘制
    QPainter ElevatorOut(this);
    ElevatorOut.setPen(QPen(Qt::black,2));
    ElevatorOut.setFont(QFont("宋体",9));
    for(int i=0;i<=12;i++) //楼层字符和外框绘制
    {
        ElevatorOut.drawRect(begin_x+330,begin_y+155+i*40,120,31);
        ElevatorOut.drawText(begin_x+335,begin_y+177+i*40,"F"+QString::number(13-i));
    }


}
/**
 * @brief 绘制按钮  在构造函数中被调用
 *
 * @param event
 * @param x 电梯图形的左上角横坐标
 * @param x 电梯图形的左上角纵坐标
 *
 */
void Elevator::setButton()
{
    // 注意 下的人数不能超过轿厢内部总人数
    //下来人数的按钮
    m_btnGroup1 = new QButtonGroup(this);
    for(int i=0;i<10;i++)
    {
        leavePeopleNum[i].setParent(this);
        leavePeopleNum[i].setText(QString::number(i));
        leavePeopleNum[i].setGeometry(QRect(begin_x+10,begin_y+340+30*i,50,30));
        leavePeopleNum[i].setFont(QFont("宋体",16));
        
        m_btnGroup1->addButton(&leavePeopleNum[i],i);
    }
    leavePeopleNum[0].setChecked(1);
    QPushButton* leaveNumConfirmBut=new QPushButton("确认",this);//确认按钮
    leaveNumConfirmBut->setGeometry(begin_x+10,begin_y+645,50,40);
    connect(leaveNumConfirmBut,&QPushButton::clicked,this,&Elevator::leaveNumConfirm);

    //轿厢内部按钮 应该直接用button就可以了
    destinationFloorOpen_but.setParent(this);
    destinationFloorClose_but.setParent(this);
    destinationFloorOpen_but.setGeometry(begin_x+80,begin_y+330,40,35);
    destinationFloorClose_but.setGeometry(begin_x+140,begin_y+330,40,35);
    destinationFloorOpen_but.setText("<||>");
    destinationFloorClose_but.setText(">||<");
    connect(&destinationFloorOpen_but,&QPushButton::clicked,this,Elevator::openDoorButFun);
    connect(&destinationFloorClose_but,&QPushButton::clicked,this,Elevator::closeDoorButFun);

    for(int i=0;i<13;i++)
    {
        if(i%2) //右边那一列的
        {
            destinationFloorBut[i].setParent(this);
            destinationFloorBut[i].setGeometry(begin_x+140,begin_y+630-(int)(i/2)*40,40,35);
            destinationFloorBut[i].setText(QString::number(i+1));
            connect(&destinationFloorBut[i],&QPushButton::clicked,[=](){
                destinationFloorButFun(i+1);
            });
        }
        else
        {
            destinationFloorBut[i].setParent(this);
            destinationFloorBut[i].setGeometry(begin_x+80,begin_y+630-(int)(i/2)*40,40,35);
            destinationFloorBut[i].setText(QString::number(i+1));
            connect(&destinationFloorBut[i],&QPushButton::clicked,[=](){
                destinationFloorButFun(i+1);
            });
        }
    }

    // 电梯外部按钮
    for(int i=13;i>0;i--)
    {
        FxUp[i-1].setParent(this);
        FxUp[i-1].setGeometry(begin_x+357,begin_y+158+(13-i)*40,25,25);
        FxUp[i-1].setText("上");
        connect(&FxUp[i-1],&QPushButton::clicked,[=](){
            upRequestFun(i);
        });
        FxDown[i-1].setParent(this);
        FxDown[i-1].setGeometry(begin_x+402,begin_y+158+(13-i)*40,25,25);
        FxDown[i-1].setText("下");
        connect(&FxDown[i-1],&QPushButton::clicked,[=](){
            downRequestFun(i);
        });
    }

    //电梯外部等待人数显示
    for(int i=13;i>0;i--)
    {
        waitingUpNum[i-1].setParent(this);
        waitingUpNum[i-1].setGeometry(begin_x+387,begin_y+158+(13-i)*40,25,25);
        waitingUpNum[i-1].setFont(QFont("宋体",10));
        waitingUpNum[i-1].setStyleSheet("color:black;");
        waitingUpNum[i-1].setText("0");
        waitingDownNum[i-1].setParent(this);
        waitingDownNum[i-1].setGeometry(begin_x+432,begin_y+158+(13-i)*40,25,25);
        waitingDownNum[i-1].setFont(QFont("宋体",10));
        waitingDownNum[i-1].setStyleSheet("color:black;");
        waitingDownNum[i-1].setText("0");
    }


    // 超载指示灯
    fullLoadIndicator.setParent(this);
    fullLoadIndicator.setGeometry(begin_x+30,begin_y+87,40,40);
    fullLoadIndicator.setFixedSize(40,40);
    fullLoadIndicator.raise();
    fullLoadIndicator.setEnabled(false);

    // 轿厢内部楼层指示
    floorIndicator.setParent(this);
    floorIndicator.setDigitCount(2);
    floorIndicator.setSegmentStyle(QLCDNumber::Flat);
    floorIndicator.setStyleSheet("border:1px solid red;color:red;background:black");
    floorIndicator.setGeometry(begin_x+104,begin_y+90,50,40);
    // 轿厢外部楼层指示
    floorIndicatorOut.setParent(this);
    floorIndicatorOut.setDigitCount(2);
    floorIndicatorOut.setSegmentStyle(QLCDNumber::Flat);
    floorIndicatorOut.setStyleSheet("border:1px solid red;color:red;background:black");
    floorIndicatorOut.setGeometry(begin_x+366,begin_y+90,50,40);

    // 门开关状态标签
    doorStatus.setParent(this);
    doorStatus.setGeometry(begin_x+60,begin_y+145,120,35);
    doorStatus.setFont(QFont("宋体",16,QFont::Bold));

    //电梯运行方向指示
    up_indoor.setParent(this);
    up_indoor.setGeometry(begin_x+104,begin_y+60,18,26);
    down_indoor.setParent(this);
    down_indoor.setGeometry(begin_x+136,begin_y+60,18,26);
    up_outdoor.setParent(this);
    up_outdoor.setGeometry(begin_x+370,begin_y+60,18,26);
    down_outdoor.setParent(this);
    down_outdoor.setGeometry(begin_x+402,begin_y+60,18,26);

    // 运行动画中轿厢位置显示
    for(int i=0;i<2;i++)
    {
        carLocation[i].setParent(this);
        carLocation[i].setText(" ");
        carLocation[i].setStyleSheet("QLabel{border:2px solid black;}");
    }
    carLocation[0].setGeometry(begin_x+215,begin_y+672-40*getCurrentFloor()-runTimeBase*2.67,10,39);
    carLocation[1].setGeometry(begin_x+295,begin_y+672-40*getCurrentFloor()-runTimeBase*2.67,10,39);

//    // 如果按钮组内ID为0的单选按钮被选中
//    if(m_btnGroup1->checkedId() == 0)
//    {

    //    }
}

void Elevator::showDirection()
{
    //电梯运行方向指示
    if(getDirection() == direction_stop)
    {
        up_indoor.setPixmap(up_gray);
        down_indoor.setPixmap(down_gray);
        up_outdoor.setPixmap(up_gray);
        down_outdoor.setPixmap(down_gray);
    }
    else if(getDirection() == direction_down)
    {
        up_indoor.setPixmap(up_gray);
        down_indoor.setPixmap(down_red);
        up_outdoor.setPixmap(up_gray);
        down_outdoor.setPixmap(down_red);
    }
    else if(getDirection() == direction_up)
    {
        up_indoor.setPixmap(up_red);
        down_indoor.setPixmap(down_gray);
        up_outdoor.setPixmap(up_red);
        down_outdoor.setPixmap(down_gray);
    }
}

void Elevator::showCarLocation()
{
    // 电梯轿厢位置指示
    int i = getCurrentFloor();
    if(getDirection()==direction_down)
    {
        carLocation[0].setGeometry(begin_x+215,begin_y+672-40*i+runTimeBase*2.67,10,39);
        carLocation[1].setGeometry(begin_x+295,begin_y+672-40*i+runTimeBase*2.67,10,39);
    }
    else
    {
        carLocation[0].setGeometry(begin_x+215,begin_y+672-40*i-runTimeBase*2.67,10,39);
        carLocation[1].setGeometry(begin_x+295,begin_y+672-40*i-runTimeBase*2.67,10,39);
    }

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

int Elevator::getInDoorStatus() const
{
    return inDoorStatus;
}

void Elevator::setInDoorStatus(int newInDoorStatus)
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
    showDirection();
}

void Elevator::setDestinationFloor(int destination_Floor)
{
    destinationFloor[destination_Floor] = 1;
}

void Elevator::resetDestinationFloor(int destination_Floor)
{
    destinationFloor[destination_Floor] = 0;
}

bool Elevator::getDestinationFloor(int destination_Floor)
{
    return destinationFloor[destination_Floor];
}

void Elevator::setupRequest(int request_floor)
{
    upRequest[request_floor] = 1;
}

void Elevator::resetupRequest(int request_floor)
{
    upRequest[request_floor] = 0;
}

bool Elevator::getupRequest(int request_floor)
{
    return upRequest[request_floor];
}

void Elevator::setdownRequest(int request_floor)
{
    downRequest[request_floor] = 1;
}

void Elevator::resetdownRequest(int request_floor)
{
    downRequest[request_floor] = 0;
}

bool Elevator::getdownRequest(int request_floor)
{
    return downRequest[request_floor];
}


void Elevator::handleCartoon() //100ms一次
{
    if(getInDoorStatus() == doorStatus_opening)
        doorOffset -= 5;
    //else if(getInDoorStatus() == doorStatus_closing)
    else
        doorOffset += 5;
    update();//wocao 这个不能删除
}

void Elevator::flashTimeBase()
{
    if(flashFlag)
        flashFlag=false;
    else
        flashFlag=true;
}

void Elevator::runTimeBaseFun()
{
#ifdef TEST
    runTimeBase +=5;//测试
#else
    //runTimeBase += 1;
#endif
    showCarLocation();
}

void Elevator::openDoorButFun()
{
    if(getInDoorStatus()== doorStatus_closing)
        setInDoorStatus(doorStatus_opening);
}

void Elevator::closeDoorButFun()
{
    if(getInDoorStatus()== doorStatus_opened)
#ifdef TEST
        doorOffset = 40;
#else
        doorOffset = 140;
#endif
}

void Elevator::leaveNumConfirm(void)  //按下下来乘客数后点击确认按钮后调用的函数
{
    if(getInDoorStatus() == doorStatus_opened)
    {

    }
}

void Elevator::destinationFloorButFun(int pushedBut)  // 轿厢内得人按下楼层的楼层的记录
{
    setDestinationFloor(pushedBut);
    destinationFloorBut[pushedBut-1].setStyleSheet("QPushButton{border:4px inset rgb(255,99,65);}");
    if(getDirection() == direction_stop)
    {
        if(pushedBut-currentFloor>0) //目的楼层在上方
        {
            setDirection(direction_up);
        }
        else if(pushedBut-currentFloor<0) //目的楼层在下方
        {
            setDirection(direction_down);
        }
        else if(getInDoorStatus()==doorStatus_closed)  //就在此楼层 并且门关着
        {
            setInDoorStatus(doorStatus_opening);  //开门
            destinationFloorBut[pushedBut-1].setStyleSheet("");
            FxUp[pushedBut-1].setStyleSheet("");
            FxDown[pushedBut-1].setStyleSheet("");
            resetDestinationFloor(pushedBut);
        }
    }
}

void Elevator::upRequestFun(int requestFool)
{
    setupRequest(requestFool);
    FxUp[requestFool-1].setStyleSheet("QPushButton{border:3px outset rgb(255,99,65);}");
    if(getDirection() == direction_stop)
    {
        if(requestFool-currentFloor>0) //目的楼层在上方
        {
            setDirection(direction_up);
        }
        else if(requestFool-currentFloor<0) //目的楼层在下方
        {
            setDirection(direction_down);
        }
        else if(getInDoorStatus()==doorStatus_closed)  //就在此楼层 并且门关着
        {
            setInDoorStatus(doorStatus_opening);  //开门
            destinationFloorBut[requestFool-1].setStyleSheet("");
            FxUp[requestFool-1].setStyleSheet("");
            resetupRequest(requestFool);
        }
    }
}

void Elevator::downRequestFun(int requestFool)
{
    setdownRequest(requestFool);
    FxDown[requestFool-1].setStyleSheet("QPushButton{border:3px outset rgb(255,99,65);}");
    if(getDirection() == direction_stop)
    {
        if(requestFool-currentFloor>0) //目的楼层在上方
        {
            setDirection(direction_up);
        }
        else if(requestFool-currentFloor<0) //目的楼层在下方
        {
            setDirection(direction_down);
        }
        else if(getInDoorStatus()==doorStatus_closed)  //就在此楼层 并且门关着
        {
            setInDoorStatus(doorStatus_opening);  //开门
            destinationFloorBut[requestFool-1].setStyleSheet("");
            FxDown[requestFool-1].setStyleSheet("");
            resetdownRequest(requestFool);
        }
    }
}

void Elevator::workControl()  // 电梯运行控制
{
    desFloorAndDir temp;
    // 确定电梯是否要运行 门关好了并且有运行需求了才会运行
    if(getInDoorStatus() == doorStatus_closed&&getDirection()!=direction_stop)
        runTimeBaseTimer->start(runTimerCycle);

    //确定电梯现在的位置
    if(getDirection() == direction_stop)
    {// 啥也不干

    }
    else if(getDirection() == direction_up)
    {
        if(runTimeBase >= 15) //运行了三秒钟了到达了下一个楼层
                {
                runTimeBase = 0;
                setCurrentFloor(getCurrentFloor()+1);
                int a = getCurrentFloor();
        //        FxDown[a-1].setStyleSheet("");
        //        resetdownRequest(a);
                if(getDestinationFloor(a)||getupRequest(a)) //当前楼层为目的楼层
                {
                    setInDoorStatus(doorStatus_opening);
                    destinationFloorBut[a-1].setStyleSheet("");
                    FxUp[a-1].setStyleSheet("");
                    resetDestinationFloor(a);
                    resetupRequest(a);
                    runTimeBaseTimer->stop();
                }
                if(demandQuery(direction_up))
                {

                }
                else //没有顺道的请求
                {
                    runTimeBaseTimer->stop();
                    setDirection(direction_stop);
                    temp = OthersDemandQuery(direction_up);
                    if(temp.destinationStr==a) //目标是当前楼层
                    {
                        setInDoorStatus(doorStatus_opening);
                        destinationFloorBut[a-1].setStyleSheet("");
                        resetDestinationFloor(a);
                        FxDown[a-1].setStyleSheet("");
                        resetdownRequest(a);
                        temp = OthersDemandQuery(direction_up);
                        setDirection(temp.directionStr);
                        runTimeBaseTimer->stop();
                    }
                    if(temp.directionStr)//有运行需求
                    {
                        setDirection(temp.directionStr);
                        if(temp.directionStr==a)
                        {
                            //setInDoorStatus(doorStatus_opening);
                            //destinationFloorBut[a-1].setStyleSheet("");
                            if(temp.directionStr==direction_down)
                            {

                                FxDown[a-1].setStyleSheet("");
                                resetdownRequest(a);
                            }
                            else if(temp.directionStr==direction_up)
                            {
                                FxUp[a-1].setStyleSheet("");
                                resetupRequest(a);
                            }
                        }
                        runTimeBaseTimer->stop();
                    }
                }
    }
    }
//        if(runTimeBase >= 15) //运行了三秒钟了到达了下一个楼层
//        {
//            runTimeBase = 0;
//            setCurrentFloor(getCurrentFloor()+1);
//            int a = getCurrentFloor();

//            if(!demandQuery(direction_up)) //没有顺道的请求
//            {
//                temp = OthersDemandQuery(direction_up);
//                if(temp.destinationStr==a&&temp.directionStr==direction_stop) //目标是当前楼层
//                {
//                    setInDoorStatus(doorStatus_opening);
//                    destinationFloorBut[a-1].setStyleSheet("");
//                    FxUp[a-1].setStyleSheet("");
//                    resetupRequest(a);
//                    resetDestinationFloor(a);
//                    temp = OthersDemandQuery(direction_up);
//                    setDirection(temp.directionStr);
//                    runTimeBaseTimer->stop();
//                }
//                if(temp.directionStr)//有运行需求
//                {
//                    setInDoorStatus(doorStatus_opening);
//                    destinationFloorBut[a-1].setStyleSheet("");
//                    if(temp.directionStr==direction_down)
//                    {
//                        FxDown[a-1].setStyleSheet("");
//                        resetdownRequest(a);
//                    }
//                    else if(temp.directionStr==direction_up)
//                    {
//                        FxUp[a-1].setStyleSheet("");
//                        resetupRequest(a);
//                    }
//                    resetDestinationFloor(a);
//                    runTimeBaseTimer->stop();
//                }
//            }


////            else if()  //有其他运行请求
////            {
////                int direction_temp = OthersDemandQuery(direction_up);
////                if(direction_temp==direction_up)
////                {
////                    set
////                }
////            }
////            else //没有运行需求
////            {
////                runTimeBaseTimer->stop();
////            }

//            if(getDestinationFloor(a)||getupRequest(a)) //当前楼层为目的楼层
//            {
//                setInDoorStatus(doorStatus_opening);
//                destinationFloorBut[a-1].setStyleSheet("");
//                FxUp[a-1].setStyleSheet("");
//                resetDestinationFloor(a);
//                resetupRequest(a);
//                runTimeBaseTimer->stop();
//            }
////            else if(OthersDemandQuery(direction_up) == direction_down&&getdownRequest(a))
////            {

////            }
///
//        }
//    }


    else if(getDirection() == direction_down)
    {
        if(runTimeBase >= 15) //运行了三秒钟了到达了下一个楼层
        {
        runTimeBase = 0;
        setCurrentFloor(getCurrentFloor()-1);
        int a = getCurrentFloor();
//        FxDown[a-1].setStyleSheet("");
//        resetdownRequest(a);
        if(getDestinationFloor(a)||getdownRequest(a)) //当前楼层为目的楼层
        {
            setInDoorStatus(doorStatus_opening);
            destinationFloorBut[a-1].setStyleSheet("");
            FxDown[a-1].setStyleSheet("");
            resetDestinationFloor(a);
            resetdownRequest(a);
            runTimeBaseTimer->stop();
        }
        if(demandQuery(direction_down))
        {

        }
        else //没有顺道的请求
        {
            runTimeBaseTimer->stop();
            setDirection(direction_stop);
            temp = OthersDemandQuery(direction_down);
            if(temp.destinationStr==a) //目标是当前楼层
            {
                setInDoorStatus(doorStatus_opening);
                destinationFloorBut[a-1].setStyleSheet("");
                resetDestinationFloor(a);
                FxUp[a-1].setStyleSheet("");
                resetupRequest(a);
                temp = OthersDemandQuery(direction_down);
                setDirection(temp.directionStr);
                runTimeBaseTimer->stop();
            }
            if(temp.directionStr)//有运行需求
            {
                setDirection(temp.directionStr);
                if(temp.directionStr==a)
                {
                    setInDoorStatus(doorStatus_opening);
                    destinationFloorBut[a-1].setStyleSheet("");
                    if(temp.directionStr==direction_down)
                    {
                        setDirection(temp.directionStr);
                        FxDown[a-1].setStyleSheet("");
                        resetdownRequest(a);
                    }
                    else if(temp.directionStr==direction_up)
                    {
                        setDirection(temp.directionStr);
                        FxUp[a-1].setStyleSheet("");
                        resetupRequest(a);
                    }
                }

                runTimeBaseTimer->stop();
            }
        }
}
//        if(runTimeBase >= 15) //运行了三秒钟了到达了下一个楼层
//        {
//            runTimeBase = 0;
//            setCurrentFloor(getCurrentFloor()-1);
//            if(!demandQuery(direction_down)) //没有请求了
//            {
//                runTimeBaseTimer->stop();
//                setDirection(direction_stop);
//            }
//            int a = getCurrentFloor();
//            if(getDestinationFloor(a)||getdownRequest(a)) //当前楼层为目的楼层
//            {
//                setInDoorStatus(doorStatus_opening);
//                destinationFloorBut[a-1].setStyleSheet("");
//                FxDown[a-1].setStyleSheet("");
//                resetDestinationFloor(a);
//                resetdownRequest(a);
//                runTimeBaseTimer->stop();
//            }
//        }
    }
}

/**
 * @brief 查询是否有顺路的运行需求
 *
 * @param direction 查询的方向 direction_up direction_down
 * @param 返回值 0:No Query;  1:have Query
 *
 */
bool Elevator::demandQuery(int direction)
{
    if(direction == direction_up)  //查询上行方向
    {
        // 查询目的楼层
        for(int i=getCurrentFloor()+1;i<=maxFloor;i++)
        {
            if(getDestinationFloor(i) == 1||getupRequest(i)==1)
                return 1;
        }
    }
    else if(direction == direction_down)  //查询上行方向
    {
        // 查询目的楼层
        for(int i=getCurrentFloor()-1;i>=minFloor;i--)
        {
            if(getDestinationFloor(i)||getdownRequest(i))
                return 1;
        }
//        // 查询请求楼层
//         for(int i=getCurrentFloor()-1;i>=minFloor;i--)
//        {
//            if(getdownRequest(i))
//                return 1;
//        }
    }

    return 0;
}

/**
 * @brief 查询其他低优先级的任务，返回运行方向
 *
 * @param direction 当前的方向 direction_up direction_down
 * @param 返回值 direction_up direction_down direction_stop
 *
 */
struct Elevator::desFloorAndDir Elevator::OthersDemandQuery(int direction)
{
    desFloorAndDir temp;
    int current = getCurrentFloor();
    int destinationFloor_temp=0;
    if(direction==direction_up)
    {
        for(int i = 1;i<=maxFloor;i++)  //以最高的向下呼叫或者目标所在楼层为电梯当前目标楼层。
        {
            if(getdownRequest(i)||getDestinationFloor(i))  //如果有就往上走
            {
                destinationFloor_temp = i; //记录目前最高的向下或者目的楼层
            }

        }
        if(destinationFloor_temp) //有需求
        {
            temp.destinationStr = destinationFloor_temp;
            if(current-destinationFloor_temp>0)
                temp.directionStr = direction_down;
            else if(current-destinationFloor_temp<0)
                temp.directionStr = direction_up;

            return temp;
        }
        for(int i=13;i>=minFloor;i--)//以最低的向上呼叫所在楼层为电梯当前的目标楼层。
        {
            if(getupRequest(i))
            {
                destinationFloor_temp = i; //记录目前最高的向下或者目的楼层
            }

        }
        if(destinationFloor_temp) //有需求
        {
            temp.destinationStr = destinationFloor_temp;
            if(current-destinationFloor_temp>0)
                temp.directionStr = direction_down;
            else if(current-destinationFloor_temp<0)
                temp.directionStr = direction_up;

            return temp;
        }

//        if(getdownRequest(current))  //查看当前楼层是否有向下的需求
//        {
//            setDirection(direction_down_F);
//            FxDown[current-1].setStyleSheet("");
//            //resetdownRequest(current);
//            setInDoorStatus(doorStatus_opening);
//            //return direction_suspend;
//        }
//        for(int i=current-1;i>=minFloor;i--)  //查看下面是否有需求
//        {
//            if(getdownRequest(i)||getupRequest(i)||getDestinationFloor(i))
//                return direction_down_F;
//        }
    }
    else if(direction==direction_down)
    {
        for(int i=13;i>=minFloor;i--)//以最低的向上呼叫或者目标所在楼层为电梯当前的目标楼层。
        {
            if(getupRequest(i)||getDestinationFloor(i))
            {
                destinationFloor_temp = i; //记录目前最高的向下或者目的楼层
            }

        }
        if(destinationFloor_temp) //有需求
        {
            temp.destinationStr = destinationFloor_temp;
            if(current-destinationFloor_temp>0)
                temp.directionStr = direction_down;
            else if(current-destinationFloor_temp<0)
                temp.directionStr = direction_up;

            return temp;
        }
        for(int i = 1;i<=maxFloor;i++)  //以最高的向下呼叫为电梯当前目标楼层。
        {
            if(getdownRequest(i))  //
            {
                destinationFloor_temp = i; //记录目前最高的向下或者目的楼层
            }

        }
        if(destinationFloor_temp) //有需求
        {
            temp.destinationStr = destinationFloor_temp;
            if(current-destinationFloor_temp>0)
                temp.directionStr = direction_down;
            else if(current-destinationFloor_temp<0)
                temp.directionStr = direction_up;

            return temp;
        }


//       //
//        for(int i=current-1;i>=minFloor;i--)  //查看下面有没有向上的需求
//        {
//            if(getupRequest(i))  //如果有就往下走
//                return direction_down_F;
//        }
//        if(getupRequest(current))  //查看当前楼层是否有向上的需求
//        {
//            setDirection(direction_up);
//            FxUp[current-1].setStyleSheet("");
//            //resetupRequest(current);
//            setInDoorStatus(doorStatus_opening);
//            return direction_up_F;
//        }
//        for(int i = current+1;i<=maxFloor;i++)  //查看上面有没有向下的需求
//        {
//            if(getdownRequest(i)||getupRequest(i)||getDestinationFloor(i))
//                return direction_up_F;
//        }
    }

    return temp;
}

