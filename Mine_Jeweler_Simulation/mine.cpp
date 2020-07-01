#include <iostream>
#include <thread>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <mutex>
#include <condition_variable>

using namespace std;
mutex mtx;
condition_variable cvMine, cvForgeResource, cvResource;

const int numMiner = 10;
const int numDriver = 2;
const int numSmith = 8;
const int numJewellerRing = 4;
const int numJewellerNecklace = 4;
const int numJewellerEarring = 4;
const int numTrader=3;

int mineStatus=50;
int digResource=40;
int forgeResourceStatus=0;

int silverBarStatus = 0;
int smallCutDiamondStatus=0;
int bigCutDiamondStatus=0;
int bigGoldBarStatus=0;
int smallGoldBarStatus=0;

int counterMiner[numMiner];

int ringStatus;
int necklaceStatus;
int earringStatus;

int ringStatusTrader[numTrader];
int necklaceStatusTrader[numTrader];
int earringStatusTrader[numTrader];
int cashTrader[numTrader];

int cash;
int extraction=1;
bool endMine = false;

void showStatus(){
    mtx.lock();

    
    for (int i=0;i<150;i++)
    {
    mvprintw(12,25+i,"-");
    }
    for(int i=0;i<45;i++)
    {
        mvprintw(13+i,25,"|");
    }
    for(int i=0;i<45;i++)
    {
        mvprintw(13+i,90,"|");
    }

    mvprintw(13,130,"Produkcja bizuterii"); 
    mvprintw(15,130,"Produkcja pierscionkow"); 
    mvprintw(25,130,"Produkcja naszyjnikow"); 
    mvprintw(35,130,"Produkcja kolczykow"); 
    mvprintw(13,95,"Gotowa bizuteria");     
    mvprintw(21,55,"Sprzedana bizuteria");
    mvprintw(23,47,"   P        N       K         PLN");    


    refresh();	
	mtx.unlock();
}

void showForgeResource(){
    mtx.lock();
    int counter = forgeResourceStatus;
    mvprintw(1,173,"Ilosc zasobow w kuzni -->     ");
    mvprintw(1,173,"Ilosc zasobow w kuzni --> %d",forgeResourceStatus);

    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(counter>0)
            {  
                mvprintw(i+2,j+182,"*");
                counter--;
            }
            else
            {
                mvprintw(i+2,j+182," ");
            }
        }
    }
    refresh();
    mtx.unlock();
}

void showMine(){
    mtx.lock();
    int counter = mineStatus;
    counter*=2;
    mvprintw(1,4,"Stan kopalni -->    ");
    mvprintw(1,4,"Stan kopalni --> %d",mineStatus);
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<20;j++)
        {
            if(counter>0)
            {
                attron(COLOR_PAIR(2));	
                mvprintw(i+2,j+2," ");
                attroff(COLOR_PAIR(2));
                counter--;
            }
            else
            {
                attron(COLOR_PAIR(6));
                mvprintw(i+2,j+2," ");
                attroff(COLOR_PAIR(6));
            } 
        }
    }
    
    refresh();
    mtx.unlock();
    
}

void showMiner(int id, bool work){
    mtx.lock();
    if(work)
    {
        attron(COLOR_PAIR(2));
        mvprintw(13+id,4,"Gornik %d: %d",id, counterMiner[id]);
        attroff(COLOR_PAIR(2));
    }
    else
    {
        mvprintw(13+id,4,"Gornik %d: %d",id, counterMiner[id]);
    }
    refresh();
    mtx.unlock();
}

void showDigResource(){
    mtx.lock();
    int counter = digResource;
    mvprintw(1,27,"Ilosc wydobytych zasobow -->     ");
    mvprintw(1,27,"Ilosc wydobytych zasobow --> %d",digResource);
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(counter>0)
            {  
                mvprintw(i+2,j+29,"*");
                counter--;
            }
            else
            {
                mvprintw(i+2,j+29," ");
            }
        }
    }
    refresh();
    mtx.unlock();
}

