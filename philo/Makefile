
NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -fsanitize=thread -g
#-fsanitize=thread -g

SRC = $(wildcard *.c)
OBJ_DIR = obj
OBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

debug: CFLAGS += -g
debug: re

$(OBJ_DIR)/%.o: %.c philo.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
