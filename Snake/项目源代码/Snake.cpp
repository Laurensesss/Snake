#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"WINMM.LIB") // 为了引入声音
#define SIZEMAX		100		// 定义蛇的最大长度
#define SNAKELEN	10		// 蛇的大小和蛇每次移动的距离
#define SPEED		100		// 初始速度 
/****************************************/

typedef enum	// 定义表示方向的枚举变量
{
	left, right, up, down
}DIR; 

typedef struct	// 定义一个结构体表示坐标
{
	int x;
	int y;
}COOR; 

struct FOOD		// 定义结构体，来存储食物的相关信息
{
	COOR crd;	// 食物出现的坐标
	int flag;	// 食物的状态，1表示食物还没被吃，0表示食物被吃了
}food; 

struct SNAKE	// 定义结构体，来存储蛇的相关信息
{
	COOR crd[SIZEMAX];	// 保存蛇的每个结数的坐标，最多有100节
	int node;			// 保存蛇当前节数
	DIR dir;			// 当前蛇行进的方向
}snake; 

/*********************************************进行函数声明********************************************************/ 
void gameBegin();				//游戏开始界面
void modeChoose();				//选择模式界面
void helpMenu();				//帮助界面
void controls();				//控制帮助界面
void initMenu();				//菜单设置
void initGame();				// 初始化游戏
void showBackgroundPhoto();     //图片轮播效果
void setFoodCoordinate();		// 设置食物要出现的坐标
void showFood();				// 在相应的坐标画出食物
void snakeMove();				// 移动蛇，实际就是改变蛇各节的坐标
void judgeDie();				// 判断蛇是否撞墙或者撞到自己
void judgeEat();				// 判断蛇是否吃到食物了
void showSnake();				// 在屏幕上画出蛇
void changeSnakeDirection();	// 接受用户输入，改变蛇的前进方向
void setObstacle();				//设置障碍
void diemode();					//死亡模式
void challenge();				//挑战模式
void reopen();					//重新开始游戏
void changeLevel();				//改变等级

unsigned long double speed = SPEED;		// 方便以后在程序中改变速度
char Score[5];
char Level[5];
static unsigned int score = 0;			// 初始化分数
static unsigned int lvl=1;				// 初始化等级
int modeFlag = 0;

/********************************************定义主函数*****************************************************/

void main()
{
	initMenu();
}

void gameBegin() //游戏开始界面
{
	initGame(); 
	while(1)
	{
		while(!kbhit())
		{
			if (!food.flag)
			{
				setFoodCoordinate();
				showBackgroundPhoto();
			}
			showFood();
			judgeDie();
			judgeEat(); 
			snakeMove();
			showSnake();
			showFood();
			if(modeFlag == 1) //障碍模式
			{
				setObstacle();	
			}else if(modeFlag == 2) //死亡模式
			{
				diemode();				
			}else if(modeFlag == 3)  //挑战模式
			{
				challenge();
			}else if(modeFlag == 4)	//无敌模式
			{
				break;	
			}
			Sleep(speed);
		}
		changeSnakeDirection();	
	} 
}

/*******************************************定义功能函数*************************************************/

void initMenu() //菜单设置
{
	initgraph(800,642);
	loadimage(NULL,"resource/主界面.bmp");	// 读取图片至绘图窗口
	PlaySound("resource/start.wav",NULL,SND_LOOP|SND_ASYNC);	//插入声音
	while(1){
		char key;
		key = getch();

		if(key == 'r' || key == 'R'){
			modeChoose();
			break;
		}else if(key == 'h' || key == 'H'){
			helpMenu();
			break;
		}else if(key == 'i' || key == 'I'){
			controls();
			break;
		}
	}
}

void modeChoose()	//模式选择
{
	initgraph(800,640);
	loadimage(NULL,"resource/模式选择.bmp");
	PlaySound("resource/ready.wav",NULL,SND_ASYNC);
	while(1){
		char key;
		key = getch();
		if(key == 's' || key == 'S')
		{
			modeFlag = 1;
			gameBegin();
			break;
		}else if(key == 'h' || key == 'H')
		{
			modeFlag = 2;
			gameBegin();
			break;
		}else if(key == 'f' || key == 'F')
		{
			modeFlag = 3;
			gameBegin();
			break;
		}else if(key == 't' || key == 'T')
		{
			modeFlag = 4;
			gameBegin();
			break;
		}else if(key == 'b' || key == 'B')
		{
			initMenu();
			break;
		}
	}
}

void changeLevel() //改变等级
{
	if(lvl<=10)
	{
		lvl=score/10+1;
	}
}

