/*
File: Menu.c
Author:	Vu Dinh Pham Duy
Description: only used for snake game. Hardcoded implementation of the game's main menu.
*/
#include "Menu.h"
/*--------------------------------------------------------------------------*/
int runMenu()
{
	initscr();
	curs_set(0);
	noecho();
	keypad(stdscr, true);

	Menu *menu = createMenu();
	int end = 0;

	while (end != 1)
	{
		printMenu(menu);
		int c = getch();
		switch(c)
		{
			case KEY_UP:
			case 'W':
			case 'w': moveCursor(menu, UP); break;

			case KEY_DOWN:
			case 'S':
			case 's': moveCursor(menu, DOWN); break;

			case 10:
			case 'F':
			case 'f': menu->fnc[menu->index](); end = 1; break; 

			default: break;
		}
		erase();
	}

	int state = stateMenu(menu);
	free(menu);
	endwin();

	return state;
}
/*--------------------------------------------------------------------------*/
void moveCursor(Menu *menu, int direction)
{
	switch(direction)
	{
		case DOWN: 
			if (menu->index == 2)
				menu->index = 0;
			else
				menu->index += 1;
			break;

		case UP: 
			if (menu->index == 0)
				menu->index = 2;
			else
				menu->index -= 1;
			break;

		default: break;
	}
}
/*--------------------------------------------------------------------------*/
void printMenu(Menu *menu)
{
	for (int i = 0; i < 3; i++)
		mvprintw(MAX_WINDOW_HEIGHT / 2 - (2 - i), MAX_WINDOW_WIDTH / 2 - 4, menu->entry[i]);

	mvprintw(MAX_WINDOW_HEIGHT / 2 - (2 - menu->index), MAX_WINDOW_WIDTH / 2 - 6, "> ");
	refresh();
}
/*--------------------------------------------------------------------------*/
int stateMenu(Menu *menu)
{
	int state;

	switch(menu->index)
	{
		case 0: state = CONTINUE; break;
		case 1: state = CONTINUE; break;
		case 2: state = END; break;
		default: break;
	}

	return state;
}
/*--------------------------------------------------------------------------*/
Menu* createMenu()
{
	Menu *menu = (Menu*)malloc(sizeof(Menu));

	menu->index = 0;
	menu->state = 0;
	menu->entry[0] = "Start";
	menu->entry[1] = "Help & About";
	menu->entry[2] = "Quit";
	menu->fnc[0] = runGame;
	menu->fnc[1] = helpnAbout;
	menu->fnc[2] = quitMenu;

	return menu;
}
/*--------------------------------------------------------------------------*/
void quitMenu()
{
	erase();
	halfdelay(6);
	mvprintw(MAX_WINDOW_HEIGHT / 2 - 1, MAX_WINDOW_WIDTH / 2 - 3, "Bye~~!");
	refresh();
	getch();
}
/*--------------------------------------------------------------------------*/
void helpnAbout()
{
	erase();
	const char *str[12];
	str[0]  = " Try to gain as many score as";
	str[1]  = " possible. Game over when the";
	str[2]  = " snake bites itself or hits  ";
	str[3]  = " any of the 4 walls.         ";
	str[4]  = " The maximum score is 300.   ";
	str[5]  = "                             ";
	str[6]  = "   Key          Action       ";
	str[7]  = " UP    / W     Move Up       ";
	str[8]  = " DOWN  / S     Move Down     ";
	str[9]  = " LEFT  / A     Move Left     ";
	str[10] = " RIGHT / D     Move Right    ";
	str[11] = " ENTER / F     Confirm       ";

	for (int i = 0; i < 12; i++)
	{
		mvprintw(i + 1, MAX_WINDOW_WIDTH / 2 - 15, "%s", str[i]);
	}

	mvprintw(MAX_WINDOW_HEIGHT - 1, MAX_WINDOW_WIDTH - 18, "powered by ncurses");

	refresh();
	getch();
}