#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <conio.h>
#include <time.h>

#include <pthread.h>

#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#endif

#ifdef linux
#define CLEAR_COMMAND "clear"
#endif

int score = 0;

int body_snake[100][2] = {{10, 5}, {9, 5}, {8, 5}};
int snake_change_to = 2;
int snake_length = 3;
int xf;
int yf;

char map[20][81];
char key;

void draw_map()
{
    sprintf(map[0], "################################################################################");
    for(int i = 1; i < 19; i++)
        sprintf(map[i], "#                                                                              #");
    sprintf(map[19], "################################################################################");

	for(int i = 0; i < snake_length; i++)
		map[body_snake[i][1]][body_snake[i][0]] = '#';

    map[yf][xf] = '*';
}

void display_map()
{
	for(int i = 0; i < 20; i++)
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
    xf = rand() % 78 + 1;
    yf = rand() % 18 + 1;
}

void * game(void *args) {
    srand(time(NULL));
    generate_food();

    while(1)
	{
		printf("\033[0;0H");
		draw_map();
		display_map();

		printf("Score: %d\n", score);

		if((key == 'w' || key == 'H') && snake_change_to != 3)
            snake_change_to = 1;
        else if((key == 'd' || key == 'M') && snake_change_to != 4)
            snake_change_to = 2;
        else if((key == 's' || key == 'P') && snake_change_to != 1)
            snake_change_to = 3;
        else if((key == 'a' || key == 'K') && snake_change_to != 2)
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
            exit(0);
        }

		usleep(40000);
	}


    return 0;
}

int main()
{
    pthread_t thread;
    int status;

    system(CLEAR_COMMAND);

    status = pthread_create(&thread, NULL, game, NULL);
    if (status != 0)
        exit(-11);

    do
    {
        key = getch();
    }
    while(key != 'q');

	return 0;
}
