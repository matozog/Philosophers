#include "Philosopher.h"
#include <unistd.h>
#include <ncurses.h>

mutex mu;
condition_variable cond;


Philosopher::Philosopher()
{
	
}

Philosopher::Philosopher(int _id)
{
	id =_id;
}

void Philosopher::eating()
{
	countEating++;
	actualState = 'E';
	mu.lock();
	if(countEating>levelPriority) levelPriority=countEating;
	priority = levelPriority - levelPriority;
	mu.unlock();
	for(int i=0; i<10 ;i++)
	{
		refreshStatus(i*10);
		usleep(rand()%400000+200000);
	}
	unique_lock<mutex> locker(mu);
	leftFork->setAvailable(true);
	rightFork->setAvailable(true);
	cond.notify_all();
	locker.unlock();
	refreshStatus(0);
}

void Philosopher::philosophizing()
{
	countPhilosophizing++;
	actualState='P';
	for(int i=0; i<10 ;i++)
	{
		refreshStatus(i*10);
		usleep(rand()%400000+300000);
	}
}

void Philosopher::refreshStatus(int percent)
{
	mu.lock();
	mvprintw(2+id*2,49,"%d",countEating);
	mvprintw(2+id*2,31,"%c",actualState);
	mvprintw(2+id*2,34, "%d %s",percent, "%");
	mvprintw(2+id*2,70,"%d",countPhilosophizing);
	if(id==0) mvprintw(2+(amountOfPhilo-1)*2+1,15,"%d", rightFork->getAvailable());
	if(id==amountOfPhilo-1) mvprintw(2+0*2-1,15,"%d", leftFork->getAvailable());
	mvprintw(2+id*2+1,15,"%d", leftFork->getAvailable());
	mvprintw(2+id*2-1,15,"%d", rightFork->getAvailable());
	refresh();
	mu.unlock();
}

void Philosopher::takeFork(mutex *mutx)
{
	actualState = 'W';
	refreshStatus(0);
	usleep(100000);
	waitForEating=true;
	while(waitForEating)
	{
		unique_lock<mutex> locker(mu);
		cond.wait(locker, [&]{return areTwoForks();});
		if(areTwoForks())
		{	
			if(priorityFlag)
			{
				mutx->unlock();
				priorityFlag=false;
			}
			rightFork->setAvailable(false);
			leftFork->setAvailable(false);
			waitForEating=false;
			locker.unlock();
		}
	}
}

bool Philosopher::areTwoForks()
{
	return rightFork->getAvailable() && leftFork->getAvailable();
}

int Philosopher::getID()
{
	return id;
}

void Philosopher::setLeftFork(Fork *Left)
{
	leftFork = Left;
}

void Philosopher::setRightFork(Fork *Right)
{
	rightFork = Right;
}

bool Philosopher::isHungry()
{
	return waitForEating;
}