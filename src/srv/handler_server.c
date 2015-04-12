/*
** handler_server.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 04:50:53 2015 Emmanuel Chambon
** Last update Sun Apr 12 12:49:29 2015 Emmanuel Chambon
*/

#include "server.h"

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
	  && rb_at(user->rb, -2) == '\r')
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
