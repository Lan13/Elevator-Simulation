#include"elevator.h"
#include"mainwindow.h"
#include "queue.h"
#include<cstdlib>

void MainWindow::initQueue(PassengerQueue &Q)
{
    Q.front=Q.rear=(QueuePtr)malloc(sizeof(QNode));
    Q.front->next=NULL;
    Q.front->data = NULL;
    Q.clientcount = 0;
}

void MainWindow::destroyQueue(PassengerQueue &Q)
{
    while(Q.front)
    {
        Q.rear=Q.front->next;
        free(Q.front);
        Q.front=Q.rear;
    }
}

int MainWindow::enQueue(PassengerQueue &Q,Passenger e)
{
    QueuePtr p =(QueuePtr)malloc(sizeof(QNode));
    p->data=e;
    p->next=NULL;
    Q.rear->next=p;
    Q.rear=p;
    Q.clientcount++;
    return 1;
}

int MainWindow::deQueue(PassengerQueue &Q,Passenger &e)
{
    if(Q.front==Q.rear)
        return false;
    QueuePtr p = Q.front->next;
    e = p->data;
    Q.front->next = p->next;
    if(Q.rear==p)
        Q.rear=Q.front;
    free(p);
    Q.clientcount--;
    return true;
}


