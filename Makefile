# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/09/23 19:42:48 by rbourgea          #+#    #+#              #
#    Updated: 2021/09/26 14:34:01 by rbourgea         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	webserv

SRCS	=	parser.cpp \
			webserv.cpp

OBJS	=	$(SRCS:.cpp=.o)

CC		=	g++

CFLAGS	=	-Wall -Werror -Wextra -std=c++98

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.cpp
			$(CC) $(CFLAGS) -o $@ -c $<

clean:
			rm -f $(OBJS)

fclean: 	clean
			rm -f $(NAME)

re: 		fclean all

.PHONY: 	clean all fclean re