void initGame() // 初始化游戏
{
	PlaySound("resource/go.wav",NULL,SND_ASYNC);
	score = 0;
	initgraph(800,640);
	showBackgroundPhoto();	
	srand(time(NULL));
	food.flag = 0;					// 食物的状态，1表示食物还没被吃，0表示食物被吃了
	snake.crd[0].x = 30 + SNAKELEN;
	snake.crd[0].y = 30;
	snake.crd[1].x = 30;
	snake.crd[1].y = 30;
	snake.node = 3;
	snake.dir = right;
} 

void setFoodCoordinate() // 设置食物要出现的坐标
{
	if(modeFlag == 1 && lvl== 1)	//当为障碍模式时，食物不应该出现在障碍物上
	{
		food.crd.x = rand() % (610 / SNAKELEN) * SNAKELEN + 10;
		food.crd.y = rand() % (610 / SNAKELEN) * SNAKELEN + 10;
	for(int a1=0;a1<10;a1++)
		{
			food.crd.x!=130+10*a1;
			food.crd.y!=170;
			food.flag = 1;
		}
	for(int a2=0;a2<10;a2++)
		{
			food.crd.x!=410+10*a2;
			food.crd.y!=170;
			food.flag = 1;
		}
	for(int a3=0;a3<10;a3++)
		{	
			food.crd.x!=270+10*a3;
			food.crd.y!=380;
			food.flag = 1;
		}
	}
	else	if(modeFlag == 1 && lvl== 2)
	{
		food.crd.x = rand() % (610 / SNAKELEN) * SNAKELEN + 10;
		food.crd.y = rand() % (610 / SNAKELEN) * SNAKELEN + 10;
	for(int b1=0;b1<10;b1++)
		{
			food.crd.x!=390+10*b1;
			food.crd.y!=160;
			food.flag = 1;
		}
	for(int b2=0;b2<10;b2++)
		{
			food.crd.x!=120;
			food.crd.y!=170+10*b2;
			food.flag = 1;
		}
	for(int b3=0;b3<10;b3++)
		{
			food.crd.x!=240+10*b2; 
			food.crd.y!=380;
			food.flag = 1;
		}
	}
	else  	if(modeFlag==1 && lvl==3)
	{
		food.crd.x = rand() % (610 / SNAKELEN) * SNAKELEN + 10;
		food.crd.y = rand() % (610 / SNAKELEN) * SNAKELEN + 10;
	for(int c1=0;c1<10;c1++)
		{
			food.crd.x!=120;
			food.crd.y!=190+10*c1;
			food.flag = 1;
		}
	for(int c2=0;c2<10;c2++)
		{
			food.crd.x!=290;
			food.crd.y!=190+10*c2;
			food.flag = 1;
		}
	for(int c3=0;c3<10;c3++)
		{
			food.crd.x!=450;
			food.crd.y!=190+10*c3;
			food.flag = 1;
		}
	}
	else {
	food.crd.x = rand() % (610 / SNAKELEN) * SNAKELEN + 10;
	food.crd.y = rand() % (610 / SNAKELEN) * SNAKELEN + 10;
	food.flag = 1;
	}
} 

void showFood() // 在相应的坐标画出食物
{
	rectangle(food.crd.x, food.crd.y, food.crd.x+SNAKELEN, food.crd.y+SNAKELEN);
} 

void showBackgroundPhoto()  //图片轮播效果
{
	switch(score)	//依据每一关分数的改变
	{
		
	case 0:
		loadimage(NULL,"resource/背景1.bmp");
			break;
	case 10:
		loadimage(NULL,"resource/背景2.bmp");
			break;
	case 20:
		loadimage(NULL,"resource/背景3.bmp");
			break;
	case 30:
		loadimage(NULL,"resource/背景4.bmp");
			break;
	case 40:
		loadimage(NULL,"resource/背景5.bmp");
			break;
	case 50:
		loadimage(NULL,"resource/背景6.bmp");
			break;
	case 60:
		loadimage(NULL,"resource/背景7.bmp");
			break;
	case 70:
		loadimage(NULL,"resource/背景8.bmp");
			break;
	case 80:
		loadimage(NULL,"resource/背景9.bmp");
			break;
	case 90:
		loadimage(NULL,"resource/背景10.bmp");
			break;
	}
	
}

void snakeMove()	// 移动蛇，实际就是改变蛇各节的坐标
{
	int i; 
	for (i=snake.node; i>0; i--)
	{
		snake.crd[i].x = snake.crd[i-1].x;
		snake.crd[i].y = snake.crd[i-1].y;
	} 
	switch (snake.dir)
	{
	case up:
		snake.crd[0].y -= SNAKELEN;
		break; 
	case down:
		snake.crd[0].y += SNAKELEN;
		break;
	case left:
		snake.crd[0].x -= SNAKELEN;
		break;
	case right:
		snake.crd[0].x += SNAKELEN;
	} showSnake();
} 

