/*
** commands.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Sun Apr 12 04:58:15 2015 Emmanuel Chambon
** Last update Sun Apr 12 20:08:43 2015 Emmanuel Chambon
*/

#include "server.h"

void	check_and_send_private(char **args, t_user *user, t_server *serv)
{
  int	i;
  int	x;

  for (i = 3; i < MAX_CONN; i++)
    {
      if (serv->user_index[i] && serv->user_index[i]->nick
	  && !strcmp(serv->user_index[i]->nick, args[0]))
	{
	  ssend(serv->user_index[i]->socket, user->nick);
	  ssend(serv->user_index[i]->socket, ": ");
	  for (x = 1; args[x]; x++)
	    {
	      ssend(serv->user_index[i]->socket, args[x]);
	      if (args[x + 1])
		ssend(serv->user_index[i]->socket, " ");
	    }
	  ssend(serv->user_index[i]->socket, "\r\n");
	  break ;
	}
    }
  if (i == MAX_CONN)
    ssend(user->socket, "401 No such nick\r\n");
  for (x = 0; args[x]; x++)
    free(args[x]);
  free(args);
}

void	msg(void *params, t_user *user, t_server *serv)
{
  char	**args;

  if (!strlen(params) || !++params || !(args = str_to_tab(params, ' ')))
    {
      ssend(user->socket, "461 PART : Not enough parameters\r\n");
      return ;
    }
  if (!args[1])
    {
      ssend(user->socket, "412 No text to send\r\n");
      return ;
    }
  check_and_send_private(args, user, serv);
}

void		in_chat_response(char *params, t_user *user)
{
  t_user	*u;

  if (((char *)params)[0] == '/')
    ssend(user->socket, "421 Unknown command\r\n");
  else
    {
      for (u = *user->list; u; u = u->next)
	{
	  if (u == user)
	    continue ;
	  ssend(u->socket, user->nick);
	  ssend(u->socket, ": ");
	  ssend(u->socket, ((char *)params));
	  ssend(u->socket, "\r\n");
	}
    }
}

void		def(void *params, t_user *user, t_server _UNUSED_*serv)
{
  if (!user->chan)
    {
      if (((char *)params)[0] == '/')
	ssend(user->socket, "421 Unknown command\r\n");
      else
	ssend(user->socket, "441 User not in channel\r\n");
    }
  else
    in_chat_response(params, user);
}
