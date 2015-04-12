/*
** handler_server.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 04:50:53 2015 Emmanuel Chambon
** Last update Sun Apr 12 19:49:16 2015 Emmanuel Chambon
*/

#include "server.h"

void				refuse_new_connection(t_user *user, t_server *s)
{
  ssend(user->socket, "400 : Too much users connected\r\n");
  s->user_index[user->socket] = NULL;
  FD_CLR(user->socket, &s->master);
  user_destroy(user);
}

void				handle_new_connection(int *max, t_server *serv)
{
  struct sockaddr_storage	r;
  socklen_t			len;
  t_user			*user;
  char				ip[INET6_ADDRSTRLEN];

  if (!(user = malloc(sizeof(t_user))))
    error("malloc");
  len = sizeof(r);
  if ((user->socket = accept(serv->socket, (struct sockaddr *)&r, &len)) == -1)
    error("accept");
  FD_SET(user->socket, &serv->master);
  if (user->socket > *max)
    *max = user->socket;
  user->ip = strdup(inet_ntop(r.ss_family, ipvx((struct sockaddr *)&r),
			      ip, INET6_ADDRSTRLEN));
  user->rb = rb_init();
  user->nick = NULL;
  user->real = NULL;
  user->auth = 0;
  user->chan = 0;
  serv->user_index[user->socket] = user;
  if (serv->user_index[MAX_CONN - 1])
    refuse_new_connection(user, serv);
  else
    user_push(user, &serv->users_alone);
}

void		remove_connection(t_user *user, t_server *s)
{
  t_channel	*c;
  t_user	*t;

  for (t = s->users_alone; t != NULL; t = t->next)
    {
      if (t == user)
	{
	  s->user_index[user->socket] = NULL;
	  s->users_alone = user_pop(user, s->users_alone);
	  return ;
	}
    }
  for (c = s->channels; c != NULL; c = c->next)
    {
      for (t = c->users; t; t = t->next)
	{
	  if (t == user)
	    {
	      s->user_index[user->socket] = NULL;
	      c->users = user_pop(user, c->users);
	      return ;
	    }
	}
    }
}

void		interpret_command(char *command, t_user *user, t_server *serv)
{
  char		*cr;
  int		i;

  if ((cr = strstr(command, "\r\n")))
    *cr = 0;
  for (i = 0; i < MAX_CMD - 1; i++)
    {
      if (!(strncmp(command, serv->cmd[i], strlen(serv->cmd[i]))))
  	{
  	  serv->cmd_handler[i](strchrnul(command, ' '), user, serv);
  	  return ;
  	}
    }
  if (i == MAX_CMD - 1)
    def(command, user, serv);
}
