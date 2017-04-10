#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <iostream>
#include <string>
using namespace std;
// Константы для указания направления перемещения змейки
#define DIR_RIGHT 1
#define DIR_LEFT -1
#define DIR_UP    2
#define DIR_DOWN -2
char field[50][50]; //размер поля, в котором строится игра
int co_x[50], co_y[50], foodx, foody; //координаты змейки и еды
const int height = 16, width = 46; // высота игрового поля и ширина
int size = 2; //длина змейки
int direction=1;//направление движения. Изначально движется вправо 
int score = 0;// подсчет очков
 // Функция для установки курсора в указанную позицию
void gotoXY (int column, int line) 
{ 
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}
 //Функция инициализации игрового поля и змейки
void init_field () 
    { 
        // инициализируем игровое поле
        for (int i = 0; i <= height; ++i) 
            for (int j = 0; j <= width; ++j)
                field[i][j] = (i == 0 || j == 0 || i == height || j == width ? '#' : ' '); //определяем границы
        // инициализируем структуру змейки
        size = 2;
        co_x[0] = 1;
        co_y[0] = 1;
        co_x[1] = 2;
        co_y[1] = 1;
		//"Тело" змейки, состоящее из символов 'O'
        field[1][1] = 'O';
        field[1][2] = 'O';
    }
 // Функция генерации фрукта на поле
void generate_food () 
    {  
	do{
		srand( time(0) );//Рандом на каждом запуске разный
        foody = (rand() % 15)+1;
        foodx = (rand() % 45)+1;
	}while(field[foody][foodx]=='O'); //Защита от появления фрукта на змейке
        gotoXY(foodx, foody);
        cout << "*"; //Отрисовка фрукта символом '*'
    }
 //Функция отрисовки поля
void draw_field () 
    {
        int i,j;
        system("cls"); //очищаем экран
        for (i = 0; i <= height; ++i)
            {
                for (j = 0; j <= width; ++j)
                    cout << field[i][j]; 
                cout << "\n";   
            }
        cout << "\n";
        // инициализируем фрукт
        generate_food();
    }
//Функция помещения фрукта на поле
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
//Функция проверки, съела ли змейка саму себя
bool dead () 
    { 
        int i;
        // врезалась в границы поля?
        if (co_x[size-1] == width) 
            return true;
        if (co_x[size-1] == 0) 
            return true;
        if (co_y[size-1] == height) 
            return true;
        if (co_y[size-1] == 0) 
            return true;
        // попала сама в себя?
        for (i = 0; i < size-3; ++i)
            if (co_x[i] == co_x[size-1] && co_y[i] == co_y[size-1])
               return true;
        return false;
    }
//Функция с основными операциями при движении
void move () 
    { 
        char input;
        int temp_dir = direction;
        int speed=300;
        char ending[]="GAME OVER!";
        do {
            if (kbhit()) input = getch(); //проверяем нажатие клавиши на консоли 
            else input = 0;
            switch (input)//глобальное значение клавиши и игнорируем движение обратно
            {
                case 'P': 
                    if (direction==DIR_UP) //  если пробуем повернуть наверх, то ничего не получается
                        break;// операция прерывается
                    temp_dir=DIR_DOWN; // можно только вниз
                    break;
                case 'H':  //Аналогично для других сторон
                    if (direction==DIR_DOWN) break; //Движение вниз
                    temp_dir=DIR_UP;
                    break;
                case 'K':
                    if (direction==DIR_RIGHT) break; //Движение влево
                    temp_dir= DIR_LEFT; 
                    break;
                case 'M': 
                    if (direction==DIR_LEFT) break; //Движение вправо
                    temp_dir= DIR_RIGHT; 
                    break;
                default:
                    break;
            }
            direction=temp_dir;
            switch (temp_dir) // движение от головы змейки по указанному направлению
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
            // если фрукт на поле и змейка его съела, то растёт сама и появляется новый фрукт,
			// скорость увеличивается
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
            // если змейка умерла, то конец игры
            if(dead()) 
                {
                for (int i=0; i<11; i++) 
                    {
                        gotoXY (i+18, 7);
                        cout << ending[i]; // Вывод надписи "Game over"
                    }
            break;
                }
            // обозначаем скорость и выводим текущий счет
            gotoXY(1,18);
            cout <<"Score : "<< score <<"\n\n";
            Sleep(speed);
        } while (input!=27); // Выход из игры по нажатию клавиши 'esc'
    }
int main() 
    { 
        init_field ();
        draw_field();
        move();
        return 0;
    }