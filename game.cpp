#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <iostream>
#include <string>
using namespace std;
// Constants for indicating the direction of displacement of the snake
#define DIR_RIGHT 1
#define DIR_LEFT -1
#define DIR_UP    2
#define DIR_DOWN -2
char field[50][50]; //The size of the field where the game is built
int co_x[50], co_y[50], foodx, foody; //Coordinates of the snake and food
const int height = 16, width = 46; // Height of the game field and width
int size = 2; //Length of snake
int direction=1;//Direction of movement. Initially moves to the right
int score = 0;// Scoring

// Function for setting the cursor to the specified position
void gotoXY (int column, int line) 
{ 
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

 //Function of initialization of the game field and snake
void init_field () 
    { 
        // Initialize the game field
        for (int i = 0; i <= height; ++i) 
            for (int j = 0; j <= width; ++j)
                field[i][j] = (i == 0 || j == 0 || i == height || j == width ? '#' : ' '); //Define the borders
        // Initialize the structure of the snake
        size = 2;
        co_x[0] = 1;
        co_y[0] = 1;
        co_x[1] = 2;
        co_y[1] = 1;
		//"Snake body" consisting of symbols 'O'
        field[1][1] = 'O';
        field[1][2] = 'O';
    }
	
 // Function of generating food on the field
void generate_food () 
    {  
	do{
		srand( time(0) );//Random on each launch is different.
        foody = (rand() % 15)+1;
        foodx = (rand() % 45)+1;
	}while(field[foody][foodx]=='O'); //Protection from the appearance of food on the snake
        gotoXY(foodx, foody);
        cout << "*"; //Drawing the food by symbol '*'
    }
	
 //Function of drawing a field
void draw_field () 
    {
        int i,j;
        system("cls"); //Clear screen
        for (i = 0; i <= height; ++i)
            {
                for (j = 0; j <= width; ++j)
                    cout << field[i][j]; 
                cout << "\n";   
            }
        cout << "\n";
        // Initialize the food
        generate_food();
    }
	
//Function of placing fruit on the field
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
//The function of checking whether the snake ate itself
bool dead () 
    { 
        int i;
        // Crashed into the borders of the field?
        if (co_x[size-1] == width) 
            return true;
        if (co_x[size-1] == 0) 
            return true;
        if (co_y[size-1] == height) 
            return true;
        if (co_y[size-1] == 0) 
            return true;
        // Hit itself in itself?
        for (i = 0; i < size-3; ++i)
            if (co_x[i] == co_x[size-1] && co_y[i] == co_y[size-1])
               return true;
        return false;
    }
	
//Function with the basic operations at movement
void move () 
    { 
        char input;
        int temp_dir = direction;
        int speed=300;
        char ending[]="GAME OVER!";
        do {
            if (kbhit()) input = getch(); //Check the key press on the console
            else input = 0;
            switch (input)//The global value of the key and ignore the movement back
            {
                case 'P': 
                    if (direction==DIR_UP) //  If we try to turn up, nothing happens
                        break;// The operation is interrupt
                    temp_dir=DIR_DOWN; // Can only move down
                    break;
                case 'H':  //Similarly for other sides.
                    if (direction==DIR_DOWN) break; //Move down
                    temp_dir=DIR_UP;
                    break;
                case 'K':
                    if (direction==DIR_RIGHT) break; //Move left
                    temp_dir= DIR_LEFT; 
                    break;
                case 'M': 
                    if (direction==DIR_LEFT) break; //Move right
                    temp_dir= DIR_RIGHT; 
                    break;
                default:
                    break;
            }
            direction=temp_dir;
            switch (temp_dir) // Movement from the head of the snake in the specified direction
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
            // If the fruit on the field and the snake ate it, it grows itself
			// and a new fruit appears, the speed increases.
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
            // If the snake died, then the end of the game
            if(dead()) 
                {
                for (int i=0; i<11; i++) 
                    {
                        gotoXY (i+18, 7);
                        cout << ending[i]; // The conclusion of the inscription "Game over"
                    }
            break;
                }
            // Denote the speed and output the current score
            gotoXY(1,18);
            cout <<"Score : "<< score <<"\n\n";
            Sleep(speed);
        } while (input!=27); // Exit the game by pressing the 'esc' key 
    }
int main() 
    { 
        init_field ();
        draw_field();
        move();
        return 0;
    }