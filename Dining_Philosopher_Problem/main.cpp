#include <iostream>
#include <ncurses.h>
#include <mutex>
#include <thread>
#include <time.h>
#include <unistd.h>

using namespace std;

mutex mx;

const int philosophNo = 5;
bool isForkEnable[philosophNo];
int status[philosophNo];
int cycles[philosophNo];
int eating[philosophNo];
int thinking[philosophNo];
bool dinnerClose=false;

void visualization(){
	mx.lock();
	for (int i=0;i<philosophNo;i++)
	{
		attron(COLOR_PAIR(3));
		mvprintw(0,i*5+3,"%d",i);
		attroff(COLOR_PAIR(3));
		mvprintw(1,i*5+3,"%d",status[i]);
		mvprintw(2,i*5+6,"%d",isForkEnable[i]);
		mvprintw(2,0,"%d",isForkEnable[philosophNo-1]);
		mvprintw(i+4,0,"Filozof nr %d jadl %d razy",i,cycles[i]);
		if(status[i]==1){
		attron(COLOR_PAIR(1));
		attron(COLOR_PAIR(1));
		mvprintw(i+4,40," Status jedzenia --> %d%%",eating[i]);
		attroff(COLOR_PAIR(1));
		}
		else
			mvprintw(i+4,40,"                                      ");
		if(status[i]==0){
		attron(COLOR_PAIR(2));
		mvprintw(i+4,75,"Status myslenia--> %d%%",thinking[i]);
		attroff(COLOR_PAIR(2));
		}
		else
			mvprintw(i+4,75,"                                      ");
		attron(COLOR_PAIR(3));
		mvprintw(0,philosophNo*6,"<-- numer filozofa");
		attroff(COLOR_PAIR(3));
		mvprintw(1,philosophNo*6,"<-- jedzacy filozofowie(1 --> jedzacy)");
		mvprintw(2,philosophNo*6,"<-- dostepnosc widelcow(1 --> dostepny)");
		refresh();
	}
	mx.unlock();
}

void takeForks(int id){
	isForkEnable[id]=false;
	isForkEnable[((2*philosophNo+id-1)%philosophNo)]=false;
}

void putForks(int id){
	isForkEnable[id]=true;
	isForkEnable[((2*philosophNo+id-1)%philosophNo)]=true;
}

void dinnerStart(int id) {
	while(!dinnerClose)
	{
		if(getch()==27)
		{
			dinnerClose=true;
			break;
		}
		mx.lock();
		if((isForkEnable[id] && isForkEnable[(((2*philosophNo)+id-1)%philosophNo)]) && 
		  (cycles[id]<=cycles[(2*philosophNo+id-1)%philosophNo]) &&
		  (cycles[id]<=cycles[(2*philosophNo+id+1)%philosophNo]))
		{
			takeForks(id);
			status[id]=1;	
			cycles[id]++;	
		}
		mx.unlock();
		
		eating[id]=0;
		for(int i=1;i<=100;i++)
		{
			usleep(rand() %35000 + 25000);
			eating[id]+=1;
			visualization();
		}
		eating[id]=0;
		clear();
		visualization();

		mx.lock();
		if(status[id]==1)
		{
			putForks(id);
			status[id]=0;		
		}
		mx.unlock();
		
		
		thinking[id]=0;
		for(int i=1;i<=100;i++)
		{
			usleep(rand() %25000+15000);
			thinking[id]+=1;
			visualization();
		}
		
		thinking[id]=0;
		clear();
		visualization();	
	}	
}
int main(){
	srand(time(NULL));
	for(int i=0;i<philosophNo;i++)
	{
		isForkEnable[i]=true;
		status[i]=0;
	}
	int tab[philosophNo];
	thread t[philosophNo];
	initscr();
	nodelay(stdscr,TRUE);

    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	refresh();
	
	for (int i = 0; i < philosophNo; i++)
	{
		t[i] = thread(dinnerStart, i);
	}
	for (int i = 0; i < philosophNo; i++)
	{
		t[i].join();
	}
	endwin();
	return 0;
}