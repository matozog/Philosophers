#include <iostream>
#include <thread>
#include <unistd.h>
#include "Philosopher.h"
#include "Fork.h"
#include <ncurses.h>
#include <mutex>

using namespace std;

int amountOfThreads=5;
mutex mutx, mt;
bool endProg = false;

void simulatingLife(Philosopher philosopher)
{
	while(!endProg)
	{
		if(getch()=='q')
		{
			endProg=true;
			break;	
		} 
		philosopher.philosophizing();
		mt.lock();
		if(philosopher.getPriority()>2)
		{
		 mutx.lock();
		 philosopher.setPriorityFlag(true);
		}
		mt.unlock();
		philosopher.takeFork(&mutx);
		philosopher.eating();
	}
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	thread *t = new thread[amountOfThreads];
	Philosopher *philosophers = new Philosopher[amountOfThreads];
	Fork *forks = new Fork[amountOfThreads];
	initscr();
	nodelay(stdscr,TRUE);
	start_color();

	for(int i=0; i<amountOfThreads;i++)
	{
		Philosopher philosopher(i);
		philosophers[i] = philosopher;

		Fork fork(i);
		forks[i] = fork;

		philosophers[i].setLeftFork(&forks[i]);
		if(i>0)
		{
			philosophers[i].setRightFork(&forks[i-1]);
		}
		if(i==amountOfThreads-1)
		{
			philosophers[0].setRightFork(&forks[amountOfThreads-1]);
		}

		mvprintw(2+i*2,0,"%s: %d","Thread", i);
	}
	mvprintw(0,14,"%s %s %s", "Fork	 Actual state","	 Eating counter", "   Philosophizing counter");
	refresh();

	for(int i=0 ;i<amountOfThreads;i++) t[i] = thread(simulatingLife,philosophers[i]);

	for(int i=0; i<amountOfThreads;i++)
	{
		t[i].join();
	}
	endwin();

	return 0;
}