void showForge(){
    mtx.lock();
    int counter1 = bigGoldBarStatus;
    int counter2 = smallGoldBarStatus;
    int counter3 = bigCutDiamondStatus;
    int counter4 = smallCutDiamondStatus;
    int counter5 = silverBarStatus;
    mvprintw(15,173,"Duze sztabki zlota-->      ");
    mvprintw(15,173,"Duze sztabki zlota--> %d",bigGoldBarStatus);

    mvprintw(20,173,"Male sztabki zlota -->     ");
    mvprintw(20,173,"Male sztabki zlota --> %d",smallGoldBarStatus);

    mvprintw(25,173,"Duze diamenty-->       ");
    mvprintw(25,173,"Duze diamenty--> %d",bigCutDiamondStatus);

    mvprintw(30,173,"Male diamenty -->      ");
    mvprintw(30,173,"Male diamenty --> %d",smallCutDiamondStatus);

    mvprintw(35,173,"Sztabki srebra -->     ");
    mvprintw(35,173,"Sztabki srebra --> %d",silverBarStatus);
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<25;j++)
        {
            if(counter1>0)
            {  
                attron(COLOR_PAIR(2));	
                mvprintw(i+16,j+175,"D");
                attroff(COLOR_PAIR(2));
                counter1--;
            }
            else
            {
                mvprintw(i+16,j+175," ");
            }
            if(counter2>0)
            {
                attron(COLOR_PAIR(2));
                mvprintw(i+21,j+175,"M");
                attroff(COLOR_PAIR(2));
                counter2--;
            }
            else
            {
                 mvprintw(i+21,j+175," ");
            }
            if(counter3>0)
            {  
                attron(COLOR_PAIR(4));	
                mvprintw(i+26,j+175,"D");
                attroff(COLOR_PAIR(4));
                counter3--;
            }
            else
            {
                mvprintw(i+26,j+175," ");
            }
            if(counter4>0)
            {
                attron(COLOR_PAIR(4));
                mvprintw(i+31,j+175,"M");
                attroff(COLOR_PAIR(4));
                counter4--;
            }
            else
            {
                 mvprintw(i+31,j+175," ");
            }
            if(counter5>0)
            {  
                attron(COLOR_PAIR(6));	
                mvprintw(i+36,j+175,"S");
                attroff(COLOR_PAIR(6));
                counter5--;
            }
            else
            {
                mvprintw(i+36,j+175," ");
            }
        }
    }
    refresh();
    mtx.unlock();
                
}

void showDriverNo1(int id, int position){
    mtx.lock();
    if(position == 0)
    {
        mvprintw(2+id*5,45,"|                      |");
        mvprintw(3+id*5,45,"|                      |");
        mvprintw(4+id*5,45,"|                      |");
        mvprintw(5+id*5,45,"|______________________|");
        mvprintw(6+id*5,45," `(_)(_)`       `(_)(_)");
        
        refresh();
    }
    else
    {
        for(int i=45;i<145;i++)
        {
            for(int j=2;j<=6;j++)
            {
                 mvprintw(j+id*5,i-1,"                                     ");
            }
            attron(COLOR_PAIR(8));
            mvprintw(2+id*5,i,"|**********************|");
            mvprintw(3+id*5,i,"|**********************|");
            mvprintw(4+id*5,i,"|**********************|");
            mvprintw(5+id*5,i,"|======================|");
            mvprintw(6+id*5,i," `(_)(_)`       `(_)(_)");
            attroff(COLOR_PAIR(8));
            refresh();
            usleep(20000);
        }
    }
    mtx.unlock();
}

void showDriverNo2(int id){
    mtx.lock();
    for(int i=145;i>45;i--)
    {
       for(int j=2;j<=6;j++)
        {
            mvprintw(j+id*5,i-1,"                                     ");
        }
        mvprintw(2+id*5,i,"|                      |");
        mvprintw(3+id*5,i,"|                      |");
        mvprintw(4+id*5,i,"|                      |");
        mvprintw(5+id*5,i,"|______________________|");
        mvprintw(6+id*5,i," `(_)(_)`       `(_)(_)");
        refresh();
        usleep(20000);
    } 
    for(int j=2;j<=6;j++)
    {
        mvprintw(j+id*5,45,"                                     ");
    }
    mtx.unlock();
}

void showJewellerRing(int id, int position){
    mtx.lock();
    mvprintw(17+id,125,"Jubiler %d: ",id);
    for(int i=0;i<10;i++)
    {
        mvprintw(17+id,136+i," ");
    }
    for(int i=0;i<position;i++)
    {
        attron(COLOR_PAIR(1));
        mvprintw(17+id,136+i," ");
        attroff(COLOR_PAIR(1));
    }
    char procent = 37;
    mvprintw(17+id,146,"     ");
    mvprintw(17+id,146,"%d %c",position*10,procent);
    refresh();
    mtx.unlock();
}

