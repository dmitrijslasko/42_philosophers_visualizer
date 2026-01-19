NAME = visualizer

SRC = src/main.c \
	  src/parsing.c \
	  src/renderer.c

INC_DIR = inc

all:
	@echo ">>> Compiling visualizer..."
	cc $(SRC) -o $(NAME) -I$(INC_DIR) -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -framework Cocoa -framework IOKit -framework OpenGL
clean:
	@echo ">>>Cleaning up..."
	rm -f visualizer	

run:
	./$(NAME)
	
PHONY: all clean