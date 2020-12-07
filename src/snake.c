#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

#ifdef _WIN32
    #include <conio.h>
    #define UP_ARROW 'H'
    #define DOWN_ARROW 'P'
    #define LEFT_ARROW 'K'
    #define RIGHT_ARROW 'M'
    #define CLEAR_COMMAND "cls"
#endif

#ifdef linux
    #include <termios.h>
    #define UP_ARROW 'A'
    #define DOWN_ARROW 'B'
    #define LEFT_ARROW 'D'
    #define RIGHT_ARROW 'C'
    #define CLEAR_COMMAND "clear"

    int getch()
    {
        int ch;
        struct termios oldt, newt;
        tcgetattr( STDIN_FILENO, &oldt );
        newt = oldt;
        newt.c_lflag &= ~( ICANON | ECHO );
        tcsetattr( STDIN_FILENO, TCSANOW, &newt );
        ch = getchar();
        tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
        return ch;
    }
#endif

#define MAP_H 20
#define MAP_W 80

int score;

int body_snake[100][2];;
int snake_change_to;
int snake_length;
int xf;
int yf;

char map[MAP_H][MAP_W + 1];
char key;

void init_snake();
void draw_map();
void display_map();
void insert_coord(int x, int y);
void pop_coord();
void generate_food();
void * game(void *args);

int main()
{
    pthread_t thread;
    int status;

    system(CLEAR_COMMAND);

    status = pthread_create(&thread, NULL, game, NULL);
    if(status != 0)
        return 1;

    while(key != 'q')
        key = getch();

	return 0;
}

void * game(void *args)
{
    srand(time(NULL));
    init_snake();
    generate_food();

    while(1)
    {
        printf("\033[0;0H");
        draw_map();
        display_map();

        printf("Score: %i\n", score);

        if((key == UP || key == UP_ARROW) && snake_change_to != 3)
            snake_change_to = 1;
        else if((key == RIGHT || key == RIGHT_ARROW) && snake_change_to != 4)
            snake_change_to = 2;
        else if((key == DOWN || key == DOWN_ARROW) && snake_change_to != 1)
            snake_change_to = 3;
        else if((key == LEFT || key == LEFT_ARROW) && snake_change_to != 2)
            snake_change_to = 4;

        if(snake_change_to == 1)
            insert_coord(body_snake[0][0], body_snake[0][1] - 1);
        else if(snake_change_to == 2)
            insert_coord(body_snake[0][0] + 1, body_snake[0][1]);
        else if(snake_change_to == 3)
            insert_coord(body_snake[0][0], body_snake[0][1] + 1);
        else if(snake_change_to == 4)
            insert_coord(body_snake[0][0] - 1, body_snake[0][1]);

        if(map[body_snake[0][1]][body_snake[0][0]] == '*')
        {
            score++;
            generate_food();
        }
        else
            pop_coord();

        if(map[body_snake[0][1]][body_snake[0][0]] == '#')
        {
            printf("Game Over\n");
            sleep(2);
            system(CLEAR_COMMAND);

            init_snake();
            generate_food();
        }

        usleep(40000);
    }

    return 0;
}

void init_snake()
{
    body_snake[0][0] = 10; body_snake[0][1] = 5;
    body_snake[1][0] = 9;  body_snake[1][1] = 5;
    body_snake[2][0] = 8;  body_snake[2][1] = 5;

    score = 0;
    snake_change_to = 2;
    snake_length = 3;
    key = RIGHT;
}

void draw_map()
{
    memset(map[0], '#', MAP_W);
    memset(map[MAP_H - 1], '#', MAP_W);

    map[0][MAP_W] = 0;
    map[MAP_H - 1][MAP_W] = 0;

    for(int i = 1; i < MAP_H - 1; i++)
    {
        memset(map[i], 32, MAP_W);
        map[i][0] = '#';
        map[i][MAP_W - 1] = '#';
        map[i][MAP_W] = 0;
    }

    for(int i = 0; i < snake_length; i++)
        map[body_snake[i][1]][body_snake[i][0]] = '#';

    map[yf][xf] = '*';
}

void display_map()
{
    for(int i = 0; i < MAP_H; i++)
        printf("%s\n", map[i]);
}

void insert_coord(int x, int y)
{
    for(int i = snake_length; i > 0; i--)
    {
        body_snake[i][0] = body_snake[i - 1][0];
        body_snake[i][1] = body_snake[i - 1][1];
    }

    body_snake[0][0] = x;
    body_snake[0][1] = y;
    snake_length++;
}

void pop_coord()
{
    snake_length--;
}

void generate_food()
{
    xf = rand() % (MAP_W - 2) + 1;
    yf = rand() % (MAP_H - 2) + 1;
}
