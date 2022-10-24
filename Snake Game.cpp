#include <stdio.h>
#include <conio.h>
#include <windows.h> 
#include <string.h>

int width=88,height=22;
char PlayerName[20],playAgain;
int xp,yp,fxp,fyp,score,gameover=0; //xp and yp for snake and fxp and fyp for food position
int upFlag,downFlag,leftFlag,rightFlag;

struct Node
{
	int nxp;
	int nyp;
	struct Node *next;
	struct Node *prev;
};
struct Node *start=NULL;

void gotoxy(int x,int y)
{
	COORD c;
	c.X=x;
	c.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

void box(int xc,int yr,int xc2,int yr2)
{
	for (int x=xc;x<=xc2;x++)
	{
		gotoxy(x,yr);
		printf("//");
		Sleep(10);
		gotoxy(x,yr2);
		printf("//");
	}
	for (int y=yr+1;y<=yr2;y++)
	{
		gotoxy(xc,y);
		printf("//");
		Sleep(10);
		gotoxy(xc2,y);
		printf("//");	
	}
}

void startGame()
{
	box(22,5,88,22); //box(width/4,height/4,width,height)
	gotoxy(45,8);
	printf("Welcome to Snake game");
	gotoxy(43,13);
	printf("Enter your name: ");
	scanf("%s",&PlayerName);
	system("cls");
}

void setupBox()
{
	for (int x=2;x<=width+2;x++)
	{
		gotoxy(x,2);
		printf("//");
		gotoxy(x,height+2);
		printf("//");
	}
	for (int y=2;y<=height+2;y++)
	{
		gotoxy(2,y);
		printf("//");
		gotoxy(width+2,y);
		printf("//");	
	}
}

void resetFlag()
{
	upFlag=0;
	downFlag=0;
	leftFlag=0;
	rightFlag=0;
}

void scoreBoard()
{
	gotoxy(width+5,3);
	printf("%s's Score: %d",PlayerName,score*100);
}

void createHeadNode()
{
	struct Node *temp;
	temp=(struct Node*)malloc(sizeof(struct Node));
	temp->next=NULL;
	temp->prev=NULL;
	temp->nxp=(width+2)/2;
	temp->nyp=(height+2)/2;
	start=temp;
}

void setup()
{
	gameover=0;
	score=0;
	//setting up a box inside of which game will be played 
	setupBox();
	resetFlag();
	scoreBoard();
	createHeadNode();
	//position {x,y}, positioning the head of the snake
	xp=(width+2)/2;
	yp=(height+2)/2;
	//positioning food
	do
	{
		fxp=rand()%width;
	}
	while(fxp==xp || fxp==width || fxp==0); //if condition is not satisfied, loop will be ended
	do
	{
		fyp=rand()%height;
	}
	while(fyp==yp || fyp==height || fyp==0);
}

void drawSnake()
{
	struct Node *curr;
	curr=start;
	while (curr!=NULL)
	{
		gotoxy(curr->nxp,curr->nyp);
//		printf("O");
		printf("Û");
		curr=curr->next;
	}
}

void destroySnake()
{
	struct Node *curr=start;
	while (curr!=NULL)
	{
		gotoxy(curr->nxp,curr->nyp);
		printf(" ");
		curr=curr->next;
	}
}
void draw()
{
	drawSnake();
	gotoxy(fxp,fyp);
	printf("@");
	Sleep(120); //speed of the snake
	destroySnake(); //for movement i.e if snake moves forward new snake will be drawn and old one will be deleted
}

void play()
{
	int direction;
	char direc;
	if(kbhit())
	{
		direc=getch();
		direction=direc;
		switch(direction)
		{
			case 72: //72 is acsii code of up key
			{
				if (downFlag!=1) //if snake is moving downward direction, it can't go up.
				{
					resetFlag();
					upFlag=1;
				}
				break;
			}
			case 80: //80 is acsii code of down key
			{
				if (upFlag!=1)
				{
					resetFlag();
					downFlag=1;
				}
				break;
			}
			case 75: //75 is acsii code of left key
			{
				if (rightFlag!=1)
				{
					resetFlag();
					leftFlag=1;
				}
				break;
			}
			case 77: //77 is acsii code of right key
			{
				if (leftFlag!=1)
				{
					resetFlag();
					rightFlag=1;
				}
				break;
			}
			default:
				break;
		}
	}
}

void run()
{
	if (upFlag==1) 
	{
		yp--;
	}
	if (downFlag==1)
	{
		yp++;
	}
	if (rightFlag==1)
	{
		xp++;
	}
	if (leftFlag==1)
	{
		xp--;
	}
}

void gameoverConditions()
{
	//gameover if snake hits the boundary i.e either width or height
	if (xp==width+2||xp==2)
	{
		gameover=1;
	}
	if (yp==height+2||yp==2)
	{
		gameover=1;
	}
	//gameover if snake hits itself
	struct Node *curr;
	curr=start->next;
	while (curr!=NULL)
	{
		if(start->nxp==curr->nxp&&start->nyp==curr->nyp)
		{
			gameover=1;
			break;
		}
		curr=curr->next;
	}
}

void addNode()
{
	struct Node *temp;
	temp=(struct Node*)malloc(sizeof(struct Node));
	temp->next=NULL;
	temp->prev=NULL;
	
	struct Node *curr=start;
	while (curr->next!=NULL)
	{
		curr=curr->next;
	}
	curr->next=temp;
	temp->prev=curr;
}

void changeFoodPosition()
{
	do
	{
		fxp=rand()%width; //generating random position to place food
	}
	while(fxp==xp || fxp==width || fxp==0); //food shouldn't be on width, or on border
	do
	{
		fyp=rand()%height;
	}
	while(fyp==yp || fyp==height || fyp==0); //if condition is not satisfied, loop will be ended
}

void eatFood()
{
	if (xp==fxp&&yp==fyp) //when snake eats the food 1-addition of node(snake size will increase) 2-place food in new position 3-increase score
	{
		addNode();
		changeFoodPosition();
		score++;
		scoreBoard();
	}	
}

void deleteLastNode()
{
	struct Node *curr=start->next;
	struct Node *curr2=start;
	while(curr->next!=NULL)
	{
		curr=curr->next;
		curr2=curr->prev;
	}
	curr->prev=curr2;
	curr2->next=NULL;
	free(curr);
}

void addHeadNode()
{
	struct Node *temp;
	struct Node *curr=start;
	temp=(struct Node*)malloc(sizeof(struct Node));
	temp->prev=NULL;
	temp->next=curr;
	curr->prev=temp;
	temp->nxp=xp;
	temp->nyp=yp;
	start=temp;
}

void snakeMovement()
{
	// removing the last node and adding the head node - show the movement
	addHeadNode();
	deleteLastNode();
}

int gameEnd()
{
	box(11,5,77,22); 
	gotoxy(40,8);
	printf("Gameover");
	gotoxy(36,13);
	printf("%s's score is %d",PlayerName,score*100);
	gotoxy(28,17);
	printf("Do you want to play again? (Y/N): ");
	playAgain=getch();
	printf("%c",playAgain);
	return(playAgain);
}

void game()
{
	do 
	{
		system("cls");
		setup();
		while (gameover!=1)
		{
			draw();
			play();
			run();
			gameoverConditions();
			eatFood();
			snakeMovement();
		}
		gameEnd();
	}while(playAgain=='y'||playAgain=='Y');
}

int main()
{
	startGame();
	game();
	gotoxy(0,24);
}
