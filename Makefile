# compiler shit
CC = g++
LIBS = -lm -lSDL -lSDL_image -lSDL_mixer
INCLUDE = -I. -I./src
# obj shit
OBJS := src/main.o src/bios.o src/eirin.o src/sinlut.o
NAME = eiyashou

%.o: %.c %.cpp
	$(CC) -c -o $@ $^ $(INCLUDE)

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm $(OBJS)
