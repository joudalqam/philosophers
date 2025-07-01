CFLAGS = -Wall -Wextra -Werror -pthread -g3
CC = cc
SRC = main.c init.c utils.c valid_input.c routine.c free.c
OBJ = $(SRC:.c=.o)
NAME=philo

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