void judgeDie()		// 判断蛇是否撞墙或者撞到自己
{
	int i;
	
	if (snake.crd[0].x<10 || snake.crd[0].x > 630 || snake.crd[0].y<10 || snake.crd[0].y > 630)
	{
	PlaySound(NULL,NULL,SND_PURGE);
	PlaySound("resource/gameover.wav",NULL,SND_ASYNC);
		reopen();	
	} 
	for (i=snake.node-1; i>0; i--)
	{
		if (snake.crd[0].x==snake.crd[i].x && snake.crd[0].y==snake.crd[i].y)
		{
	PlaySound(NULL,NULL,SND_PURGE);
	PlaySound("resource/gameover.wav",NULL,SND_ASYNC);
			reopen();
		}
	}
} 
/*******************************************************模式设置********************************************/
void setObstacle()		//障碍设置
{
	switch(lvl)
	{
		case 1:
			{
		setfillcolor(BLUE);
		for(int i=0;i<10;i++)	  
	{
		POINT pts[] = {{130+10*i,170}, {140+10*i,170},{140+10*i,180},{130+10*i,180}};
		fillpolygon(pts, 4);	
	}
		for(int j=0;j<10;j++)	  
	{
		POINT pts[] = {{410+10*j,170}, {420+10*j,170},{420+10*j,180},{410+10*j,180}};
		fillpolygon(pts, 4);	
	}
		for(int k=0;k<10;k++)	  
	{
		POINT pts[] = {{270+10*k,380}, {280+10*k,380},{280+10*k,390},{270+10*k,390}};
		fillpolygon(pts, 4);	
	}	
		for(int a=0;a<10;a++)
	{
		if (((snake.crd[0].x==130+10*a) && (snake.crd[0].y==170)) || ((snake.crd[0].x==410+10*a) && (snake.crd[0].y==170)) || ((snake.crd[0].x==270+10*a) && (snake.crd[0].y==380)))
		{
			
	PlaySound(NULL,NULL,SND_PURGE);
	PlaySound("resource/gameover.wav",NULL,SND_ASYNC);
			reopen();
		}
	}
			}
			break;
		case 2:
			{
		setfillcolor(BLUE);
		for(int x=0;x<20;x++)
	{
		POINT pts[] = {{120,190+10*x}, {130,190+10*x},{130,200+10*x},{120,200+10*x}};
		fillpolygon(pts, 4);	
	}
		for(int y=0;y<20;y++)
	{
		POINT pts[] = {{290,190+10*y}, {300,190+10*y},{300,200+10*y},{290,200+10*y}};
		fillpolygon(pts, 4);	
	}
		for(int z=0;z<20;z++)
	{
		POINT pts[] = {{450,190+10*z}, {460,190+10*z},{460,200+10*z},{450,200+10*z}};
		fillpolygon(pts, 4);	
	}	
		for(int c=0;c<20;c++)
	{
		if (((snake.crd[0].x==120) && (snake.crd[0].y==190+10*c)) || ((snake.crd[0].x==290) && (snake.crd[0].y==190+10*c)) || ((snake.crd[0].x==450) && (snake.crd[0].y==190+10*c)))
		{
				PlaySound(NULL,NULL,SND_PURGE);
				PlaySound("resource/gameover.wav",NULL,SND_ASYNC);
						reopen();
		}
	}
			}
			break;
		case 3:
			{
		setfillcolor(BLUE);
		for(int k=0;k<10;k++)	  
	{
		POINT pts[] = {{390+10*k,160}, {400+10*k,160},{400+10*k,170},{390+10*k,170}};
		fillpolygon(pts, 4);	
	}
		for(int l=0;l<10;l++)	  
	{
		POINT pts[] = {{240+10*l,380}, {250+10*l,380},{250+10*l,390},{240+10*l,390}};
		fillpolygon(pts, 4);	
	}
		for(int m=0;m<10;m++)	  
	{
		POINT pts[] = {{120,170+10*m}, {130,170+10*m},{130,180+10*m},{120,180+10*m}};
		fillpolygon(pts, 4);	
	}
		for(int b=0;b<10;b++)
	{
		if (((snake.crd[0].x==390+10*b) && (snake.crd[0].y==160)) || ((snake.crd[0].x==240+10*b) && (snake.crd[0].y==380)) || ((snake.crd[0].x==120) && (snake.crd[0].y==170+10*b)))
		{
			
	PlaySound(NULL,NULL,SND_PURGE);
	PlaySound("resource/gameover.wav",NULL,SND_ASYNC);
			reopen();
		}
	}
			}	
			break;
	}
}

void diemode() //死亡模式
{
	speed = SPEED/5;
}

void challenge() //挑战模式
{
	setfillcolor(RED);
	fillrectangle(food.crd.x, food.crd.y, food.crd.x+SNAKELEN, food.crd.y+SNAKELEN);
}
/**********************************************************************************************************/


