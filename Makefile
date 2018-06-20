CC      = cc
CFLAGS  = -Wall `sdl-config --cflags`
LDFLAGS = `sdl-config --libs` -lSDL_image -lSDL_ttf -lSDL_gfx
P_NAME  = game

$(P_NAME): main.c
	$(CC) $(CFLAGS) $(LDFLAGS) main.c -o $(P_NAME)

clean:
	rm $(P_NAME)

run: $(P_NAME)
	./$(P_NAME)

rebuild: clean $(P_NAME)
