# 电梯实验报告

## 实验要求

### 问题描述

设计一个电梯模拟系统。这是一个离散的模拟程序，因为电梯系统是乘客和电梯等“活动体”构成的集合， 虽然它们彼此相互作用，但是他们的行为基本是独立的。在离散的模拟中，以模拟时钟决定每个活动体的动作发生的时刻和顺序，系统在某个模拟瞬间处理有待完成的各种事情，然后把模拟时钟推进到某个动作预定要发生的下一个时刻。

### 基本要求

(1) 模拟某校五层教学搂的电梯系统。该楼有一个自动电梯，能在每层停留。五个楼 层由下至上依次称为地下层、第一层、第二层、第三层和第四层，其中第一层是大楼的进出层，即是电梯的“本垒层” ，电梯“空闲”时，将来到该层候命。

(2) 乘客可随机地进出于任何层。对每个人来说，他有一个能容忍的最长等待时间，一旦等候电梯时间过长，他将放弃。

(3) 模拟时钟从 0 开始。人和电梯的各种动作均要耗费一定的时间单位(简记为 t),比如：

- 有人进出时，电梯每隔 40t测试一次，若无人进出，则关门；

- 关门和开门各需要20t 
- 每个人进出电梯均需要 25t; 
- 如果电梯在某层静止时间超过 300t，则驶回1层候命。

(4) 按时序显示系统状态的变化过程:发生的全部人和电梯的动作序列。

(5) 另外的一些要求：

- 电梯加速时间为15t
- 电梯正常上升时间51t、减速停下时间14t
- 电梯正常下降时间61t、减速停下时间23t

### 测试数据

模拟时钟 Time 的初值为 0,终值可在 500~10000 范围内逐步增加。

## 设计思路

由于电梯系统是乘客和电梯等“活动体”构成的集合， 虽然它们彼此相互作用，但是他们的行为基本是独立的。所以我们分别设计一个电梯对象和乘客对象。并且在两个对象之间建立好联系。

对于乘客，我们需要设计：

- 乘客的出现以及进入等待队列
- 乘客进电梯
- 乘客出电梯
- 乘客放弃等待

对于电梯，我们需要设计：

- 电梯当前的状态、当前的动作
- 测试距离下一个状态的时间
- 改变电梯动作和状态
- 两个电梯之间的联系

对于乘客和电梯之间的联系，我们需要设计：

- 是否有乘客进出电梯
- 电梯在下一层楼是否停下
- 低楼层高楼层是否有人需要坐电梯
- 乘客来临时调度电梯

以及需要设计一些显示电梯信息和位置、乘客等待队列的图形化表示。

**最后实验功能能够满足实验的所有基本要求，并且正确地实现了双梯系统，并且对双梯系统的电梯调用函数做了优化，同时采用了图形化设计。**

## 关键代码讲解

**实验功能能够满足实验的所有基本要求，并且正确地实现了双梯系统，并且对双梯系统的电梯调用函数做了优化，同时采用了图形化设计。**

下面只挑选各文件中一些比较重要的进行讲解

### mainwindow.ui

首先介绍`mainwindow.ui`中各组件的作用和名称：

![](C:\Users\蓝\Desktop\电梯图片\电梯主界面.png)

测试按钮就是开始进行离散事件模拟的的开始按钮。

测试时间输入框是用来输入测试总时间的。

当前时间文本框每10t更新一次时间。

等待乘客文本框中，上面的框代表当层的上行乘客，下面的框代表当层的下行乘客。

<center class="half">
    <img src="C:\Users\蓝\Desktop\电梯图片\名称1.png" width="300" height="350"/>
	<img src="C:\Users\蓝\Desktop\电梯图片\名称2.png" width="300" />
</center>


上述便是设计界面中重要组件的名称。

### elevator.h

```c++
#define MAX_FLOOR 4   				//最高楼层
#define MAX_NEXT_CLIENT_TIME 300	//距离下一个乘客到来的最长时间
#define CLOSE_TEST_TIME 40      	//关门测试时间
#define DOOR_TIME 20            	//开门时间
#define IN_OUT_TIME 25          	//乘客进出时间
#define STAY_RETURN_TIME 300   		//在一层楼停留的最带时间
#define ACCELERATE_TIME 15			//电梯加速时间
#define UP_TIME 51					//电梯上行时间
#define UP_SLOW_TIME 14				//电梯上行减速时间
#define DOWN_TIME 61				//电梯下行时间
#define DOWN_SLOW_TIME 23			//电梯下行减速时间
#define MAX_GIVEUP_TIME 500			//乘客能忍受的最长等待时间
#define MAX_RUN_TIME 10000			//总测试时间
#define MAX_CLIENT_NUM 8			//电梯内同时能够坐的最大人数
```

