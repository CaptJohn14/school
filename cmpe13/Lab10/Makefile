all: rpg

rpg.o: rpg.c
	gcc -c rpg.c Game.c Player.c

rpg: rpg.o
	gcc -o rpg Player.c Game.c rpg.c

Player.o: Player.c
	gcc -c Player.c

Game.o: Game.c
	gcc -c Game.c

clean: 
	rm *.o rpg
