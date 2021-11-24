#include"mainwindow.h"
#include "elevator.h"
#include"stack.h"
#include<cstdlib>

void MainWindow::initStack(Stack &S)
{
    S.base = (Passenger *)malloc(STACK_INIT_SIZE * sizeof(Passenger));
    S.top = S.base;
    S.stacksize = STACK_INIT_SIZE - 1;
}

void MainWindow::pushStack(Stack &S, Passenger e)
{
    *S.top++ = e;
}
void MainWindow::popStack(Stack &S, Passenger &e)
{
    if (S.top == S.base)
        return;
    e=*(--S.top);
}
int MainWindow::sizeStack(Stack &S)
{
    return S.top - S.base;
}
