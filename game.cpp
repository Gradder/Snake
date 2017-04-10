#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <iostream>
#include <string>
using namespace std;
// ��������� ��� �������� ����������� ����������� ������
#define DIR_RIGHT 1
#define DIR_LEFT -1
#define DIR_UP    2
#define DIR_DOWN -2
char field[50][50]; //������ ����, � ������� �������� ����
int co_x[50], co_y[50], foodx, foody; //���������� ������ � ���
const int height = 16, width = 46; // ������ �������� ���� � ������
int size = 2; //����� ������
int direction=1;//����������� ��������. ���������� �������� ������ 
int score = 0;// ������� �����
 // ������� ��� ��������� ������� � ��������� �������
void gotoXY (int column, int line) 
{ 
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}
 //������� ������������� �������� ���� � ������
void init_field () 
    { 
        // �������������� ������� ����
        for (int i = 0; i <= height; ++i) 
            for (int j = 0; j <= width; ++j)
                field[i][j] = (i == 0 || j == 0 || i == height || j == width ? '#' : ' '); //���������� �������
        // �������������� ��������� ������
        size = 2;
        co_x[0] = 1;
        co_y[0] = 1;
        co_x[1] = 2;
        co_y[1] = 1;
		//"����" ������, ��������� �� �������� 'O'
        field[1][1] = 'O';
        field[1][2] = 'O';
    }
 // ������� ��������� ������ �� ����
void generate_food () 
    {  
	do{
		srand( time(0) );//������ �� ������ ������� ������
        foody = (rand() % 15)+1;
        foodx = (rand() % 45)+1;
	}while(field[foody][foodx]=='O'); //������ �� ��������� ������ �� ������
        gotoXY(foodx, foody);
        cout << "*"; //��������� ������ �������� '*'
    }
 //������� ��������� ����
void draw_field () 
    {
        int i,j;
        system("cls"); //������� �����
        for (i = 0; i <= height; ++i)
            {
                for (j = 0; j <= width; ++j)
                    cout << field[i][j]; 
                cout << "\n";   
            }
        cout << "\n";
        // �������������� �����
        generate_food();
    }
//������� ��������� ������ �� ����
bool isFoodField ()
    {
        if (co_x[size]==foodx && co_y[size]==foody) 
        {
            foodx=0;
            foody=0;
            score++;
            return true;
        }
        return false;
    }
//������� ��������, ����� �� ������ ���� ����
bool dead () 
    { 
        int i;
        // ��������� � ������� ����?
        if (co_x[size-1] == width) 
            return true;
        if (co_x[size-1] == 0) 
            return true;
        if (co_y[size-1] == height) 
            return true;
        if (co_y[size-1] == 0) 
            return true;
        // ������ ���� � ����?
        for (i = 0; i < size-3; ++i)
            if (co_x[i] == co_x[size-1] && co_y[i] == co_y[size-1])
               return true;
        return false;
    }
//������� � ��������� ���������� ��� ��������
void move () 
    { 
        char input;
        int temp_dir = direction;
        int speed=300;
        char ending[]="GAME OVER!";
        do {
            if (kbhit()) input = getch(); //��������� ������� ������� �� ������� 
            else input = 0;
            switch (input)//���������� �������� ������� � ���������� �������� �������
            {
                case 'P': 
                    if (direction==DIR_UP) //  ���� ������� ��������� ������, �� ������ �� ����������
                        break;// �������� �����������
                    temp_dir=DIR_DOWN; // ����� ������ ����
                    break;
                case 'H':  //���������� ��� ������ ������
                    if (direction==DIR_DOWN) break; //�������� ����
                    temp_dir=DIR_UP;
                    break;
                case 'K':
                    if (direction==DIR_RIGHT) break; //�������� �����
                    temp_dir= DIR_LEFT; 
                    break;
                case 'M': 
                    if (direction==DIR_LEFT) break; //�������� ������
                    temp_dir= DIR_RIGHT; 
                    break;
                default:
                    break;
            }
            direction=temp_dir;
            switch (temp_dir) // �������� �� ������ ������ �� ���������� �����������
            {
                case DIR_RIGHT:
                    co_x[size] = co_x[size-1]+1;
                    co_y[size] = co_y[size-1];
                    break;
                case DIR_LEFT:
                    co_x[size] = co_x[size-1]-1;
                    co_y[size] = co_y[size-1];
                    break;
                case DIR_UP:
                    co_y[size] = co_y[size-1]-1;
                    co_x[size] = co_x[size-1];
                    break;
                case DIR_DOWN:
                    co_y[size] = co_y[size-1]+1;
                    co_x[size] = co_x[size-1];
                    break;
                default:
                    break;
            }
            // ���� ����� �� ���� � ������ ��� �����, �� ����� ���� � ���������� ����� �����,
			// �������� �������������
            if (isFoodField()) 
                {
                    gotoXY (co_x[size], co_y[size]);
                    cout << "O";
                    ++size;
                    speed=speed-10;
                    generate_food();
                } 
            else 
               {
                gotoXY (co_x[0],co_y[0]);
                cout << ' ';
                for (int i = 0; i < size; i++)
                {
                    co_x[i] = co_x[i+1];
                    co_y[i] = co_y[i+1];
                } 
                // 
                gotoXY (co_x[size], co_y[size]);
                cout << "O";
            }
            // ���� ������ ������, �� ����� ����
            if(dead()) 
                {
                for (int i=0; i<11; i++) 
                    {
                        gotoXY (i+18, 7);
                        cout << ending[i]; // ����� ������� "Game over"
                    }
            break;
                }
            // ���������� �������� � ������� ������� ����
            gotoXY(1,18);
            cout <<"Score : "<< score <<"\n\n";
            Sleep(speed);
        } while (input!=27); // ����� �� ���� �� ������� ������� 'esc'
    }
int main() 
    { 
        init_field ();
        draw_field();
        move();
        return 0;
    }