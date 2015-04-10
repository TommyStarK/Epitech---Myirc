/*
** client.h for MyIRC in
**
** Made by THOMAS MILOX
** Login   <loxmi@epitech.net>
**
** Started on  Fri Apr 10 18:04:36 2015 THOMAS MILOX
** Last update Fri Apr 10 18:05:01 2015 THOMAS MILOX
*/

#ifndef __CLIENT_H__
# define __CLIENT_H__

# include <errno.h>
# include <netdb.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <pthread.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/param.h>
# include <sys/socket.h>
# include <sys/sendfile.h>

# define ERR_INIT_CLIENT 				"Error: at initialization of client."


# define ERROR(s)								(fprintf(stderr, "%s\n", s))

/*
**
*/

typedef struct sockaddr_storage stor;

typedef struct		s_fct
{
	char						*cmd;
	char						*(*op)(void *, void *);
}									t_fct;


typedef struct		s_socket
{
	int							fd;
	stor						addr;
	socklen_t				len;
}									t_socket;

typedef struct		s_client
{
	t_fct						*cmd;
	t_socket				*client;
}									t_client;


char								*connect_server(void *, void *);
char								*change_nickname(void *, void *);
char								*list_channels(void *, void *);
char								*join_channel(void *, void *);
char								*leave_channel(void *, void *);
char								*list_users(void *, void *);
char								*send_msg_to_user(void *, void *);
char								*send_file_to_user(void *, void *);
char								*accept_file_from_user(void *, void *);

#endif /* ! __CLIENT_H__ */
