/*
File: SnakeEngine.c
Author:	Vu Dinh Pham Duy
Description: use circularly linked list to represent the snake and provide some functions to draw and control snake.
*/
#include "SnakeEngine.h"
/*-----------------------------------------------------------------------------*/
Vector2D createVector2D(int x, int y)
{
	Vector2D pos;
	pos.x = x;
	pos.y = y;
	return pos;
}

int equalVector2D(Vector2D a, Vector2D b)
{
	if (a.x == b.x && a.y == b.y)
		return true;

	return false;
}
/*-----------------------------------------------------------------------------*/
Snake* createSnake(Vector2D initPosition)
{
	Snake *snake = (Snake*)malloc(sizeof(Snake));
	snake->body = createCircularList();

	for (int i = 0; i < MAX_SNAKE_LENGTH; i++)
	{
		SnakeNode *newnode = (SnakeNode*)malloc(sizeof(SnakeNode));
		newnode->pos.x = -1; // default coordinate
		newnode->pos.y = -1; // outside the range of render viewport
		newnode->direction = 0;
		Node *listnode = createNode(newnode);
		add(snake->body, listnode);
	}

	snake->length = 3;
	snake->head = snake->body->head;
	snake->tail = snake->body->head->next->next;

	Node *temp = snake->head;

	// assign position and direction for head of the snake
	SnakeNode *currentnode = (SnakeNode*)temp->data;
	currentnode->pos.x = initPosition.x;
	currentnode->pos.y = initPosition.y;
	currentnode->direction = RIGHT;

	temp = temp->next;

	// assign position and direction for body of the snake
	currentnode = (SnakeNode*)temp->data;
	currentnode->pos.x = initPosition.x - 1;
	currentnode->pos.y = initPosition.y;
	currentnode->direction = RIGHT;

	temp = temp->next;

	// assign position and direction for tail of the snake
	currentnode = (SnakeNode*)temp->data;
	currentnode->pos.x = initPosition.x - 2;
	currentnode->pos.y = initPosition.y;
	currentnode->direction = RIGHT;

	return snake;
}

Apple* createApple()
{
	Apple *apple = (Apple*)malloc(sizeof(Apple));

	apple->pos.y = -1;
	apple->pos.x = -1;

	return apple;
}
/*-----------------------------------------------------------------------------*/
void drawSnake(Scene *scene, Snake *snake)
{
	SnakeNode *headNode = (SnakeNode*)snake->head->data;
	mvwprintw(scene->window, headNode->pos.y, headNode->pos.x, "0"); // draw head of the snake

	Node *current = snake->head->next; // head had been drawn, skip the head
	do // draw the rest of its body 
	{
		SnakeNode *currentNode = (SnakeNode*)current->data;

		mvwprintw(scene->window, currentNode->pos.y, currentNode->pos.x, "O");

		current = current->next;
	} while (current != snake->tail->next);
}