宏定义中是一些配置数据，是实验要求中定义的一些常量。

```c++
enum ElevatorMove {Opening,Opened, Closing, Closed, Moving, SlowDown, Waiting, Accelerate};
enum ElevatorState {GoingUp, GoingDown, Idle};
```

定义电梯的状态和动作。电梯动作分别为：开门中、已开门、关门中、已关门、楼层间移动、减速（即将停下）、等待、加速移动。电梯状态为：上行、下行、等待。

```c++
typedef struct PassengerNode
{
    int passengerid;
    int infloor;
    int outfloor;
    int giveuptime;
}*Passenger;

typedef struct QNode
{
    Passenger data;
    struct QNode *next;
}QNode, *QueuePtr;

typedef struct
{
    QueuePtr front;
    QueuePtr rear;
    int clientcount;
}PassengerQueue;
```

定义乘客结构体以及乘客的等待队列。乘客结构体中分别为乘客序号、进出楼层以及放弃时间。

```c++
typedef struct
{
    int stacksize;
    Passenger *base;
    Passenger *top;
}Stack;

typedef struct ElevatorNode
{
    int clientnum;
    ElevatorState State;
    int statetime;
    int floor;
    ElevatorMove currentmove;
    int callcar[5];
    Stack passengerstack[5];
    int clientid[8];
}*Elevator;
```

定义电梯结构体以及电梯栈。电梯结构体中分别为乘客数量、电梯状态、当前状态的剩余时间、当前楼层、电梯动作、电梯的目标楼层、每个楼层的乘客栈、电梯内的乘客序号。

### mainwindow.h

```c++
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);	//主窗口的构造函数
    ~MainWindow();
    //图形化相关函数（与课程内容关系不大，不做讲解）
    void showWaitingPassenger(PassengerQueue Q,int floor,int k);	//显示第floor层的k方向的队列（k为0时是上行）
    void showElevatorPosition(Elevator e,int k,int x);		//显示第k个电梯，x为楼层
    void clearElevatorPosition(int k);				//清空第k个电梯上一次的位置
    void popWaitingPassenger(PassengerQueue Q,int floor,int k);		//乘客进入电梯时，重新显示等待队列
    void init();	//初始化所有信息
	//队列函数
    void initQueue(PassengerQueue &Q);
    void destroyQueue(PassengerQueue &Q);
    int enQueue(PassengerQueue &Q,Passenger e);
    int deQueue(PassengerQueue &Q, Passenger &e);
	//栈函数
    void initStack(Stack &S);
    void pushStack(Stack &S, Passenger e);
    void popStack(Stack &S, Passenger &e);
    int sizeStack(Stack &S);
	//乘客函数
    void newPassenger();	//随机生成一个乘客的信息，并且让它进入等待队列，并且定义下一个乘客出现的时间
    bool outPassenger(Elevator &e,int k);	//乘客走出第k个电梯
    bool inPassenger(Elevator &e,int k);	//乘客进入第k个电梯
    void giveupPassenger(PassengerQueue &Q, Elevator E[], int floor);	//第floor层的第一个乘客是否放弃等待
	//电梯函数
    bool timeUp(Elevator e);	//状态时间是否结束
    bool noEnter(Elevator e);	//是否有人进入电梯
    bool noLeave(Elevator e);	//是否有人离开电梯
    void changeMove(Elevator &e,int k);		//改变第k个电梯的动作以及状态
    bool stopNextFloor(Elevator e);		//下一个楼层是否要停下
    int highFloorCall(Elevator e);		//是否有等待乘客在高楼层呼叫电梯
    int lowFloorCall(Elevator e);		//是否有等待乘客在低楼层呼叫电梯
    int farestFloorCall(Elevator e);	//最远的楼层呼叫电梯
    void elevatorControler(Elevator e[]);	//电梯调用函数

    void mySleep(int sectime);		//延时函数
private slots:

    void on_beginbtn_clicked();		//开始离散事件模拟的槽函数

private:
    Ui::MainWindow *ui;
    Elevator e[2];		//两个电梯
    PassengerQueue waitqueue[2][5];		//5层楼的上下行乘客等待队列
    int callup[5],calldown[5],callcar[5];	//各楼层是否有人要上行或下行以及电梯内乘客的目标楼层
    int totalpassenger=0;		//整个离散模拟事件中出现过的总乘客数量
    int nextpassengertime=0;	//整个离散模拟时间的时间
};
```

