#include"elevator.h"
#include"mainwindow.h"
#include<cstdlib>
#include<QTime>
#include<QtGlobal>

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
        if(calldown[c->infloor]!=-1)
            calldown[c->infloor]=1;
        showWaitingPassenger(waitqueue[1][c->infloor],c->infloor,1);
    }
    else
    {
        enQueue(waitqueue[0][c->infloor],c);
        if(callup[c->infloor]!=-1)
            callup[c->infloor]=1;
        showWaitingPassenger(waitqueue[0][c->infloor],c->infloor,0);
    }
    // 下一个乘客出现的时间
    nextpassengertime = rand()%MAX_NEXT_CLIENT_TIME;
    while(nextpassengertime<=50)       //正常时间 150
        nextpassengertime=rand()%MAX_NEXT_CLIENT_TIME;
    // 本乘客的放弃乘坐时间
    c->giveuptime = rand()%MAX_GIVEUP_TIME;
    while(c->giveuptime<=250)
        c->giveuptime=rand()%MAX_GIVEUP_TIME;
}

bool MainWindow::outPassenger(Elevator &e,int k)//k是电梯编号
{
    Passenger c;
    int id,movepassenger;
    // 判断本层的乘客栈是否为空
    if(sizeStack(e->passengerstack[e->floor])!=0)
    {
        popStack(e->passengerstack[e->floor],c);

        for(id=0;id<=7;id++)
            if(e->clientid[id]==c->passengerid)
                break;
        for(movepassenger=id;movepassenger<7;movepassenger++)
            e->clientid[movepassenger]=e->clientid[movepassenger+1];

        e->clientnum--;
        showElevatorPosition(e,k,e->floor);
        return true;
    }
    else
        return false;
}

bool MainWindow::inPassenger(Elevator &e,int k)
{
    Passenger c;
    if(e->clientnum>=8)
        return false;
    switch (e->State)
    {
        case GoingDown:
            if(waitqueue[1][e->floor].clientcount != 0)
            {
                if(deQueue(waitqueue[1][e->floor],c))
                {
                    pushStack(e->passengerstack[c->outfloor],c);
                    e->callcar[c->outfloor]=1;
                    e->clientid[e->clientnum++]=c->passengerid;
                    popWaitingPassenger(waitqueue[1][e->floor],e->floor,1);
                    showElevatorPosition(e,k,e->floor);
                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        break;
        case GoingUp:
            if(waitqueue[0][e->floor].clientcount != 0)
            {
                if(deQueue(waitqueue[0][e->floor],c))
                {
                    pushStack(e->passengerstack[c->outfloor],c);
                    e->callcar[c->outfloor]=1;
                    e->clientid[e->clientnum++]=c->passengerid;
                    popWaitingPassenger(waitqueue[0][e->floor],e->floor,0);
                    showElevatorPosition(e,k,e->floor);
                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        break;
        case Idle:
//            qDebug()<<"Idle状态inclient会有吗？";
        break;
    }
    return false;
}
//giveup这个函数貌似只能从队列头开始放弃，不可以从后面的人开始放弃
void MainWindow::giveupPassenger(PassengerQueue &Q, Elevator E[], int floor)
{
    Passenger c;
    if(Q.clientcount<=0)
        return;
    else if(Q.front->next->data!=NULL)
        c=Q.front->next->data;
    // 当忍耐时间到了而本层却没有电梯时
    if(E[0]->floor!=floor&&E[1]->floor!=floor&&c->giveuptime<=0)
    {
        //放弃
        deQueue(Q,c);
        int direction;
        if(c->infloor>c->outfloor)
            direction=1;
        else
            direction=0;
        popWaitingPassenger(Q,floor,direction);
        qDebug()<<"乘客"<<c->passengerid<<"放弃";
        // 重置本层为非相应层，否则后面的乘客将不能继续坐电梯了
        if(direction==1&&calldown[floor]==-1)
        {
            qDebug()<<floor<<"乘客放弃calldown重置";
            calldown[floor]=0;
        }
        if(direction==0&&callup[floor]==-1)
        {
            qDebug()<<floor<<"乘客放弃callup重置";
            callup[floor]=0;
        }
    }
    else
        c->giveuptime--;
}
