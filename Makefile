CFLAGS = -Wall -Wextra -Werror -pthread
CC = cc
SRC = main.c init.c
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