### client.cpp

```c++
void MainWindow::newPassenger()
{
    Passenger c;
    c = (Passenger)malloc(sizeof(PassengerNode));
    c->passengerid = ++totalpassenger;
//  srand(QTime(0,0,0).secsTo(QTime::currentTime()));
    // 乘客楼层初始化
    c->infloor = rand()%(MAX_FLOOR+1);
    c->outfloor = rand()%(MAX_FLOOR+1);
    while(c->infloor==c->outfloor)
        c->outfloor=rand()%(MAX_FLOOR+1);
    qDebug()<<c->infloor<<c->outfloor;
    // 乘客上下行的判断
    if(c->infloor>c->outfloor)
    {
        enQueue(waitqueue[1][c->infloor],c);
        if(calldown[c->infloor]!=-1)	//-1代表已经有电梯要前往本层，则无需再次呼叫电梯，避免两个电梯抢同个资源
            calldown[c->infloor]=1;
        showWaitingPassenger(waitqueue[1][c->infloor],c->infloor,1);
    }
    else
    {
        enQueue(waitqueue[0][c->infloor],c);
        if(callup[c->infloor]!=-1)	//-1代表已经有电梯要前往本层，则无需再次呼叫电梯，避免两个电梯抢同个资源
            callup[c->infloor]=1;
        showWaitingPassenger(waitqueue[0][c->infloor],c->infloor,0);
    }
    // 下一个乘客出现的时间
    nextpassengertime = rand()%MAX_NEXT_CLIENT_TIME;
    while(nextpassengertime<=150)      
        nextpassengertime=rand()%MAX_NEXT_CLIENT_TIME;
    // 本乘客的放弃乘坐时间
    c->giveuptime = rand()%MAX_GIVEUP_TIME;
    while(c->giveuptime<=250)
        c->giveuptime=rand()%MAX_GIVEUP_TIME;
}
```

### elevator.cpp

电梯动作以及状态改变函数`changeMove`是一个很重要的部分，它里面包含了在离散事件模拟中可能出现的各种情况，以及定义了在各种情况下所对应的动作状态转化。

