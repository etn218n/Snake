#ifndef MENU_H
#define MENU_H
/*-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SnakeEngine.h"
/*-----------------------------------------------------------------------------*/
typedef struct menu{
	int index;
	int state;
	const char *entry[3];
	void (*fnc[3])();
} Menu;
/*-----------------------------------------------------------------------------*/
void printMenu(Menu *menu);
void helpnAbout();
void quitMenu();
int runMenu();
int stateMenu(Menu *menu);
void moveCursor(Menu *menu, int direction);
Menu* createMenu();
/*-----------------------------------------------------------------------------*/
#endif