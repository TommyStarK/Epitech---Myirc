/*
** handler_server.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 04:50:53 2015 Emmanuel Chambon
** Last update Sun Apr 12 06:24:13 2015 Emmanuel Chambon
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
      for (t = s->users_alone; t; t = t->next)
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

  if ((cr = strstr(command, "\r\n")))
    *cr = 0;
  printf("command = <%s>\n", command);
}

int		substring_pattern(char *str, char *pattern)
{
  int		i;
  char		*res;

  i = 0;
  res = str;
  while ((res = strstr(res, pattern)))
    {
      res++;
      i++;
    }
  return (i);
}

void		input_interpret(t_user *user, t_server *serv)
{
  char		*input;
  char		*tok;

  input = rb_read(user->rb);
  if (substring_pattern(input, "\r\n") > 1)
    {
      tok = input;
      for (tok = strtok(tok, "\r\n"); tok; (tok = strtok(tok, "\r\n")))
	{
	  interpret_command(tok, user, serv);
	  tok = NULL;
	}
   }
  else
    interpret_command(input, user, serv);
  free(input);
}

void		handle_io_connection(t_user *user, t_server *serv)
{
  char		tmp[RB_SIZE];
  int		rc;

  memset(tmp, 0, RB_SIZE);
  if ((rc = recv(user->socket, tmp, rb_available(user->rb), 0)) > 0)
    {
      rb_write(user->rb, tmp);
      if (rb_at(user->rb, -1) == '\n'
	  && rb_at(user->rb, -1) == '\r')
	input_interpret(user, serv);
    }
  else
    {
      if (rc == 0)
	printf("%s Disconnected\n", (user->nick)
	       ? user->nick : user->ip);
      else
      	perror("recv");
      FD_CLR(user->socket, &serv->master);
      remove_connection(user, serv);
    }
}

void		watch_sockets(int *i, int *max, fd_set *catch, t_server *serv)
{
  if (FD_ISSET(*i, catch))
    {
      if (*i == serv->socket)
	handle_new_connection(max, serv);
      else
      	handle_io_connection(serv->user_index[*i], serv);
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
