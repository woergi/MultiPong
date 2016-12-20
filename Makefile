CC=g++-6
CFLAGS=-I ~/Programming/SFML-2.4.1/include/ -g
LDFLAGS=-l sfml-graphics -l sfml-window -l sfml-system -L ~/Programming/SFML-2.4.1/include//../lib
OBJ= World.o Player.o Ball.o main.o Scene.o StateStack.o Menu.o

mulitpong: $(OBJ)
	$(CC) $(CFLAGS) -o multipong $(OBJ) $(LDFLAGS)

World.o: World.cpp *.h
	$(CC) $(CFLAGS) -c World.cpp

Player.o: Player.cpp *.h
	$(CC) $(CFLAGS) -c Player.cpp

main.o: main.cpp *.h
	$(CC) $(CFLAGS) -c main.cpp

Ball.o: Ball.cpp *.h
	$(CC) $(CFLAGS) -c Ball.cpp

Scene.o: Scene.cpp *.h
	$(CC) $(CFLAGS) -c Scene.cpp

StateStack.o: StateStack.cpp *.h
	$(CC) $(CFLAGS) -c StateStack.cpp

Menu.o: Menu.cpp *.h
	$(CC) $(CFLAGS) -c Menu.cpp

clean:
	rm *.o multipong

