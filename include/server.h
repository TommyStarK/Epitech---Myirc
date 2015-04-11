/*
** server.h for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 04:12:08 2015 Emmanuel Chambon
** Last update Fri Apr 10 19:13:55 2015 Emmanuel Chambon
*/

#ifndef _SERVER_H_
# define _SERVER_H_

# include "common.h"
# include "network.h"

# define MAX_CONN		128

typedef struct s_user		  t_user;
typedef struct s_channel	t_channel;
typedef struct s_server		t_server;

struct				s_user
{
  char				*ip;
  char				*nick;
  int				socket;
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
};

/*
**	glob
*/
int8_t				g_run;
/*
**	main.c
*/
int				usage();
void				error(const char *);
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
void				user_push_back(t_user *, t_user **);
void				user_destroy(t_user *);
t_user				*user_pop(t_user *, t_user *);
void				user_release(t_user *);

#endif /* !_SERVER_H_ */
