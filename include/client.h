/*
** client.h for MyIRC in
**
** Made by THOMAS MILOX
** Login   <loxmi@epitech.net>
**
** Started on  Fri Apr 10 18:04:36 2015 THOMAS MILOX
** Last update Fri Apr 10 21:13:25 2015 Emmanuel Chambon
*/

#ifndef __CLIENT_H__
# define __CLIENT_H__

# include "common.h"
# include "network.h"

typedef struct addrinfo 				addinf;
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
	char 						connected;
	char						*nickname;
	t_fct						*cmd;
	t_socket				*client;
}									t_client;


/*
** main.c
*/

t_socket						*init_client(t_client *);
int             		connect_it(t_socket *, const char *, const char *);
int             		end_client(t_client *, char *);
/*
** cmd_handler.c
*/
int 								handle_cmd(t_client *, char *);
int 								known_cmd(t_client *, t_request *, int);
int 								unknown_cmd(t_client *, t_request *);
t_request       		*parse_cmd(char *);
void            		pars_hdl(t_request *, char **, char *, int);

/*
** commands.c
*/
char								*connect_server(void *, void *);
char								*change_nickname(void *, void *);
char								*list_channels(void *, void *);
char								*join_channel(void *, void *);
char								*leave_channel(void *, void *);

/*
** commands_bis.c
*/

char								*list_users(void *, void *);
char								*send_msg_to_user(void *, void *);
char								*send_file_to_user(void *, void *);
char								*accept_file_from_user(void *, void *);
char								*quit_client(void *, void *);

#endif /* ! __CLIENT_H__ */
