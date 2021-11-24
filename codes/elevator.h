#ifndef ELEVATOR_H
#define ELEVATOR_H
#include<string>
#include"stack.h"
#include"queue.h"
#define MAX_FLOOR 4
#define MAX_NEXT_CLIENT_TIME 200//300 正常速度
#define CLOSE_TEST_TIME 40      //进出测试时间，若无人进出，则关门
#define DOOR_TIME 20            //开门和关门时间
#define IN_OUT_TIME 25          //每个人进出时间
#define STAY_RETURN_TIME 300    //在某层超过300t，则返回第一层
#define ACCELERATE_TIME 15
#define UP_TIME 51
#define UP_SLOW_TIME 14
#define DOWN_TIME 61
#define DOWN_SLOW_TIME 23
#define MAX_GIVEUP_TIME 500
#define MAX_RUN_TIME 3000
#define MAX_CLIENT_NUM 8

using namespace std;
enum ElevatorMove {Opening,Opened, Closing, Closed, Moving, SlowDown, Waiting, Accelerate};
enum ElevatorState {GoingUp, GoingDown, Idle};

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



#endif // ELEVATOR_H
