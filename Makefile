NAME = playcmf

CC = gcc
#CFLAGS = -Wall -O3
CFLAGS = -g -O2 -Wall -std=c99 -fcommon
LIBS = -lm

SDL_CFLAGS = `sdl2-config --cflags`
SDL_LIBS = `sdl2-config --libs`

OBJS = playcmf.o fmdrv.o
OBJS += opl/opl.o

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBS) $(SDL_LIBS) -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(NAME)
