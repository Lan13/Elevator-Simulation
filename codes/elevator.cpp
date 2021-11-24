#include"elevator.h"
#include"mainwindow.h"
#include<QDebug>
#include<algorithm>
#include<cmath>

bool MainWindow::timeUp(Elevator e)
{
    if(e->State==Idle && e->floor==1)
        return false;
    if(e->statetime<=0)
        return true;
    return false;
}

bool MainWindow::noEnter(Elevator e)
{
    if(e->State==GoingUp && waitqueue[0][e->floor].clientcount==0)
        return true;
    else if(e->State==GoingDown && waitqueue[1][e->floor].clientcount==0)
        return true;
    return false;
}

bool MainWindow::noLeave(Elevator e)
{
    if(e->callcar[e->floor]==0)
        return true;
    else if(e->State==GoingUp && sizeStack(e->passengerstack[e->floor])==0)
        return true;
    else if(e->State==GoingDown && sizeStack(e->passengerstack[e->floor])==0)
        return true;
    return false;
}

void MainWindow::changeMove(Elevator &e, int k)
{
    switch (e->currentmove)
    {
        case Opening:
            e->currentmove=Opened;
            e->statetime=CLOSE_TEST_TIME;
        break;

        case Opened:
            if((noEnter(e)==true && noLeave(e)==true)||e->clientnum==MAX_CLIENT_NUM)
            {
//                qDebug()<<"Opened测试";
                e->currentmove=Closing;
                e->statetime=DOOR_TIME;
                if(e->State==Idle)
                    e->currentmove=Waiting;
            }
            else if(e->State==Idle)
                e->currentmove=Waiting;
            else
                e->statetime=CLOSE_TEST_TIME;
        break;

        case Closing:
            if(noEnter(e)==true && noLeave(e)==true)
                e->currentmove=Closed;
            else if(e->clientnum<MAX_CLIENT_NUM)
            {
                e->currentmove=Opening;
                e->statetime=DOOR_TIME;
            }
        break;

        case Closed:
            if(e->State==GoingUp)
            {
                int flag=0;
                if(callup[e->floor]==-1)
                {
                    qDebug()<<e->floor<<"层重置callup";
                    callup[e->floor]=0;
                }
                for(int i=e->floor;i<=4;i++)//判断电梯里是否有人需要向上
                {
                    if(e->callcar[i]==1)
                    {
                        e->currentmove=Accelerate;
                        e->statetime=ACCELERATE_TIME;
                        flag=1;
                        break;
                    }
                }
                if(flag==0)//电梯里没有人要向上走，此时电梯应处于等待状态了
                {
                    e->currentmove=Waiting;
                    e->State=Idle;
                }
            }
            else if(e->State==GoingDown)
            {
                int flag=0;
                if(calldown[e->floor]==-1)
                {
                    qDebug()<<e->floor<<"层重置calldown";
                    calldown[e->floor]=0;
                }
                for(int i=e->floor;i>=0;i--)
                {
                    if(e->callcar[i]==1)
                    {
                        e->currentmove=Accelerate;
                        e->statetime=ACCELERATE_TIME;
                        flag=1;
                        break;
                    }
                }
                if(flag==0)
                {
                    e->currentmove=Waiting;
                    e->State=Idle;
                }
            }
            else if(e->State==Idle)
            {
//                qDebug()<<"这里不变吗？？？";
                e->currentmove=Waiting;
            }
        break;

        case Accelerate:
            e->currentmove=Moving;
            if(e->State==GoingUp)
                e->statetime=UP_TIME;
            else if(e->State==GoingDown)
                e->statetime=DOWN_TIME;
        break;

        case Moving:
            if(e->State==GoingUp)
            {
                e->floor++;
                if(stopNextFloor(e)==true)
                {
//                  //qDebug()<<"要停下来";
                    e->currentmove=SlowDown;
                    e->statetime=UP_SLOW_TIME;
                }
                else
                    e->statetime=UP_TIME;
            }
            else if(e->State==GoingDown)
            {
                e->floor--;
                if(stopNextFloor(e)==true)
                {
//                  //qDebug()<<"要停下来";
                    e->currentmove=SlowDown;
                    e->statetime=DOWN_SLOW_TIME;
                }
                else
                    e->statetime=DOWN_TIME;
            }
            showElevatorPosition(e,k,e->floor);
        break;

        case SlowDown:              //即将停下
            e->callcar[e->floor]=0;
            if(e->State==GoingUp)
            {
                int stillgoing=0;
                int highfloorcall=0;
                for(int i=e->floor;i<4;i++)
                {
                    if(e->callcar[i]==1)
                    {
                        stillgoing=1;     //如果电梯还需向上走就置stillgoing=1
                        break;
                    }
                }
                highfloorcall=highFloorCall(e);
                if(stillgoing==0 && highfloorcall==-1 && waitqueue[0][e->floor].clientcount==0)
                    e->State=GoingDown;
            }
            else if(e->State==GoingDown)
            {
                int stillgoing=0;
                int lowfloorcall=0;
                for(int i=e->floor;i>0;i--)
                {
                    if(e->callcar[i]==1)
                    {
                        stillgoing=1;     //如果电梯还需向下走就置stillgoing=1
                        break;
                    }
                }
                lowfloorcall=lowFloorCall(e);
                if(stillgoing==0 && lowfloorcall== -1 && waitqueue[1][e->floor].clientcount==0)
                    e->State=GoingUp;
            }
            e->currentmove=Opening;
            e->statetime=DOOR_TIME;
        break;

        case Waiting:
        break;
    }
}