void showJewellerNecklace(int id, int position){
    mtx.lock();
    mvprintw(27+id,125,"Jubiler %d: ",id);
    for(int i=0;i<10;i++)
    {
        mvprintw(27+id,136+i," ");
    }
    for(int i=0;i<position;i++)
    {
        attron(COLOR_PAIR(3));
        mvprintw(27+id,136+i," ");
        attroff(COLOR_PAIR(3));
    }
    char procent = 37;
    mvprintw(27+id,146,"     ");
    mvprintw(27+id,146,"%d %c",position*10,procent);
    refresh();
    mtx.unlock();
}

void showJewellerEarring(int id, int position){
    mtx.lock();    
    mvprintw(37+id,125,"Jubiler %d: ",id);
    for(int i=0;i<10;i++)
    {
        mvprintw(37+id,136+i," ");
    }
    for(int i=0;i<position;i++)
    {
        attron(COLOR_PAIR(5));
        mvprintw(37+id,136+i," ");
        attroff(COLOR_PAIR(5));
    }
    char procent = 37;
    mvprintw(37+id,146,"     ");
    mvprintw(37+id,146,"%d %c",position*10,procent);
    refresh();
    mtx.unlock();
}

void showRing(){
    mtx.lock();
    mvprintw(15,95,"Pierscionki -->   ", ringStatus);
    mvprintw(15,95,"Pierscionki --> %d", ringStatus);     
    int counter = ringStatus;
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(counter>0)
            {  
                attron(COLOR_PAIR(1));
                mvprintw(i+17,j+95,"P");
                attroff(COLOR_PAIR(1));
                counter--;
            }
            else
            {
                mvprintw(i+17,j+95," ");
            }
        }
    }
    refresh();
    mtx.unlock();
}

void showNecklace(){
    mtx.lock(); 
    mvprintw(25,95,"Naszyjniki -->   ");
    mvprintw(25,95,"Naszyjniki --> %d", necklaceStatus);     
    int counter = necklaceStatus;
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(counter>0)
            {  
                attron(COLOR_PAIR(3));
                mvprintw(i+27,j+95,"N");
                attroff(COLOR_PAIR(3));
                counter--;
            }
            else
            {
                mvprintw(i+27,j+95," ");
            }
        }
    }
    refresh();
    mtx.unlock();
}

void showEarring(){
    mtx.lock(); 
    mvprintw(35,95,"Kolczyki -->   ");
    mvprintw(35,95,"Kolczyki --> %d", earringStatus);     
    int counter = earringStatus;
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(counter>0)
            {  
                attron(COLOR_PAIR(5));
                mvprintw(i+37,j+95,"K");
                attroff(COLOR_PAIR(5));
                counter--;
            }
            else
            {
                mvprintw(i+37,j+95," ");
            }
        }
    }
    refresh();
    mtx.unlock();
}

void showTrader(int id){
    mtx.lock();
    mvprintw(25+id,35,"Klient %d:      %d        %d       %d         %d",id, ringStatusTrader[id], 
    necklaceStatusTrader[id], earringStatusTrader[id], cashTrader[id]);
	refresh();	
	mtx.unlock();
}

void showCash(){
    mtx.lock();
    mvprintw(30,4,"Pieniadze --> %d",cash);
    refresh();
    mtx.unlock();
}

//-------------------------------------------------------
void beginResource(){
    while(!endMine)
    {
            showMine();   
            while(mineStatus<=100)
            {
                usleep(rand()%1000000 + (int)(1000000/extraction));
                unique_lock<mutex> lck(mtx);
                mineStatus++;
                lck.unlock();
                cvMine.notify_all();
                showMine();
            }      
    }
}

void beginMiner(int id){
    while(!endMine)
	{
            bool work=false;
            showMiner(id, work);
            usleep(1000000);
            if(mineStatus<25)
            {
                unique_lock<mutex> lck(mtx);
                while(mineStatus<50) cvMine.wait(lck);
                lck.unlock(); 
            }
            mtx.lock();
            if((counterMiner[id])<=counterMiner[(2*numMiner+id-1)%numMiner] &&
            (counterMiner[id])<=counterMiner[(2*numMiner+id+1)%numMiner])
            {
                if(mineStatus>0 && digResource<100)
                {
                    counterMiner[id]++;
                    mineStatus--;
                    digResource++;
                    work=true;
                }
            }  
            mtx.unlock(); 
            cvResource.notify_one();    
            showMine();
            showDigResource();
            if(work)
            {
                for(int i=0;i<10;i++)
                {
                    showMiner(id, work);
                    usleep(rand()%100000+100000);
                }
            }
        
    }
}

