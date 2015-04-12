/*
** handler_server.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 04:50:53 2015 Emmanuel Chambon
** Last update Sun Apr 12 02:11:51 2015 Emmanuel Chambon
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
  serv->user_index[user->socket] = user;
  if (serv->user_index[MAX_CONN - 1])
    refuse_new_connection(user, serv);
  else
    user_push(user, &serv->users_alone);
}

void		remove_connection(int *i, t_server *s)
{
  t_channel	*c;
  t_user	*t;

  for (t = s->users_alone; t != NULL; t = t->next)
    {
      if (t == s->user_index[*i])
	{
	  s->users_alone = user_pop(s->user_index[*i], s->users_alone);
	  s->user_index[*i] = NULL;
	  return ;
	}
    }
  for (c = s->channels; c != NULL; c = c->next)
    {
      for (t = s->users_alone; t; t = t->next)
	{
	  if (t == s->user_index[*i])
	    {
	      c->users = user_pop(s->user_index[*i], c->users);
	      s->user_index[*i] = NULL;
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
    {
      /* Ecrit le buffer tmp dans le buffer circulaire */
      rb_write(serv->user_index[*i]->rb, tmp);

      rb_delete_last(serv->user_index[*i]->rb);

      /* Affiche le buffer entier du client */
      /* printf("buffer =\n"); */
      rb_display(serv->user_index[*i]->rb);

      /* Lit le dernier message dans le buffer circulaire */
      char *res = rb_read(serv->user_index[*i]->rb);
      /* printf("read = \n<%s>", res); */
      free(res);
    }
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