void judgeEat() // 判断蛇是否吃到食物了,并显示等级与分数
{
		//显示等级
		changeLevel();
		sprintf(Level, "%d",lvl);
		settextstyle(25,0,_T("楷体"),1,1,FW_NORMAL,false,false,false);
		settextcolor(LIGHTRED);
		outtextxy(720,430,Level);
		//初始化分数
		sprintf(Score,"%d",score*10);
		settextcolor(LIGHTRED);
		outtextxy(710,230,Score); 

	if (snake.crd[0].x==food.crd.x && snake.crd[0].y==food.crd.y )
	{	PlaySound(NULL,NULL,SND_PURGE);
		PlaySound("resource/eating.wav",NULL,SND_ASYNC);
		snake.node++;
		setcolor(BLACK);
		rectangle(food.crd.x, food.crd.y, food.crd.x+SNAKELEN, food.crd.y+SNAKELEN);
		setcolor(WHITE);
		food.flag = 0;
		score++;
		//显示分数
		sprintf(Score,"%d",score*10);	//吃到一食物就增加十分
		settextcolor(LIGHTRED);
		outtextxy(710,230,Score); 
	}
} 

void showSnake() // 在屏幕上画出蛇
{
	int i;
	for (i=snake.node-1; i>=0; i--)
	{	
		rectangle(snake.crd[i].x, snake.crd[i].y, snake.crd[i].x+SNAKELEN, snake.crd[i].y+SNAKELEN);
	} 
		setcolor(BLACK);
		rectangle(snake.crd[snake.node].x, snake.crd[snake.node].y, snake.crd[snake.node].x+SNAKELEN, snake.crd[snake.node].y+SNAKELEN);
		setcolor(WHITE); 
		rectangle(snake.crd[snake.node-1].x, snake.crd[snake.node-1].y, snake.crd[snake.node-1].x+SNAKELEN, snake.crd[snake.node-1].y+SNAKELEN);
} 

void changeSnakeDirection() // 接受用户输入，改变蛇的前进方向
{
	char key;
	key = getch();
	switch (key) //键盘获取蛇移动方向
	{
		case 'w':
		case 'W':
		case 72:
			if (snake.dir != down)
			{
				snake.dir = up;
			}
			break;
		case 's':
		case 'S':
		case 80:
			if (snake.dir != up)
			{
				snake.dir = down;
			}
			break;
		case 'a':
		case 'A':
		case 75:
			if (snake.dir != right)
			{
				snake.dir = left;
			}
			break;
		case 'd':
		case 'D':
		case 77:
			if (snake.dir != left)
			{
				snake.dir = right;
			}
			break;
			//下面的‘e’‘q’起加速与减速的作用
		case 'e':
		case 'E':
			switch(modeFlag){
			case 1:
			case 2:
			case 3:
			case 4:
			speed = SPEED/2;
			break;
			}
			break;
		case 'q':
		case 'Q':
			switch(modeFlag){
			case 1:
			case 2:
			case 3:
			case 4:
			speed = SPEED;
			break;
			}
			break;
	}
}

void reopen() //游戏结束界面，并显示该轮游戏的总分数和评语
{
	switch(lvl)
	{
		case 1:
		case 2:
			initgraph(800,642);
			loadimage(NULL,"resource/评分1.bmp");
				break;
		case 3:
		case 4:
			initgraph(800,642);
			loadimage(NULL,"resource/评分2.bmp");
				break;
		case 5:
		case 6:
			initgraph(800,642);
			loadimage(NULL,"resource/评分3.bmp");
				break;
		case 7:
		case 8:
			initgraph(800,642);
			loadimage(NULL,"resource/评分4.bmp");
				break;
		case 9:
		case 10:
			initgraph(800,642);
			loadimage(NULL,"resource/评分5.bmp");
				break;
	} 
	//显示总分数
	sprintf(Score, "%d", score*10);
	settextstyle(25,0,_T("楷体"),1,1,FW_NORMAL,false,false,false);
	settextcolor(RED);
	outtextxy(475,308,Score);
	while(1){
		char key;
		key = getch();
		if(key == 'r' || key == 'R' || key == 0x0d){
			modeChoose();
			break;
		}
	}
}

void helpMenu()		//控制帮助
{
	initgraph(800,640);
	loadimage(NULL,"resource/帮助界面.bmp");	// 读取图片至绘图窗口
	
	while(1){
		char key;
		key = getch();
		if(key == 'b' || key == 'B'){
			initMenu();
			break;
		}
	}
}

void controls()		//游戏说明
{
	initgraph(800,640);
	loadimage(NULL,"resource/介绍界面.bmp");	// 读取图片至绘图窗口
	
	while(1){
		char key;
		key = getch();
		if(key == 'b' || key == 'B'){
			initMenu();
			break;
		}
	}
}