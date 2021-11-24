#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"elevator.h"
#include<cstdlib>
#include"stack.h"
#include"queue.h"
#include<QDebug>
#include<QTime>

void MainWindow::showWaitingPassenger(PassengerQueue Q, int floor,int k)//k为0向上,k为1向下
{
    QNode *p;
    QString display;
    string client="";
    p=Q.front->next;
    while(p)//既然有分上下行了，这里是否也需要修改？
    {
        if(p->data->infloor>p->data->outfloor)
            client+= "下行乘客"+to_string(p->data->passengerid)+" ";
        else
            client+= "上行乘客"+to_string(p->data->passengerid)+" ";
        p=p->next;
    }
    display = QString::fromStdString(client);
    qDebug()<<display;
    if(k==0)
    {
        switch (floor)
        {
        case 0:
            ui->passenger0up->clear();
            ui->passenger0up->setText(display);
        break;
        case 1:
            ui->passenger1up->clear();
            ui->passenger1up->setText(display);
        break;
        case 2:
            ui->passenger2up->clear();
            ui->passenger2up->setText(display);
        break;
        case 3:
            ui->passenger3up->clear();
            ui->passenger3up->setText(display);
        break;
        case 4:
            ui->passenger4up->clear();
            ui->passenger4up->setText(display);
        break;
        default:
            break;
        }
    }
    else
    {
        switch (floor)
        {
        case 0:
            ui->passenger0down->clear();
            ui->passenger0down->setText(display);
        break;
        case 1:
            ui->passenger1down->clear();
            ui->passenger1down->setText(display);
        break;
        case 2:
            ui->passenger2down->clear();
            ui->passenger2down->setText(display);
        break;
        case 3:
            ui->passenger3down->clear();
            ui->passenger3down->setText(display);
        break;
        case 4:
            ui->passenger4down->clear();
            ui->passenger4down->setText(display);
        break;
        default:
            break;
        }
    }
}
void MainWindow::popWaitingPassenger(PassengerQueue Q, int floor,int k)
{
    QString display;
    if(k==0)
    {
        switch (floor)
        {
        case 0:
            display=ui->passenger0up->toPlainText();
        break;
        case 1:
            display=ui->passenger1up->toPlainText();
        break;
        case 2:
            display=ui->passenger2up->toPlainText();
        break;
        case 3:
            display=ui->passenger3up->toPlainText();
        break;
        case 4:
            display=ui->passenger4up->toPlainText();
        break;
        default:
            break;
        }
    }
    else
    {
        switch (floor)
        {
        case 0:
            display=ui->passenger0down->toPlainText();
        break;
        case 1:
            display=ui->passenger1down->toPlainText();
        break;
        case 2:
            display=ui->passenger2down->toPlainText();
        break;
        case 3:
            display=ui->passenger3down->toPlainText();
        break;
        case 4:
            display=ui->passenger4down->toPlainText();
        break;
        default:
            break;
        }
    }
    QString subdisplay=display.section(' ',1,-1);
    if(k==0)
    {
        switch (floor)
        {
        case 0:
            ui->passenger0up->clear();
            ui->passenger0up->setText(subdisplay);
        break;
        case 1:
            ui->passenger1up->clear();
            ui->passenger1up->setText(subdisplay);
        break;
        case 2:
            ui->passenger2up->clear();
            ui->passenger2up->setText(subdisplay);
        break;
        case 3:
            ui->passenger3up->clear();
            ui->passenger3up->setText(subdisplay);
        break;
        case 4:
            ui->passenger4up->clear();
            ui->passenger4up->setText(subdisplay);
        break;
        default:
            break;
        }
    }
    else
    {
        switch (floor)
        {
        case 0:
            ui->passenger0down->clear();
            ui->passenger0down->setText(subdisplay);
        break;
        case 1:
            ui->passenger1down->clear();
            ui->passenger1down->setText(subdisplay);
        break;
        case 2:
            ui->passenger2down->clear();
            ui->passenger2down->setText(subdisplay);
        break;
        case 3:
            ui->passenger3down->clear();
            ui->passenger3down->setText(subdisplay);
        break;
        case 4:
            ui->passenger4down->clear();
            ui->passenger4down->setText(subdisplay);
        break;
        default:
            break;
        }
    }
}
// k第几个电梯，x第几层
void MainWindow::showElevatorPosition(Elevator e, int k, int x)
{
    QString display;
    string client="";
    for(int i=0;i<e->clientnum;i++)
    {
        if(e->clientid[i]!=0)
            client+= "乘客"+to_string(e->clientid[i])+" ";
    }
    display = QString::fromStdString(client);
    clearElevatorPosition(k);
    switch (k)
    {
    case 0:
        switch(x)
        {
        case 0: ui->first0->setText(display);
                ui->first0->setStyleSheet("background-color: rgb(237, 248, 255);");
            break;
        case 1: ui->first1->setText(display);
                ui->first1->setStyleSheet("background-color: rgb(237, 248, 255);");
            break;
        case 2: ui->first2->setText(display);
                ui->first2->setStyleSheet("background-color: rgb(237, 248, 255);");
            break;
        case 3: ui->first3->setText(display);
                ui->first3->setStyleSheet("background-color: rgb(237, 248, 255);");
            break;
        case 4: ui->first4->setText(display);
                ui->first4->setStyleSheet("background-color: rgb(237, 248, 255);");
            break;
        default: break;
        }
    break;
    case 1:
        switch(x)
        {
        case 0: ui->second0->setText(display);
                ui->second0->setStyleSheet("background-color: rgb(237, 248, 255);");
            break;
        case 1: ui->second1->setText(display);
                ui->second1->setStyleSheet("background-color: rgb(237, 248, 255);");
            break;
        case 2: ui->second2->setText(display);
                ui->second2->setStyleSheet("background-color: rgb(237, 248, 255);");
            break;
        case 3: ui->second3->setText(display);
                ui->second3->setStyleSheet("background-color: rgb(237, 248, 255);");
            break;
        case 4: ui->second4->setText(display);
                ui->second4->setStyleSheet("background-color: rgb(237, 248, 255);");
            break;
        default: break;
        }
    break;
    default: break;
    }
}
void MainWindow::clearElevatorPosition(int k)
{
    if(k==0)
    {
        ui->first0->clear();
        ui->first1->clear();
        ui->first2->clear();
        ui->first3->clear();
        ui->first4->clear();
        ui->first0->setStyleSheet("");
        ui->first1->setStyleSheet("");
        ui->first2->setStyleSheet("");
        ui->first3->setStyleSheet("");
        ui->first4->setStyleSheet("");
    }
    else
    {
        ui->second0->clear();
        ui->second1->clear();
        ui->second2->clear();
        ui->second3->clear();
        ui->second4->clear();
        ui->second0->setStyleSheet("");
        ui->second1->setStyleSheet("");
        ui->second2->setStyleSheet("");
        ui->second3->setStyleSheet("");
        ui->second4->setStyleSheet("");
    }
}
void MainWindow::init()
{
    for(int i=0;i<=4;i++)
    {
        initQueue(waitqueue[0][i]);
        initQueue(waitqueue[1][i]);
    }
    for(int i=0;i<=1;i++)
    {
        e[i]=(Elevator)malloc(sizeof(ElevatorNode));
        e[i]->clientnum=0;
        e[i]->floor=1;
        e[i]->State=Idle;
        e[i]->statetime=0;
        e[i]->currentmove=Waiting;
        for(int j=0;j<5;j++)
        {
            e[i]->callcar[j]=0;
            initStack(e[i]->passengerstack[j]);
        }
        for(int j=0;j<8;j++)
            e[i]->clientid[j]=0;
    }
    for(int i=0;i<=4;i++)
    {
        callup[i]=0;
        calldown[i]=0;
        callcar[i]=0;
    }
    showElevatorPosition(e[0],0,1);
    showElevatorPosition(e[1],1,1);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_beginbtn_clicked()
{
    init();
    int time=0;
    int t[2]={0};
    int runtime;
    QString move[8]={"Opening","Opened", "Closing", "Closed", "Moving", "SlowDown", "Waiting", "Accelerate"};
    QString state[3]={"GoingUp", "GoingDown", "Idle"};
    runtime=ui->timeedit->text().toInt();
    while(time<runtime)
    {
        if(nextpassengertime==0)
            newPassenger();
        else
            nextpassengertime--;
        for(int i=0;i<=4;i++)
        {
            giveupPassenger(waitqueue[0][i],e,i);
            giveupPassenger(waitqueue[1][i],e,i);
        }
        elevatorControler(e);
        for(int i=0;i<2;i++)
        {
            if(e[i]->currentmove==Opened)
            {
                if(t[i]==IN_OUT_TIME)
                {
                    t[i]=0;
                    if(outPassenger(e[i],i)==false)
                    {
                        if(inPassenger(e[i],i)==true)
                            e[i]->statetime=CLOSE_TEST_TIME;
                    }
                    else
                        e[i]->statetime=CLOSE_TEST_TIME;
                }
                else
                    t[i]++;
            }
            if(timeUp(e[i])==true)
            {
                changeMove(e[i],i);
                //qDebug()<<"电梯"<<i<<"动作"<<move[e[i]->currentmove]<<"状态"<<state[e[i]->State]<<"time"<<e[i]->statetime;
            }
            else
                e[i]->statetime--;
        }
        if(time%10==0)
        {
            ui->timetxt->clear();
            ui->timetxt->setText(QString::number(time));
        }
        time++;
        mySleep(10);//30
    }
    qDebug()<<"结束";
}

void MainWindow::mySleep(int sectime)
{
    QTime dietime=QTime::currentTime().addMSecs(sectime);
    while(QTime::currentTime()<dietime)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}
