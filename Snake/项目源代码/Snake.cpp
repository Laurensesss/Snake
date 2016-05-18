#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"WINMM.LIB") // Ϊ����������
#define SIZEMAX		100		// �����ߵ���󳤶�
#define SNAKELEN	10		// �ߵĴ�С����ÿ���ƶ��ľ���
#define SPEED		100		// ��ʼ�ٶ� 
/****************************************/

typedef enum	// �����ʾ�����ö�ٱ���
{
	left, right, up, down
}DIR; 

typedef struct	// ����һ���ṹ���ʾ����
{
	int x;
	int y;
}COOR; 

struct FOOD		// ����ṹ�壬���洢ʳ��������Ϣ
{
	COOR crd;	// ʳ����ֵ�����
	int flag;	// ʳ���״̬��1��ʾʳ�ﻹû���ԣ�0��ʾʳ�ﱻ����
}food; 

struct SNAKE	// ����ṹ�壬���洢�ߵ������Ϣ
{
	COOR crd[SIZEMAX];	// �����ߵ�ÿ�����������꣬�����100��
	int node;			// �����ߵ�ǰ����
	DIR dir;			// ��ǰ���н��ķ���
}snake; 

/*********************************************���к�������********************************************************/ 
void gameBegin();				//��Ϸ��ʼ����
void modeChoose();				//ѡ��ģʽ����
void helpMenu();				//��������
void controls();				//���ư�������
void initMenu();				//�˵�����
void initGame();				// ��ʼ����Ϸ
void showBackgroundPhoto();     //ͼƬ�ֲ�Ч��
void setFoodCoordinate();		// ����ʳ��Ҫ���ֵ�����
void showFood();				// ����Ӧ�����껭��ʳ��
void snakeMove();				// �ƶ��ߣ�ʵ�ʾ��Ǹı��߸��ڵ�����
void judgeDie();				// �ж����Ƿ�ײǽ����ײ���Լ�
void judgeEat();				// �ж����Ƿ�Ե�ʳ����
void showSnake();				// ����Ļ�ϻ�����
void changeSnakeDirection();	// �����û����룬�ı��ߵ�ǰ������
void setObstacle();				//�����ϰ�
void diemode();					//����ģʽ
void challenge();				//��սģʽ
void reopen();					//���¿�ʼ��Ϸ
void changeLevel();				//�ı�ȼ�

unsigned long double speed = SPEED;		// �����Ժ��ڳ����иı��ٶ�
char Score[5];
char Level[5];
static unsigned int score = 0;			// ��ʼ������
static unsigned int lvl=1;				// ��ʼ���ȼ�
int modeFlag = 0;

/********************************************����������*****************************************************/

void main()
{
	initMenu();
}

void gameBegin() //��Ϸ��ʼ����
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
			if(modeFlag == 1) //�ϰ�ģʽ
			{
				setObstacle();	
			}else if(modeFlag == 2) //����ģʽ
			{
				diemode();				
			}else if(modeFlag == 3)  //��սģʽ
			{
				challenge();
			}else if(modeFlag == 4)	//�޵�ģʽ
			{
				break;	
			}
			Sleep(speed);
		}
		changeSnakeDirection();	
	} 
}

/*******************************************���幦�ܺ���*************************************************/

