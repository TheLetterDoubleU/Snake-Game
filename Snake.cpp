#include "pch.h"
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <windows.h>
#include <process.h>
#include <ctime>

using namespace std;

const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
int AppleNutrition = 3;

#define FI FOREGROUND_INTENSITY
#define RED FOREGROUND_RED
#define GREEN FOREGROUND_GREEN
#define BLUE FOREGROUND_BLUE

void StartMenu();
void RattlerRace();
void OptionMenu();
void Point(int, int);
char Arrows2WASD();
void ClearPlayArea();

int main()
{
	StartMenu();
	return EXIT_SUCCESS;
}

void StartMenu()
{
	int Selection = 0;
	while (Selection != 2)
	{
		Selection = 0;
		char Navigate = '*';
		SetConsoleTextAttribute(hOut, BLUE | FI);
		Point(0, 0);

		//Setup Border
		cout << char(201);
		for (int i = 1; i < 79; i++)
		{
			cout << char(205);
		}
		cout << char(187);
		for (int i = 1; i < 24; i++)
		{
			cout << char(186);
			Point(79, i);
			cout << char(186);
		}
		cout << char(200);
		for (int i = 1; i < 79; i++)
		{
			cout << char(205);
		}
		cout << char(188);

		//Brings the screen back up
		Point(0, 0);

		SetConsoleTextAttribute(hOut, RED | GREEN | BLUE | FI);
		Point(32, 5);
		cout << "~    SNAKE    ~";
		Point(20, 7);
		cout << "Arrow Keys = Navigate Menu / Change Direction";
		Point(25, 8);
		cout << "Space = Select Option / Return to Menu";
		Point(36, 10);
		cout << " Play";
		Point(36, 12);
		cout << "Options";
		Point(36, 14);
		cout << " Exit";

		//Menu
		while (Navigate != ' ')
		{
			if (Navigate == 'w')
			{
				Selection--;
				if (Selection < 0)
				{
					Selection = 2;
				}
			}
			else if (Navigate == 's')
			{
				Selection++;
				if (Selection > 2)
				{
					Selection = 0;
				}
			}
			Point(43, 10 + (Selection * 2));
			Navigate = Arrows2WASD();
		}
		_getch();

		switch (Selection)
		{
		case 0: RattlerRace(); break;
		case 1: OptionMenu(); break;
		case 2: break;
		}
	}
}

