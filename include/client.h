/*
** client.h for MyIRC in
**
** Made by THOMAS MILOX
** Login   <loxmi@epitech.net>
**
** Started on  Fri Apr 10 18:04:36 2015 THOMAS MILOX
** Last update Sat Apr 11 18:18:45 2015 THOMAS MILOX
*/

#ifndef __CLIENT_H__
# define __CLIENT_H__

# include "common.h"
# include "network.h"

typedef struct addrinfo 				addinf;
typedef struct sockaddr_storage stor;

typedef struct    s_request
{
  char            *cmd;
  char            **arg;
}                 t_request;


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
	int 						*fdmax;
	char 						connected;
	char 						*channel;
	char						*nickname;
	t_fct						*cmd;
	fd_set 					*rfds;
	t_socket				*client;
	t_ring_buffer 	*rb;
	t_ring_buffer 	*rs;
}									t_client;

/*
** main.c
*/

t_socket						*init_client(t_client *);
void 								run(t_client *);
int 								handle_cmd(t_client *);
int             		read_answer(t_client *);
/*
** cmd_handler.c
*/
int 								known_cmd(t_client *, t_request *, int);
int 								unknown_cmd(t_client *, t_request *);
void 								unknown_cmd_handler(t_client *, t_request *);
t_request       		*parse_cmd(char *);
void            		pars_hdl(t_request *, char **, char *, int);
/*
** connect.c
*/
char								*connect_server(void *, void *);
int             		connect_it(t_socket *, const char *, const char *);
/*
** commands.c
*/
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
/*
** utils.c
*/
void	 							non_canon_mode(char);
char                **str_to_tab(char *, char);
void                free_it(char **);
void                free_arrays(char *, ...);

#endif /* ! __CLIENT_H__ */