void initMenu() //�˵�����
{
	initgraph(800,642);
	loadimage(NULL,"resource/������.bmp");	// ��ȡͼƬ����ͼ����
	PlaySound("resource/start.wav",NULL,SND_LOOP|SND_ASYNC);	//��������
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

void modeChoose()	//ģʽѡ��
{
	initgraph(800,640);
	loadimage(NULL,"resource/ģʽѡ��.bmp");
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

void changeLevel() //�ı�ȼ�
{
	if(lvl<=10)
	{
		lvl=score/10+1;
	}
}

void initGame() // ��ʼ����Ϸ
{
	PlaySound("resource/go.wav",NULL,SND_ASYNC);
	score = 0;
	initgraph(800,640);
	showBackgroundPhoto();	
	srand(time(NULL));
	food.flag = 0;					// ʳ���״̬��1��ʾʳ�ﻹû���ԣ�0��ʾʳ�ﱻ����
	snake.crd[0].x = 30 + SNAKELEN;
	snake.crd[0].y = 30;
	snake.crd[1].x = 30;
	snake.crd[1].y = 30;
	snake.node = 3;
	snake.dir = right;
} 

void setFoodCoordinate() // ����ʳ��Ҫ���ֵ�����
{
	if(modeFlag == 1 && lvl== 1)	//��Ϊ�ϰ�ģʽʱ��ʳ�ﲻӦ�ó������ϰ�����
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

void showFood() // ����Ӧ�����껭��ʳ��
{
	rectangle(food.crd.x, food.crd.y, food.crd.x+SNAKELEN, food.crd.y+SNAKELEN);
} 

void showBackgroundPhoto()  //ͼƬ�ֲ�Ч��
{
	switch(score)	//����ÿһ�ط����ĸı�
	{
		
	case 0:
		loadimage(NULL,"resource/����1.bmp");
			break;
	case 10:
		loadimage(NULL,"resource/����2.bmp");
			break;
	case 20:
		loadimage(NULL,"resource/����3.bmp");
			break;
	case 30:
		loadimage(NULL,"resource/����4.bmp");
			break;
	case 40:
		loadimage(NULL,"resource/����5.bmp");
			break;
	case 50:
		loadimage(NULL,"resource/����6.bmp");
			break;
	case 60:
		loadimage(NULL,"resource/����7.bmp");
			break;
	case 70:
		loadimage(NULL,"resource/����8.bmp");
			break;
	case 80:
		loadimage(NULL,"resource/����9.bmp");
			break;
	case 90:
		loadimage(NULL,"resource/����10.bmp");
			break;
	}
	
}

void snakeMove()	// �ƶ��ߣ�ʵ�ʾ��Ǹı��߸��ڵ�����
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

void judgeDie()		// �ж����Ƿ�ײǽ����ײ���Լ�
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
/*******************************************************ģʽ����********************************************/
void setObstacle()		//�ϰ�����
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

void diemode() //����ģʽ
{
	speed = SPEED/5;
}

void challenge() //��սģʽ
{
	setfillcolor(RED);
	fillrectangle(food.crd.x, food.crd.y, food.crd.x+SNAKELEN, food.crd.y+SNAKELEN);
}
/**********************************************************************************************************/


void judgeEat() // �ж����Ƿ�Ե�ʳ����,����ʾ�ȼ������
{
		//��ʾ�ȼ�
		changeLevel();
		sprintf(Level, "%d",lvl);
		settextstyle(25,0,_T("����"),1,1,FW_NORMAL,false,false,false);
		settextcolor(LIGHTRED);
		outtextxy(720,430,Level);
		//��ʼ������
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
		//��ʾ����
		sprintf(Score,"%d",score*10);	//�Ե�һʳ�������ʮ��
		settextcolor(LIGHTRED);
		outtextxy(710,230,Score); 
	}
} 

void showSnake() // ����Ļ�ϻ�����
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

void changeSnakeDirection() // �����û����룬�ı��ߵ�ǰ������
{
	char key;
	key = getch();
	switch (key) //���̻�ȡ���ƶ�����
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
			//����ġ�e����q�����������ٵ�����
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

void reopen() //��Ϸ�������棬����ʾ������Ϸ���ܷ���������
{
	switch(lvl)
	{
		case 1:
		case 2:
			initgraph(800,642);
			loadimage(NULL,"resource/����1.bmp");
				break;
		case 3:
		case 4:
			initgraph(800,642);
			loadimage(NULL,"resource/����2.bmp");
				break;
		case 5:
		case 6:
			initgraph(800,642);
			loadimage(NULL,"resource/����3.bmp");
				break;
		case 7:
		case 8:
			initgraph(800,642);
			loadimage(NULL,"resource/����4.bmp");
				break;
		case 9:
		case 10:
			initgraph(800,642);
			loadimage(NULL,"resource/����5.bmp");
				break;
	} 
	//��ʾ�ܷ���
	sprintf(Score, "%d", score*10);
	settextstyle(25,0,_T("����"),1,1,FW_NORMAL,false,false,false);
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

void helpMenu()		//���ư���
{
	initgraph(800,640);
	loadimage(NULL,"resource/��������.bmp");	// ��ȡͼƬ����ͼ����
	
	while(1){
		char key;
		key = getch();
		if(key == 'b' || key == 'B'){
			initMenu();
			break;
		}
	}
}

void controls()		//��Ϸ˵��
{
	initgraph(800,640);
	loadimage(NULL,"resource/���ܽ���.bmp");	// ��ȡͼƬ����ͼ����
	
	while(1){
		char key;
		key = getch();
		if(key == 'b' || key == 'B'){
			initMenu();
			break;
		}
	}
}