#include <stdio.h>
#include "SnakeEngine.h"
#include "Menu.h"

int main()
{
	initGame();

	if (MAX_WINDOW_HEIGHT < 15 || MAX_WINDOW_WIDTH < 32)
	{
		printf("Height: %d (Minimum: 15)\n", MAX_WINDOW_HEIGHT);
		printf("Width:  %d (Minimum: 32)\n", MAX_WINDOW_WIDTH);
		printf("Window size is too small.\n");
		return 0;
	}

	int state = START;

	while (state != END)
	{
		state = runMenu();
	}

	return 0;
}









