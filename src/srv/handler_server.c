/*
** handler_server.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 04:50:53 2015 Emmanuel Chambon
** Last update Fri Apr 10 17:58:08 2015 Emmanuel Chambon
*/

#include "server.h"

void				handle_new_connection(int *max, t_server *serv)
{
  struct sockaddr_storage	r;
  socklen_t			len;
  t_user			*user;
  char				ip[INET6_ADDRSTRLEN];

  /* Faire degager
  if (serv->user_index[MAX_CONN - 1])
  */
  if (!(user = malloc(sizeof(t_user))))
    error("malloc");
  len = sizeof(r);
  printf("%d %d\n", serv->socket, len);
  if ((user->socket = accept(serv->socket, (struct sockaddr *)&r, &len)) == -1)
    error("accept");
  FD_SET(user->socket, &serv->master);
  if (user->socket > *max)
    *max = user->socket;
  user->ip = strdup(inet_ntop(r.ss_family,
			      ipvx((struct sockaddr *)&r),
			      ip, INET6_ADDRSTRLEN));
  user->rb = rb_init();
  user->nick = NULL;
  serv->user_index[user->socket] = user;
  users_push_back(user, &serv->users_alone);
}

void		remove_connection(int *i, t_server *s)
{
  t_channel	*c;
  t_user	*t;

  for (t = s->users_alone; t != NULL; t = t->next)
    {
      if (t == s->user_index[*i])
	{
	  s->users_alone = users_pop(s->user_index[*i], s->users_alone);
	  return ;
	}
    }
  for (c = s->channels; c != NULL; c = c->next)
    {
      for (t = s->users_alone; t; t = t->next)
	{
	  if (t == s->user_index[*i])
	    {
	      c->users = users_pop(s->user_index[*i], c->users);
	      return ;
	    }
	}
    }
}

void		handle_io_connection(int *i, int *max, t_server *serv)
{
  char		tmp[RB_SIZE];
  int		rc;

  memset(tmp, 0, RB_SIZE);
  if ((rc = recv(*i, tmp, rb_available(serv->user_index[*i]->rb), 0)) > 0)
    printf("%s\n", tmp);
  else
    {
      if (rc == 0)
	printf("%s Disconnected\n", (serv->user_index[*i]->nick)
	       ? serv->user_index[*i]->nick : serv->user_index[*i]->ip);
      else
      	perror("recv");
      remove_connection(i, serv);
      FD_CLR(*i, &serv->master);
    }
}

void		watch_sockets(int *i, int *max, fd_set *catch, t_server *serv)
{
  if (FD_ISSET(*i, catch))
    {
      if (*i == serv->socket)
	handle_new_connection(max, serv);
      else
      	handle_io_connection(i, max, serv);
    }
}

void		handler_server(t_server *serv)
{
  fd_set	catch;
  int		max;
  int		i;

  max = serv->socket;
  FD_ZERO(&catch);
  while (g_run)
    {
      catch = serv->master;
      if (select(max + 1, &catch, NULL, NULL, NULL) == -1)
	{
	  if (!g_run)
	    return ;
	  error("select");
	}
      for (i = 0; i <= max; i++)
	watch_sockets(&i, &max, &catch, serv);
    }
}