bool MainWindow::stopNextFloor(Elevator e)
{

    if(e->State==GoingUp)
    {
        if(e->floor==4)
            return true;
        if(callup[e->floor]==1)
            return true;
        if(e->callcar[e->floor]==1)
            return true;
        return false;
    }
    else if(e->State==GoingDown)
    {
        if(e->floor==0)
            return true;
        if(calldown[e->floor]==1)
            return true;
        if(e->callcar[e->floor]==1)
            return true;
        return false;
    }
    else
        return false;
}

int MainWindow::highFloorCall(Elevator e)
{
    for(int j=e->floor;j<=4;j++)
        if(callup[j]==1)
            return j;
    return -1;
}

int MainWindow::lowFloorCall(Elevator e)
{
    for(int j=e->floor;j>=0;j--)
        if(calldown[j]==1)
            return j;
    return -1;
}

int MainWindow::farestFloorCall(Elevator e)
{
    int floor=-1;
    for(int i=e->floor-1,j=e->floor+1;i>=0||j<=4;i--,j++)
    {
        if(i>=0&&(calldown[i]==1||callup[i]==1))
            floor=i;
        if(j<=4&&(calldown[j]==1||callup[j]==1))
            floor=j;
    }
    return floor;
}

void MainWindow::elevatorControler(Elevator e[])
{
    for(int i=0;i<=1;i++)
    {
        if(e[i]->currentmove==Waiting)//&& e[i]->clientnum==0
        {
//            qDebug()<<"再次controler";
            int floor;
            // 如果本层就有乘客
            if(callup[e[i]->floor]==1)
            {
                e[i]->currentmove=Opening;
                callup[e[i]->floor]=-1; //设置-1为响应层，避免两个电梯争抢一个
                e[i]->State=GoingUp;
            }
            else if(calldown[e[i]->floor]==1)
            {
                e[i]->currentmove=Opening;
                calldown[e[i]->floor]=-1;
                e[i]->State=GoingDown;
            }
            else //不然，寻找最远的需要搭乘的乘客
            {
                floor=farestFloorCall(e[i]);
                if(floor==-1)
                    return;
                else if(callup[floor]==1)
                {
                    e[i]->currentmove=Accelerate;
                    //为什么这里要置一个callcar呢,是因为需要让电梯自行走到响应楼层，然后再到响应楼层时，
                    //就可以让乘客进去，否则电梯是不会自行到响应楼层的
                    e[i]->callcar[floor]=1;
                    callup[floor]=-1;
                    if(floor>=e[i]->floor)
                        e[i]->State=GoingUp;
                    else
                        e[i]->State=GoingDown;
                }
                else if(calldown[floor]==1)
                {
                    e[i]->currentmove=Accelerate;
                    e[i]->callcar[floor]=1;
                    calldown[floor]=-1;
                    if(floor>=e[i]->floor)
                        e[i]->State=GoingUp;
                    else
                        e[i]->State=GoingDown;
                }
            }
        }
    }
}
