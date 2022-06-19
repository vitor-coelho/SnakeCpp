PROG  = test
CC    = g++
FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -I..\include -Wall
SRCS  = ..\main.cpp ..\src\game.cpp ..\src\snake.cpp ..\src\nn.cpp ..\src\matrix.cpp ..\src\gen-algo.cpp
OBJS  = main.o game.o snake.o nn.o matrix.o gen-algo.o

$(PROG):
	$(CC) -c $(SRCS) $(FLAGS)
	$(CC) $(OBJS) $(FLAGS) -o $(PROG)

clean:
	del $(PROG).exe $(OBJS)