void beginDriver(int id){
    
    while(!endMine)
    {   
        showDriverNo1(id, 0);
        unique_lock<mutex> lck(mtx);
        while(digResource<20) cvResource.wait(lck);
        lck.unlock();

        digResource-=20;
        showDigResource();
        showDriverNo1(id,1);
        usleep(500000); 
        lck.lock();

        while(forgeResourceStatus>80) cvForgeResource.wait(lck);
        lck.unlock();  
        forgeResourceStatus+=20;
        showForgeResource();
        showDriverNo2(id);
    }
}

void beginForge(int id){
    bool makeGold;
    bool makeDiamond;
    bool makeSilver;
    while(!endMine)
    {
        makeGold=false;
        makeDiamond=false;
        makeSilver=false;

        mtx.lock();
        if(forgeResourceStatus>0 && smallGoldBarStatus<=50 && bigGoldBarStatus<=50  
            && smallGoldBarStatus<=smallCutDiamondStatus &&smallGoldBarStatus<=silverBarStatus)
        {      
            forgeResourceStatus--;
            makeGold=true;
        }
        mtx.unlock();
        showForgeResource();

        mtx.lock();
        if(forgeResourceStatus>0 && smallCutDiamondStatus<=50 && bigCutDiamondStatus<=50  
            && smallCutDiamondStatus<=smallGoldBarStatus && smallCutDiamondStatus<=silverBarStatus)
        {      
            forgeResourceStatus--;
            makeDiamond=true;
        }
        cvForgeResource.notify_one();
        mtx.unlock();
        showForgeResource();

        mtx.lock();
        if(forgeResourceStatus>0 && silverBarStatus<=50  && silverBarStatus<=smallCutDiamondStatus 
            && silverBarStatus<=smallGoldBarStatus)
        {      
            forgeResourceStatus--;
            makeSilver=true;
        }
        mtx.unlock();
        showForgeResource();

        if(makeGold)
        {
            for(int i=0;i<10;i++)
            {
                usleep(rand()%1000000+1000000);
                smallGoldBarStatus++;               
                showForge();     
                if(i%2)
                {
                    usleep(rand()%1000000+1000000);
                    bigGoldBarStatus++;               
                    showForge();
                }
            }
        }
        if(makeDiamond)
        {
            for(int i=0;i<10;i++)
            {
                usleep(rand()%1000000+1000000);
                smallCutDiamondStatus++;               
                showForge();     
                if(i%2)
                {
                    usleep(rand()%1000000+1000000);
                    bigCutDiamondStatus++;               
                    showForge();
                }
            }
        }
        if(makeSilver)
        {
            for(int i=0;i<10;i++)
            {
                usleep(rand()%1000000+1000000);
                silverBarStatus++;               
                showForge();     
            }
        }
        if(cash>=1000 && extraction<1000000)
        {
            cash-=1000;
            extraction*=2;
            showCash();
        }
    }
}


void beginJewellerRing(int id){
    bool make;
    while(!endMine)
    {
        make = false;
        mtx.lock();
        if((smallGoldBarStatus>=7 && smallCutDiamondStatus>=7 && silverBarStatus>=7) 
            && (ringStatus<=necklaceStatus) && (ringStatus<=earringStatus))
        {       
            smallGoldBarStatus-=7;
            smallCutDiamondStatus-=7;
            bigCutDiamondStatus-=2;
            silverBarStatus-=7;
            make=true;
        }
        mtx.unlock();
        showForge();
        if(make)
        {
            for(int i=1;i<=10;i++)
            {
                usleep(rand()%100000+100000);
                showJewellerRing(id, i);
            }
            ringStatus++;
            showRing();
        }
    }
}

void beginJewellerNecklace(int id){
    bool make;
    while(!endMine)
    {
        make = false;
        mtx.lock();
        if((smallGoldBarStatus>=6 && bigGoldBarStatus>=3 && smallCutDiamondStatus>=6 && bigCutDiamondStatus>=3 
            && silverBarStatus>=6) && necklaceStatus<=ringStatus && necklaceStatus<=earringStatus)
        {       
            smallGoldBarStatus-=4;
            bigGoldBarStatus-=3;
            smallCutDiamondStatus-=6;
            bigCutDiamondStatus-=3;
            silverBarStatus-=6;
            make=true;
        }
        mtx.unlock();
        showForge();
        if(make)
        {
            for(int i=1;i<=10;i++)
            {
                usleep(rand()%100000+100000);
                showJewellerNecklace(id, i);
            }
            necklaceStatus++;
            showNecklace();
        }
    }  
}

