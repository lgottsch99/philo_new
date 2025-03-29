# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: Watanudon <Watanudon@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/18 11:25:47 by lgottsch          #+#    #+#              #
#    Updated: 2025/03/29 20:45:31 by Watanudon        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=thread

SRC_FOLDER = ./src/
SRC = $(addprefix $(SRC_FOLDER), main.c input.c utils.c init.c get_set.c simulation.c sync_utils.c \
	simulation_helpers.c routine.c monitor.c lonely.c mac.c free.c)


OBJ = $(SRC:.c=.o)

$(NAME): re
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)
	make clean

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

	
.PHONY: all clean fclean re