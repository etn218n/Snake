#ifndef SNAKEENGINE_H
#define SNAKEENGINE_H
/*-----------------------------------------------------------------------------*/
#include "CircularList.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h> 
/*-----------------------------------------------------------------------------*/
#define MAX_SNAKE_LENGTH 300
#define LEFT 12 // 1100
#define RIGHT 3 // 0011
#define UP  6   // 0110
#define DOWN 9  // 1001

#define BITE_ITSELF 50
#define HIT_WALL 51
#define SWALLOW_APPLE 52
#define MOVING 53

#define START 54
#define CONTINUE 55
#define END 56
/*-----------------------------------------------------------------------------*/
typedef struct vector2d{
	int x;
	int y;
} Vector2D;

typedef struct snakenode{
	struct vector2d pos;
	int isVisible;
	int direction;
} SnakeNode;

typedef struct Snake
{
	CircularList *body;
	Node *head;
	Node *tail;
	int length;
} Snake;

typedef struct scene{
	WINDOW *window;
	int maxHeight;
	int maxWidth;
	struct vector2d pos;
} Scene;

typedef struct apple{
	struct vector2d pos;
} Apple;
/*-----------------------------------------------------------------------------*/
int MAX_WINDOW_HEIGHT;
int MAX_WINDOW_WIDTH;
/*-----------------------------------------------------------------------------*/
Vector2D createVector2D(int x, int y);
int equalVector2D(Vector2D a, Vector2D b);

void initGame();
void runGame();
void gameOver(Scene *scene, Snake *snake);
void update();
void updateScore(Scene *scene, int score);
void updateSnake(Scene *scene, Snake *snake);
void updateNotice(Scene *scene);
void updateApple(Scene *scene, Apple *apple);

void drawSnake(Scene *scene, Snake *snake);
void transformSnake(Snake *snake, int direction);
void growSnake(Snake *snake);
int stateSnake(Scene *scene, Snake *snake, Apple *apple);

int checkBoundary(Scene *scene, Snake *snake);
int checkBiteItself(Snake *snake);
int checkSwallowApple(Snake *snake, Apple *apple);
void spawnApple(Scene *scene, Snake *snake, Apple *apple);

Snake* createSnake(Vector2D initPosition);
Scene* createScene(int maxHeight, int maxWidth, Vector2D initPosition);
Apple* createApple();

void deleteScene(Scene *scene);
/*-----------------------------------------------------------------------------*/
#endif