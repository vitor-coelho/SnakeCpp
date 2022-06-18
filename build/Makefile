PROG  = test
CC    = g++
FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -I..\include -Wall
SRCS  = ..\main.cpp ..\src\game.cpp ..\src\snake.cpp
OBJS  = main.o	game.o 	 snake.o

$(PROG):
	$(CC) -c $(SRCS) $(FLAGS)
	$(CC) $(OBJS) $(FLAGS) -o $(PROG)

clean:
	del $(PROG).exe $(OBJS)


# main.o :
# $(CC) $(CPPFLAGS) -c main.cpp

# blurb.o : blurb.h
# $(CC) $(CPPFLAGS) -c blurb.cpp

# database.o : database.h
# $(CC) $(CPPFLAGS) -c database.cpp

# clean:
# rm -f core $(PROG) $(OBJS)