void RattlerRace()
{
	ClearPlayArea();
	//Mechanics
	int TICK = 150, Length = 2, AppleTally = 0, LengthUpdateDelay = 0;
	bool GameOver = false;
	char Direction = 'd';
	COORD SnakeHead = { 5,5 }, OldSnakeHead;

	int FieldOfPlay[80][25] = { 0 };
	for (int i = 0; i < 80; i++)
	{
		FieldOfPlay[i][0] = -1;
		FieldOfPlay[i][24] = -1;
	}
	for (int j = 1; j < 24; j++)
	{
		FieldOfPlay[0][j] = -1;
		FieldOfPlay[79][j] = -1;
	}
	FieldOfPlay[4][5] = 1;

	//First Apple
	FieldOfPlay[25][5] = -2;
	Point(25, 5);
	SetConsoleTextAttribute(hOut, RED | FI);
	cout << '*';

	while (!GameOver)
	{
		if (_kbhit())
		{
			Direction = Arrows2WASD();
		}

		OldSnakeHead = SnakeHead;
		switch (Direction)
		{
		case 'w': SnakeHead.Y--; break;
		case 'a': SnakeHead.X--; break;
		case 's': SnakeHead.Y++; break;
		case 'd': SnakeHead.X++; break;
		case ' ': GameOver = true; break;
		}

		if (FieldOfPlay[SnakeHead.X][SnakeHead.Y] == 0
			|| FieldOfPlay[SnakeHead.X][SnakeHead.Y] == 1
			|| FieldOfPlay[SnakeHead.X][SnakeHead.Y] == -2)
		{
			if (FieldOfPlay[SnakeHead.X][SnakeHead.Y] == -2)
			{
				//Apples
				LengthUpdateDelay += AppleNutrition;
				++AppleTally;
				if (AppleTally <= 50)
				{
					TICK -= 2;
				}
				srand((unsigned int(time(NULL))));
				int x, y;
				bool Valid = false;
				while (!Valid)
				{
					x = 1 + rand() % 78, y = 1 + rand() % 23;
					if (FieldOfPlay[x][y] == 0)
					{
						Valid = true;
					}
				}
				FieldOfPlay[x][y] = -2;
				Point(x, y);
				SetConsoleTextAttribute(hOut, RED | FI);
				cout << '*';
			}

			FieldOfPlay[OldSnakeHead.X][OldSnakeHead.Y] = Length;
			if (LengthUpdateDelay > 0)
			{
				++Length;
				--LengthUpdateDelay;
			}

			//Snake Color
			SetConsoleTextAttribute(hOut, GREEN | FI);

			Point(OldSnakeHead.X, OldSnakeHead.Y);
			//Body Character
			cout << 'o';
			int loop = Length;
			COORD temp = SnakeHead;
			while (loop > 0)
			{
				if (FieldOfPlay[temp.X - 1][temp.Y] == loop)
				{
					--loop;
					--temp.X;
					FieldOfPlay[temp.X][temp.Y] = loop;
				}
				else if (FieldOfPlay[temp.X + 1][temp.Y] == loop)
				{
					--loop;
					++temp.X;
					FieldOfPlay[temp.X][temp.Y] = loop;
				}
				else if (FieldOfPlay[temp.X][temp.Y - 1] == loop)
				{
					--loop;
					--temp.Y;
					FieldOfPlay[temp.X][temp.Y] = loop;
				}
				else if (FieldOfPlay[temp.X][temp.Y + 1] == loop)
				{
					--loop;
					++temp.Y;
					FieldOfPlay[temp.X][temp.Y] = loop;
				}
				else
				{
					loop = 0;
				}

				if (loop == 1) //TAIL
				{
					SetConsoleCursorPosition(hOut, temp);
					cout << '.';
				}
				if (loop == 0)
				{
					SetConsoleCursorPosition(hOut, temp);
					cout << ' ';
				}
			}

			//Head Updates
			Point(SnakeHead.X, SnakeHead.Y);
			switch (Direction)
			{
			case 'w': cout << char(30); break;
			case 'a': cout << char(17); break;
			case 's': cout << char(31); break;
			case 'd': cout << char(16); break;
			}
		}
		else
		{
			//Lose Animation
			SetConsoleCursorPosition(hOut, OldSnakeHead);
			cout << 'x';
			//FieldOfPlay[OldSnakeHead.X][OldSnakeHead.Y]=Length;
			int loop = Length - 1;
			while (loop > 0)
			{
				COORD temp = OldSnakeHead;
				if (FieldOfPlay[OldSnakeHead.X - 1][OldSnakeHead.Y] == loop)
				{
					--loop;
					--OldSnakeHead.X;
					FieldOfPlay[OldSnakeHead.X][OldSnakeHead.Y] = loop;
				}
				else if (FieldOfPlay[OldSnakeHead.X + 1][OldSnakeHead.Y] == loop)
				{
					--loop;
					++OldSnakeHead.X;
					FieldOfPlay[OldSnakeHead.X][OldSnakeHead.Y] = loop;
				}
				else if (FieldOfPlay[OldSnakeHead.X][OldSnakeHead.Y - 1] == loop)
				{
					--loop;
					--OldSnakeHead.Y;
					FieldOfPlay[OldSnakeHead.X][OldSnakeHead.Y] = loop;
				}
				else if (FieldOfPlay[OldSnakeHead.X][OldSnakeHead.Y + 1] == loop)
				{
					--loop;
					++OldSnakeHead.Y;
					FieldOfPlay[OldSnakeHead.X][OldSnakeHead.Y] = loop;
				}
				else
				{
					loop = 0;
				}
				SetConsoleCursorPosition(hOut, temp);
				cout << ' ';
				SetConsoleCursorPosition(hOut, OldSnakeHead);
				cout << 'x';
				Sleep(50);
			}
			GameOver = true;
		}
		Sleep(TICK);
	}
	//Display Score
	ClearPlayArea();
	SetConsoleTextAttribute(hOut, RED | GREEN | BLUE | FI);
	Point(32, 5);
	cout << "You ate " << AppleTally;
	if (AppleTally == 1)
	{
		cout << " apple and";
	}
	else
	{
		cout << " apples and";
	}
	if (AppleTally == 0)
	{
		Point(32, 6);
		cout << "did not grow at all.";
	}
	else
	{
		Point(31, 6);
		cout << "grew to a length of " << Length - 2 << ".";
	}
	Sleep(2500);
	Point(25, 8);
	cout << "Press any key to return to menu.";
	_getch();
	ClearPlayArea();
}

void OptionMenu()
{
	ClearPlayArea();
	Point(32, 5);
	cout << "~    OPTIONS    ~";
	Point(24, 7);
	cout << "Left & Right = Modify Variable(s)";
	Point(31, 8);
	cout << "Space = Main Menu";
	Point(30, 10);
	cout << "Apple Nutrition : " << AppleNutrition;

	char Navigate = '*';
	while (Navigate != ' ')
	{
		if (Navigate == 'a')
		{
			if (AppleNutrition > 1)
			{
				AppleNutrition--;
				Point(48, 10);
				cout << "  ";
				Point(48, 10);
				cout << AppleNutrition;
			}
		}
		else if (Navigate == 'd')
		{
			if (AppleNutrition < 25)
			{
				AppleNutrition++;
				Point(48, 10);
				cout << "  ";
				Point(48, 10);
				cout << AppleNutrition;
			}
		}
		Navigate = Arrows2WASD();
	}
	ClearPlayArea();
}

void Point(int X, int Y)
{
	COORD pos = { (short)X,(short)Y };
	SetConsoleCursorPosition(hOut, pos);
}

char Arrows2WASD()
{
	char Input = _getch();
	if (Input == char(224))
	{
		Input = _getch();
		switch (int(Input))
		{ //Respectively: UP DOWN LEFT RIGHT
		case 72: return 'w';
		case 80: return 's';
		case 75: return 'a';
		case 77: return 'd';
		}
	}
	return Input;
}

void ClearPlayArea()
{
	for (int i = 1; i < 24; i++)
	{
		Point(1, i);
		for (int j = 1; j < 79; j++)
		{
			cout << " ";
		}
	}
}