void transformSnake(Snake *snake, int direction)
{
	SnakeNode *headNode = (SnakeNode*)snake->head->data;

	if ((headNode->direction & direction) == 0) // e.g: LEFT & RIGHT = 0, TOP & DOWN = 0 but LEFT & TOP != 0
		direction = 15 - direction; // e.g if direction == RIGHT, direction = 15 - RIGHT = LEFT 

	SnakeNode *newNode = (SnakeNode*)snake->head->previous->data;
	newNode->direction = direction;

	switch(direction)
	{
		case RIGHT: newNode->pos.x = headNode->pos.x + 1;
					newNode->pos.y = headNode->pos.y; break;

		case LEFT:  newNode->pos.x = headNode->pos.x - 1;
					newNode->pos.y = headNode->pos.y; break;

		case UP:    newNode->pos.y = headNode->pos.y - 1;
					newNode->pos.x = headNode->pos.x; break;

		case DOWN:  newNode->pos.y = headNode->pos.y + 1;
					newNode->pos.x = headNode->pos.x; break;

		default: return;
	}

	// move head and tail pointer of the snake ,NOT head and tail pointer of its body (a CircularList)
	snake->head = snake->head->previous;
	snake->tail = snake->tail->previous;
}
/*-----------------------------------------------------------------------------*/
void growSnake(Snake *snake)
{
	if (snake->length == MAX_SNAKE_LENGTH - 1)
		return;

	snake->tail = snake->tail->next;
	snake->length += 1;
}
/*-----------------------------------------------------------------------------*/
int stateSnake(Scene *scene, Snake *snake, Apple *apple)
{
	if (checkBoundary(scene, snake) == HIT_WALL)
		return END;
	else if (checkBiteItself(snake) == BITE_ITSELF)
		return END;
	else if (checkSwallowApple(snake, apple) == SWALLOW_APPLE)
	{
		growSnake(snake);
		spawnApple(scene, snake, apple);
		return CONTINUE;
	}

	return CONTINUE;
}
/*-----------------------------------------------------------------------------*/
void spawnApple(Scene *scene, Snake *snake, Apple *apple)
{
	int valid = 0;
	srand (time(NULL));

	while (valid == 0)
	{
		int y = rand() % (scene->maxHeight - 2) + 1;
		int x = rand() % (scene->maxWidth - 2) + 1;

		apple->pos.y = y;
		apple->pos.x = x;

		Node *current = snake->head;

		do
		{
			valid = 1;

			SnakeNode *currentnode = (SnakeNode*)current->data;

			if (equalVector2D(apple->pos, currentnode->pos) == true) // find new position if apple's spawn location 
			{														 // equals any part of the snake
				valid = 0;
				break; // stop traverse the list, get out and find new position
			}

			current = current->next;
		} while (current != snake->tail->next);
	}
}
/*-----------------------------------------------------------------------------*/
int checkBoundary(Scene *scene, Snake *snake)
{
	SnakeNode *node = (SnakeNode*)snake->head->data;

	if (node->pos.y == 0 || node->pos.y == scene->maxHeight - 1 || node->pos.x == 0 || node->pos.x == scene->maxWidth - 1)
		return HIT_WALL;

	return MOVING;
}
/*-----------------------------------------------------------------------------*/
int checkBiteItself(Snake *snake)
{
	Node *current = snake->head->next;
	SnakeNode *headnode = (SnakeNode*)snake->head->data;

	do
	{
		SnakeNode *currentnode = (SnakeNode*)current->data;

		if (equalVector2D(headnode->pos, currentnode->pos) == true)
			return BITE_ITSELF;

		current = current->next;
	} while (current != snake->tail->next);

	return MOVING;
}
/*-----------------------------------------------------------------------------*/
int checkSwallowApple(Snake *snake, Apple *apple)
{
	SnakeNode *headNode = (SnakeNode*)snake->head->data;

	if (equalVector2D(headNode->pos, apple->pos) == true)
		return SWALLOW_APPLE;

	return MOVING;
}
/*-----------------------------------------------------------------------------*/
Scene* createScene(int maxHeight, int maxWidth, Vector2D initPosition)
{
	Scene *scene = (Scene*)malloc(sizeof(Scene));
	scene->window = newwin(maxHeight, maxWidth, initPosition.y, initPosition.x);
	scene->maxWidth = maxWidth;
	scene->maxHeight = maxHeight;
	scene->pos.y = initPosition.y;
	scene->pos.x = initPosition.x;
	
	return	scene;
}
/*-----------------------------------------------------------------------------*/
void updateScore(Scene *scorescene, int score)
{
	box(scorescene->window, 0, 0);
	mvwprintw(scorescene->window, 1, 1, "Score: %d", score);
	wrefresh(scorescene->window);
}
/*-----------------------------------------------------------------------------*/
void updateSnake(Scene *Scene, Snake *snake)
{
	box(Scene->window, 0, 0);
	drawSnake(Scene, snake);
	wrefresh(Scene->window);
}
/*-----------------------------------------------------------------------------*/
void updateNotice(Scene *noticescene)
{
	box(noticescene->window, 0, 0);
	mvwprintw(noticescene->window, 1, 1, "Just to be here!");
	wrefresh(noticescene->window);
}
/*-----------------------------------------------------------------------------*/
void updateApple(Scene *Scene, Apple *apple)
{
	mvwprintw(Scene->window, apple->pos.y, apple->pos.x, "*");
	wrefresh(Scene->window);
}
/*-----------------------------------------------------------------------------*/
void initGame()
{
	initscr();
	getmaxyx(stdscr, MAX_WINDOW_HEIGHT, MAX_WINDOW_WIDTH);
	endwin(); 
}
/*-----------------------------------------------------------------------------*/
void runGame()
{	
	initscr();
	curs_set(0);
	noecho();
	refresh();
	nodelay(stdscr, true);
	keypad(stdscr, true);

	Snake *snake = createSnake(createVector2D(4, 3));
	Apple *apple = createApple();
	Scene *scoreScene = createScene(3, 12, createVector2D(0, 0));
	Scene *noticeScene = createScene(3, MAX_WINDOW_WIDTH - 12, createVector2D(12, 0));
	Scene *gameScene = createScene(MAX_WINDOW_HEIGHT - 4, MAX_WINDOW_WIDTH, createVector2D(0, 3));
	
	spawnApple(gameScene, snake, apple);
	int state = START;
	int direction = RIGHT;
	int tick = 0;
	int delay = 60000;

	while (state != END)
	{
		int c = getch();

		switch(c)
		{	
			case KEY_UP:
			case 'W':
			case 'w': direction = UP; break;

			case KEY_DOWN:
			case 'S':
			case 's': direction = DOWN; break;

			case KEY_RIGHT:
			case 'D':
			case 'd': direction = RIGHT; break;

			case KEY_LEFT:
			case 'A':
			case 'a': direction = LEFT; break;

			default: break;
		}

		if (tick % delay == 0)
		{
			updateScore(scoreScene, snake->length - 3);
			updateNotice(noticeScene);
			updateSnake(gameScene, snake);
			updateApple(gameScene, apple);
			transformSnake(snake, direction);
			werase(gameScene->window);
			werase(scoreScene->window);
			werase(noticeScene->window);
		}

		tick++;
		state = stateSnake(gameScene, snake, apple);
	} 

	gameOver(gameScene, snake);

	deleteAll(snake->body);
	deleteScene(gameScene);
	deleteScene(scoreScene);
	deleteScene(noticeScene);
	free(apple);

	endwin(); 
}
/*-----------------------------------------------------------------------------*/
void deleteScene(Scene *scene)
{
	delwin(scene->window);
	free(scene);
}
/*-----------------------------------------------------------------------------*/
void gameOver(Scene *scene, Snake *snake)
{
	updateSnake(scene, snake); // print the last frame
	halfdelay(3); // wait for 0.3s
	getch();
	cbreak(); // turn off haldelay mode for input
	nodelay(stdscr, false);
	erase();
	mvprintw(MAX_WINDOW_HEIGHT / 2 - 2, MAX_WINDOW_WIDTH / 2 - 4, "Game Over");
	mvprintw(MAX_WINDOW_HEIGHT / 2 - 1, MAX_WINDOW_WIDTH / 2 - 4, "Score: %d", snake->length - 3);
	mvprintw(MAX_WINDOW_HEIGHT - 1, MAX_WINDOW_WIDTH / 2 - 15, "Press 'Q' or ENTER to continue." );

	int c =getch();
	while (c != 'q' && c != 'Q' && c != 10)
		c = getch();
}
/*-----------------------------------------------------------------------------*/