void beginJewellerEarring(int id){
    bool make;
    while(!endMine)
    {
        make = false;
        mtx.lock();
        if((smallGoldBarStatus>=4 && bigGoldBarStatus>=4 && smallCutDiamondStatus>=4 && bigCutDiamondStatus>=4 
            && silverBarStatus>=4) && earringStatus<=necklaceStatus && earringStatus<=ringStatus)
        {       
            smallGoldBarStatus-=3;
            bigGoldBarStatus-=4;
            smallCutDiamondStatus-=3;
            bigCutDiamondStatus-=4;
            silverBarStatus -=3;
            make=true;
        }
        mtx.unlock();
        showForge();
        if(make)
        {
            for(int i=1;i<=10;i++)
            {
                usleep(rand()%100000+100000);
               showJewellerEarring(id, i);
            }
            earringStatus++;
            showEarring();
        }
    }
}

void beginTrader(int id){

    while(!endMine)
    {
        int choice = rand()%3+1;
        mtx.lock();
        if(choice==1)
        {
            if(ringStatus>0)
            {
                ringStatus--;
                cash+=150;
                ringStatusTrader[id]++;
                cashTrader[id]+=150;
            }
        }
        else if(choice==2)
        {
            if(necklaceStatus>0)
            {
                necklaceStatus--;
                cash+=200;
                necklaceStatusTrader[id]++;
                cashTrader[id]+=200;
            }
        }
        else if(choice==3)
        {
            if(earringStatus>0)
            {
                earringStatus--;
                cash+=100;
                earringStatusTrader[id]++;
                cashTrader[id]+=100;
            }
        }
        mtx.unlock();
        showRing();
        showNecklace();
        showEarring();
        showTrader(id);
        showCash();
        usleep(5000000);
    }
}

int main()
{

    if(getch()==27)
		{
			endMine=true;
		}
    
    initscr();
	nodelay(stdscr,TRUE);   

    refresh();
    srand(time(NULL));
    clear();
    start_color();
    init_pair(1,  COLOR_BLACK,COLOR_RED);
	init_pair(2,  COLOR_BLACK,COLOR_YELLOW);
	init_pair(3,  COLOR_BLACK,COLOR_GREEN);
	init_pair(4,  COLOR_BLACK,COLOR_BLUE);
	init_pair(5,  COLOR_BLACK,COLOR_MAGENTA);
	init_pair(6,  COLOR_BLACK,COLOR_WHITE);
	init_pair(7,  COLOR_BLACK,COLOR_CYAN);
    init_pair(8,  COLOR_YELLOW,COLOR_BLACK);
    curs_set(0);

//------------------deklaracja wątków--------------------
    thread resource;
    thread miner[numMiner];
    thread driver[numDriver];
    thread forge[numSmith];
    thread jewellerRing[numJewellerRing];
    thread jewellerNecklace[numJewellerNecklace];
    thread jewellerEarring[numJewellerEarring];
    thread trader[numTrader];
//-------------------------------------------------------   
showStatus();
//------------------uruchamianie wątków------------------
    resource = thread(beginResource);
    for (int i=0;i<numMiner;i++)
    {
        miner[i]=thread(beginMiner, i);
        counterMiner[i]=0;
    }
    for (int i=0;i<numDriver;i++)
    {
        driver[i]=thread(beginDriver, i);
    }

    for (int i=0;i<numSmith;i++)
    {
        forge[i]=thread(beginForge, i);
    }

    for (int i=0;i<numJewellerRing;i++)
    {
        jewellerRing[i]=thread(beginJewellerRing, i);
    }
    for (int i=0;i<numJewellerNecklace;i++)
    {
        jewellerNecklace[i]=thread(beginJewellerNecklace, i);
    }
    for (int i=0;i<numJewellerEarring;i++)
    {
        jewellerEarring[i]=thread(beginJewellerEarring, i);
    }
    for (int i=0;i<numTrader;i++)
    {
        trader[i]=thread(beginTrader, i);
    }
  
//-------------------join-------------------------

    resource.join();
    for (int i=0;i<numMiner;i++)
    {
        miner[i].join();
    }
    for (int i=0;i<numDriver;i++)
    {
        driver[i].join();
    }
    for (int i=0;i<numSmith;i++)
    {
        forge[i].join();
    }
    for (int i=0;i<numJewellerRing;i++)
    {
        jewellerRing[i].join();
    }
    for (int i=0;i<numJewellerNecklace;i++)
    {
        jewellerNecklace[i].join();
    }
    for (int i=0;i<numJewellerEarring;i++)
    {
        jewellerEarring[i].join();
    }
    for (int i=0;i<numTrader;i++)
    {
        trader[i].join();
    }

    endwin();
    return 0;
}
