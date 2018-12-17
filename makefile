all:
	gcc -o Snake -Wall -Wextra -std=c11 MainGame.c SnakeEngine.c CircularList.c Menu.c -lncurses

MainGame.o:
	gcc -c MainGame.c