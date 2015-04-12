/*
** server.h for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 04:12:08 2015 Emmanuel Chambon
** Last update Sun Apr 12 04:56:53 2015 Emmanuel Chambon
*/

#ifndef _SERVER_H_
# define _SERVER_H_

# include "common.h"
# include "network.h"

# define MAX_USERS		128
# define MAX_CONN		MAX_USERS + 5

typedef struct s_user		t_user;
typedef struct s_channel	t_channel;
typedef struct s_server		t_server;

struct				s_user
{
  char				auth;
  int				socket;
  char				*ip;
  char				*nick;
  char				*real;
  t_ring_buffer			*rb;
  t_user			*next;
};

struct				s_channel
{
  t_user			*users;
  t_channel			*next;
};

struct				s_server
{
  char				*port;
  int				socket;
  t_channel			*channels;
  t_user			*users_alone;
  t_user			*user_index[MAX_CONN];
  fd_set			master;
  char				*cmd[11];
  void				*cmd_handler[11];
};

/*
**	glob
*/
int8_t				g_run;
/*
**	main.c
*/
int				usage();
/*
**	server.c
*/
int				bind_local(struct addrinfo *, int*);
int				bind_port(char *);
void				close_handler(int);
void				release_server(t_server *);
t_server			*init_server(char *);
/*
**	handler_server.c
*/
void				handler_server(t_server *);
/*
**      usersutils.c
*/
void				user_push(t_user *, t_user **);
t_user				*user_pop(t_user *, t_user *);
void				user_destroy(t_user *);
void				user_release(t_user *);
/*
**	commands.c
*/
void				nick(void *, t_user *);
void				user(void *, t_user *);
void				list(void *, t_user *);
void				join(void *, t_user *);
void				part(void *, t_user *);
void				names(void *, t_user *);
void				msg(void *, t_user *);
void				sendfile(void *, t_user *);
void				acceptfile(void *, t_user *);
void				quit(void *, t_user *);
void				def(void *, t_user *);

#endif /* !_SERVER_H_ */
