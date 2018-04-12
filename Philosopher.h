#pragma once
#include <iostream>
#include <condition_variable>
#include "Fork.h"
#include <mutex>

using namespace std;

static int amountOfPhilo=5;
static int levelPriority=0;

class Philosopher{

bool waitForEating = false;
int id, countEating=0, countPhilosophizing=0, priority=0;
Fork *leftFork, *rightFork;
bool priorityFlag=false;
char actualState='W';

public:

Philosopher();
Philosopher(int _id);
void run();
void philosophizing();
void eating();
int getID();
Fork getLeftFork();
Fork getRightFork();
void setLeftFork(Fork *Left);
void setRightFork(Fork *Right);
void simulatingLife();
void takeFork(mutex *mutx);
bool isHungry();
bool areTwoForks();
void refreshStatus(int percent);
int getPriority() { return priority;}
void setPriorityFlag(bool _priority) {priorityFlag=_priority;}

};