```c++
void MainWindow::changeMove(Elevator &e, int k)
{
    switch (e->currentmove)		//判断电梯当前的动作
    {
        case Opening:
            e->currentmove=Opened;	//下一个状态是已开门
            e->statetime=CLOSE_TEST_TIME; //状态改变时间为测试是否有人进出
        break;

        case Opened:
            if((noEnter(e)==true && noLeave(e)==true)||e->clientnum==MAX_CLIENT_NUM)//如果没有人进出或电梯满员
            {
//                qDebug()<<"Opened测试";
                e->currentmove=Closing;	//关门中
                e->statetime=DOOR_TIME;	//关门时间
                if(e->State==Idle)		//如果状态是等待状态
                    e->currentmove=Waiting;		//电梯动作也为等待
            }
            else if(e->State==Idle)
                e->currentmove=Waiting;
            else						//如果还有人进出
                e->statetime=CLOSE_TEST_TIME;	//时间保持为开关门测试时间
        break;

        case Closing:
            if(noEnter(e)==true && noLeave(e)==true)	//在关门中还是没有乘客要进出
                e->currentmove=Closed;			//确认关门
            else if(e->clientnum<MAX_CLIENT_NUM)	//如果有人要进出且电梯没有满员
            {
                e->currentmove=Opening;			//开门中
                e->statetime=DOOR_TIME;			//开门时间
            }
        break;

        case Closed:
            if(e->State==GoingUp)
            {
                int flag=0;
                if(callup[e->floor]==-1)	//前往呼叫楼层
                {
                    qDebug()<<e->floor<<"层重置callup";
                    callup[e->floor]=0;		//重置呼叫楼层
                }
                for(int i=e->floor;i<=4;i++)	//判断电梯里是否有人需要向上
                {
                    if(e->callcar[i]==1)
                    {
                        e->currentmove=Accelerate;	//电梯转为加速
                        e->statetime=ACCELERATE_TIME;
                        flag=1;
                        break;
                    }
                }
                if(flag==0)		//电梯里没有人要向上走，此时电梯应处于等待状态了
                {
                    e->currentmove=Waiting;
                    e->State=Idle;
                }
            }
            else if(e->State==GoingDown)
            {
                int flag=0;
                if(calldown[e->floor]==-1)	//前往呼叫楼层
                {
                    qDebug()<<e->floor<<"层重置calldown";
                    calldown[e->floor]=0;	//重置呼叫楼层
                }
                for(int i=e->floor;i>=0;i--)	//判断电梯里是否有人需要向下
                {
                    if(e->callcar[i]==1)
                    {
                        e->currentmove=Accelerate;	//电梯转为加速
                        e->statetime=ACCELERATE_TIME;
                        flag=1;
                        break;
                    }
                }
                if(flag==0)		//电梯里没有人要向下走，此时电梯应处于等待状态了
                {
                    e->currentmove=Waiting;
                    e->State=Idle;
                }
            }
            else if(e->State==Idle)
            {
                e->currentmove=Waiting;
            }
        break;

        case Accelerate:
            e->currentmove=Moving;	//下一个动作就是电梯在楼层间移动
            if(e->State==GoingUp)
                e->statetime=UP_TIME;
            else if(e->State==GoingDown)
                e->statetime=DOWN_TIME;
        break;

        case Moving:
            if(e->State==GoingUp)
            {
                e->floor++;		//上行一层
                if(stopNextFloor(e)==true)	//如果下一层要停下来
                {
//                  //qDebug()<<"要停下来";
                    e->currentmove=SlowDown;	//减速
                    e->statetime=UP_SLOW_TIME;
                }
                else
                    e->statetime=UP_TIME;	//否则继续上行
            }
            else if(e->State==GoingDown)
            {
                e->floor--;		//下行一层
                if(stopNextFloor(e)==true)	//如果下一层要停下来
                {
//                  //qDebug()<<"要停下来";
                    e->currentmove=SlowDown;	//减速
                    e->statetime=DOWN_SLOW_TIME;
                }
                else
                    e->statetime=DOWN_TIME;	//否则继续下行
            }
            showElevatorPosition(e,k,e->floor);	//更新电梯位置
        break;

        case SlowDown:              //减速、即将停下
            e->callcar[e->floor]=0;		//到达目标楼层
            if(e->State==GoingUp)
            {
                int stillgoing=0;
                int highfloorcall=0;	//高楼层是否有人呼叫
                for(int i=e->floor;i<4;i++)
                {
                    if(e->callcar[i]==1)
                    {
                        stillgoing=1;     //如果电梯还有乘客要上行就置stillgoing=1
                        break;
                    }
                }
                highfloorcall=highFloorCall(e);
                //如果电梯内没有人要上行了且高楼层没有人呼叫
                if(stillgoing==0 && highfloorcall==-1 && waitqueue[0][e->floor].clientcount==0)
                    e->State=GoingDown;		//电梯下行
            }
            else if(e->State==GoingDown)
            {
                int stillgoing=0;
                int lowfloorcall=0;		//低楼层是否有人呼叫
                for(int i=e->floor;i>0;i--)
                {
                    if(e->callcar[i]==1)
                    {
                        stillgoing=1;     //如果电梯还有乘客要下行就置stillgoing=1
                        break;
                    }
                }
                lowfloorcall=lowFloorCall(e);
                //如果电梯内没有人要下行了且低楼层没有人呼叫
                if(stillgoing==0 && lowfloorcall== -1 && waitqueue[1][e->floor].clientcount==0)
                    e->State=GoingUp;	//电梯上行
            }
            e->currentmove=Opening;		//下个状态时开门
            e->statetime=DOOR_TIME;
        break;

        case Waiting:			//等待动作将由elevatorControler来控制
        break;
    }
}
```

接下来是电梯调用函数，这是双电梯函数调用的重要函数。其中为了避免两个电梯同时前往一个楼层去接乘客（即抢同一个资源），我设计了一个技巧，即设置**响应层**，可以只调用一个电梯前往。当电梯前往时，将呼应楼层的呼叫键设置为-1，这样就会使其”失效“，而只有当这个电梯到达该楼层时，才使这个楼层的呼叫键恢复。这样这个楼层只能和该电梯进行”绑定“，而另一个电梯不会影响它们。这样就实现了双梯系统。

