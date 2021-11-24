#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<string>
#include"elevator.h"
#include"queue.h"
#include"stack.h"
#include"elevator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showWaitingPassenger(PassengerQueue Q,int floor,int k);
    void showElevatorPosition(Elevator e,int k,int x);
    void clearElevatorPosition(int k);
    void popWaitingPassenger(PassengerQueue Q,int floor,int k);
    void init();

    void initQueue(PassengerQueue &Q);
    void destroyQueue(PassengerQueue &Q);
    int enQueue(PassengerQueue &Q,Passenger e);
    int deQueue(PassengerQueue &Q, Passenger &e);

    void initStack(Stack &S);
    void pushStack(Stack &S, Passenger e);
    void popStack(Stack &S, Passenger &e);
    int sizeStack(Stack &S);

    void newPassenger();
    bool outPassenger(Elevator &e,int k);
    bool inPassenger(Elevator &e,int k);
    void giveupPassenger(PassengerQueue &Q, Elevator E[], int floor);

    bool timeUp(Elevator e);
    bool noEnter(Elevator e);
    bool noLeave(Elevator e);
    void changeMove(Elevator &e,int k);
    bool stopNextFloor(Elevator e);
    int highFloorCall(Elevator e);
    int lowFloorCall(Elevator e);
    int farestFloorCall(Elevator e);
    void elevatorControler(Elevator e[]);

    void mySleep(int sectime);
private slots:


    void on_beginbtn_clicked();

private:
    Ui::MainWindow *ui;
    Elevator e[2];
    PassengerQueue waitqueue[2][5];
    int callup[5],calldown[5],callcar[5];
    int totalpassenger=0;
    int nextpassengertime=0;
};
#endif // MAINWINDOW_H
