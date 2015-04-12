##
## Makefile for  in /home/chambo_e/epitech/PSU_2014_myirc
## 
## Made by Emmanuel Chambon
## chambo_e  <chambon.emmanuel@gmail.com>
## 
## Started on  Wed Apr  8 22:49:16 2015 Emmanuel Chambon
## Last update Sun Apr 12 20:06:10 2015 Emmanuel Chambon
##

CC		=	gcc -g

INC		=	include

CFLAGS		=	-W -Wall -Wextra -I$(INC) -D_GNU_SOURCE

RM		=	rm -f

SERVER		=	src/srv

CLIENT		=	src/cli

COMMON		=	src/common

NAME_SERVER	=	server

NAME_CLIENT	=	client

SRC_SERVER	=	$(SERVER)/main.c		\
			$(SERVER)/server.c		\
			$(SERVER)/handler_server.c	\
			$(SERVER)/handler_client.c	\
			$(SERVER)/usersutils.c		\
			$(SERVER)/channelsutils.c	\
			$(SERVER)/socketutils.c		\
			$(SERVER)/commands.c		\
			$(SERVER)/file_related.c	\
			$(SERVER)/channels_related.c	\
			$(SERVER)/misc_related.c	\
			$(SERVER)/welcoming.c		\
			$(COMMON)/rbutils.c		\
			$(COMMON)/strtowordtab.c	\
			$(COMMON)/rbinit.c		\
			$(COMMON)/utils.c		\
			$(COMMON)/sockutils.c

SRC_CLIENT	=	$(CLIENT)/main.c		\
			$(CLIENT)/commands.c		\
			$(CLIENT)/cmd_handler.c		\
			$(CLIENT)/connect.c		\
			$(CLIENT)/commands_bis.c	\
			$(CLIENT)/utils.c		\
			$(COMMON)/utils.c		\
			$(COMMON)/strtowordtab.c	\
			$(COMMON)/rbutils.c		\
			$(COMMON)/rbinit.c		\
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