```c++
void MainWindow::elevatorControler(Elevator e[])
{
    for(int i=0;i<=1;i++)
    {
        if(e[i]->currentmove==Waiting)	//只有当电梯是等待动作时才需要进行电梯调用
        {
//            qDebug()<<"再次controler";
            int floor;
            if(callup[e[i]->floor]==1)			//如果电梯当层就有乘客要上行
            {
                e[i]->currentmove=Opening;	//电梯开门
                callup[e[i]->floor]=-1; 	//设置-1为响应层，避免两个电梯争抢一个
                e[i]->State=GoingUp;		//电梯上行
            }
            else if(calldown[e[i]->floor]==1)	//如果电梯当层就有乘客要下行
            {
                e[i]->currentmove=Opening;	//电梯开门
                calldown[e[i]->floor]=-1;	//设置-1为响应层，避免两个电梯争抢一个
                e[i]->State=GoingDown;		//电梯下行
            }
            else //不然，寻找最远的需要搭乘的乘客
            {
                floor=farestFloorCall(e[i]);
                if(floor==-1)	//当前没有乘客呼叫
                    return;
                else if(callup[floor]==1)
                {
                    e[i]->currentmove=Accelerate;
                    //为什么这里要置一个callcar呢,是因为需要让电梯自行走到响应楼层，然后再到响应楼层时，
                    //就可以让乘客进去，否则电梯是不会自行到响应楼层的
                    e[i]->callcar[floor]=1;
                    callup[floor]=-1;	//设置-1为响应层，避免两个电梯争抢一个
                    if(floor>=e[i]->floor)
                        e[i]->State=GoingUp;
                    else
                        e[i]->State=GoingDown;
                }
                else if(calldown[floor]==1)
                {
                    e[i]->currentmove=Accelerate;
                    e[i]->callcar[floor]=1;
                    calldown[floor]=-1;		//设置-1为响应层，避免两个电梯争抢一个
                    if(floor>=e[i]->floor)
                        e[i]->State=GoingUp;
                    else
                        e[i]->State=GoingDown;
                }
            }
        }
    }
}
```

### mainwindow.cpp

```c++
void MainWindow::on_beginbtn_clicked()		//开始离散事件的模拟{    init();		//初始化电梯信息、电梯乘客栈、乘客等待队列    int time=0;		//当前时间    int t[2]={0};	//每个电梯的测试时间    int runtime;	//输入的模拟运行事件    QString move[8]={"Opening","Opened", "Closing", "Closed", "Moving", "SlowDown", "Waiting", "Accelerate"};    QString state[3]={"GoingUp", "GoingDown", "Idle"};    runtime=ui->timeedit->text().toInt();	//获取文本框的运行时间    while(time<runtime)    {        if(nextpassengertime==0)	//当距离下一个乘客来临时时间为0，则生成一个新乘客            newPassenger();        else            nextpassengertime--;	//否则，时间流逝        for(int i=0;i<=4;i++)		//在每个时间段判断所有等待队列中的乘客是否放弃        {            giveupPassenger(waitqueue[0][i],e,i);            giveupPassenger(waitqueue[1][i],e,i);        }        elevatorControler(e);		//调用电梯        for(int i=0;i<2;i++)        {            if(e[i]->currentmove==Opened)	//在电梯打开门的时候            {                if(t[i]==IN_OUT_TIME)                {                    t[i]=0;                    /*如果有人进出，就将测试时间重置*/                    if(outPassenger(e[i],i)==false)                    {                        if(inPassenger(e[i],i)==true)                            e[i]->statetime=CLOSE_TEST_TIME;                    }                    else                        e[i]->statetime=CLOSE_TEST_TIME;                }                else                    t[i]++;		//没有人进出的话就记录时间            }            if(timeUp(e[i])==true)	//当前动作时间已结束            {                changeMove(e[i],i);	//改变电梯当前动作以及状态                //qDebug()<<"电梯"<<i<<"动作"<<move[e[i]->currentmove]<<"状态"<<state[e[i]->State]<<"time"<<e[i]->statetime;            }            else                e[i]->statetime--;	//没有结束时，则时间流逝        }        if(time%10==0)	//每隔10t进行一次检测        {            ui->timetxt->clear();            ui->timetxt->setText(QString::number(time));        }        time++;        mySleep(10);	//延时    }    qDebug()<<"结束";}
```

