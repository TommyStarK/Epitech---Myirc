##
## Makefile for  in /home/chambo_e/epitech/PSU_2014_myirc
## 
## Made by Emmanuel Chambon
## chambo_e  <chambon.emmanuel@gmail.com>
## 
## Started on  Wed Apr  8 22:49:16 2015 Emmanuel Chambon
## Last update Sun Apr 12 02:17:57 2015 THOMAS MILOX
##

CC		=	gcc -g

INC		=	include

CFLAGS		=	-W -Wall -Wextra -I$(INC)

RM		=	rm -f

SERVER		=	src/srv

CLIENT		=	src/cli

COMMON		=	src/common

NAME_SERVER	=	server

NAME_CLIENT	=	client

SRC_SERVER	=	$(SERVER)/main.c		\
			$(SERVER)/server.c		\
			$(SERVER)/handler_server.c	\
			$(SERVER)/usersutils.c		\
			$(COMMON)/rbutils.c		\
			$(COMMON)/sockutils.c

SRC_CLIENT	=	$(CLIENT)/main.c		\
			$(CLIENT)/commands.c		\
			$(CLIENT)/cmd_handler.c		\
			$(CLIENT)/connect.c			\
			$(CLIENT)/commands_bis.c	\
			$(CLIENT)/utils.c		\
			$(COMMON)/utils.c		\
			$(COMMON)/strtowordtab.c	\
			$(COMMON)/rbutils.c		\
			$(COMMON)/sockutils.c

OBJS		=	$(SRC_SERVER:.c=.o)

OBJC		=	$(SRC_CLIENT:.c=.o)


all		:
			@$(MAKE) --no-print-directory $(NAME_SERVER)
			@$(MAKE) --no-print-directory $(NAME_CLIENT)

server		:	$(OBJS)
			$(CC) $(CFLAGS) $(OBJS) -o $(NAME_SERVER)

client		:	$(OBJC)
			$(CC) $(CFLAGS) $(OBJC) -o $(NAME_CLIENT) -lncurses

clean		:
			$(RM) $(OBJS) $(OBJC)

fclean		:	clean
			$(RM) $(NAME_SERVER) $(NAME_CLIENT)

re		:	fclean all