## 调试分析

实验中遇到的问题有：

1. 图形化界面显示的问题。包括不能及时的更新电梯位置；乘客进入电梯后，乘客等待队列没有及时的将进入电梯的乘客清除等待队列中；新乘客进入队列时没能显示在队伍的末尾。
2. 电梯调用问题。当一个乘客按下呼叫键且两个电梯都在等待状态时，两个电梯会一同前往，浪费了资源。
3. 电梯动作是已开门的情况下，测试时间的重置问题。
4. 为了动态显示电梯运动过程，延时函数的设置问题。
5. 没有正确地重置呼应层，使得电梯不再前往本层响应方向。
6. 当乘客放弃时，也需要重置呼应层。不然电梯到达本层之后，由于没有人要进电梯，则无法重置呼应层。
7. 电梯动作改变时的许多问题。

## 代码测试

**实验功能能够满足实验的所有基本要求，并且正确地实现了双梯系统，并且对双梯系统的电梯调用函数做了优化，同时采用了图形化设计。**

运行时，可以通过改变乘客出现的频率，来达到不同的模拟效果，同时与之对应，乘客放弃的频率也会变化。

运行时，可以通过Qdebug()内容来看一些变化信息，更助于理清楚程序当前进行的状态。

程序刚开始的界面是这样：

<img src="C:\Users\蓝\Desktop\电梯图片\初始化.png" width="400" />

当输入一个运行时间后，程序会开始离散事件模拟：

<img src="C:\Users\蓝\Desktop\电梯图片\运行1.png" width="400" />

<img src="C:\Users\蓝\Desktop\电梯图片\运行2.png" width="400" />

<img src="C:\Users\蓝\Desktop\电梯图片\运行3.png" width="400" />

通过改变乘客出现的频率，可以模拟出高峰时期，此时电梯内的人会多很多。

<img src="C:\Users\蓝\Desktop\电梯图片\运行4.png" width="400" />

最后这是结束状态：

<img src="C:\Users\蓝\Desktop\电梯图片\结束状态.png" width="400" />

## 实验总结

本次实验是2021秋季学期数据结构第一次大作业。主要内容为编写离散模拟事件的代码。而在本次实验中，我选择了编写电梯离散模拟事件。本次实验，其本质上是面向对象的编程，在这次实验中，分别设计电梯和乘客但同时又不忘它们之间的联系，使得程序结构紧凑、简洁。同时还将电梯和栈联系、将乘客与队列联系在一起，将这些内容一起编写，并且穿插图形化设计思路，使得能够满足整个程序的运行正确性以及运行效果。**最后实验功能能够满足实验的所有基本要求，并且正确地实现了双梯系统，并且对双梯系统的电梯调用函数做了优化，同时采用了图形化设计。**通过本次实验，我锻炼了代码设计能力以及思考能力，加深了对栈和队列两种数据结构的理解，并且着手实现栈和队列的代码应用，同时通过图形化设计，使得这个离散模拟事件的过程能够动态地、显式地、易于分析和理解地展示出来，能够在发现问题的过程中不断进行优化更改，使得程序达到更优的状态。

## 附录

**实验功能能够满足实验的所有基本要求，并且正确地实现了双梯系统，并且对双梯系统的电梯调用函数做了优化，同时采用了图形化设计。**

下面是各代码文件介绍：

` Elevator.pro`是项目管理文件，是Qt项目中的默认文件，负责存储与当前项目有关的配置信息，包括：

- 项目中用到了哪些模块
- 项目中包含了哪些源文件，哪些头文件
- 项目最终生成的可执行文件的名称是什么

`mainwindow.ui`是Qt项目界面文件，是一个可视化设计的窗体的定义文件。

`elevator.h`是包含一些乘客等待队列，电梯结构体以及一些配置数据的头文件。

`mainwindow.h`是包含一些函数，设计图形化界面的头文件。

`client.cpp`是设计乘客模块的源文件。

`elevator.cpp`是设计电梯以及电梯和乘客之间联系的模块。

`main.cpp`的主要功能是创建应用程序，创建窗口，显示窗口，并运行应用程序，开始应用程序的消息循环和事件处理。

`mainwindow.cpp`是对电梯和乘客队列进行图形化设计以及初始化的源文件。

`queue.cpp`是定义乘客队列的源文件。

`stack.cpp`是定义电梯栈的源文件。

