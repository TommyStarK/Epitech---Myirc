/*
** commands.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Sun Apr 12 04:58:15 2015 Emmanuel Chambon
** Last update Sun Apr 12 20:07:48 2015 Emmanuel Chambon
*/

#include "server.h"

void	nick(void *params, t_user *user, t_server _UNUSED_*serv)
{
  int	i;

  if (!strlen(params))
    {
      ssend(user->socket, "431 NICK : No nickname given\r\n");
      return ;
    }
  params++;
  for (i = 3; i < MAX_CONN; i++)
    {
      if (serv->user_index[i] && serv->user_index[i]->nick
	  && !strcmp(serv->user_index[i]->nick, params))
	{
	  ssend(user->socket, "433 NICK : Nickname is already in use\r\n");
	  return ;
	}
    }
  if (strlen(params) > 9)
    {
      ssend(user->socket, "432 NICK : Erroneus nickname\r\n");
      return ;
    }
  if (user->nick)
    free(user->nick);
  user->nick = strdup(params);
}

void	send_greetings(t_user *user)
{
  dprintf(user->socket,
	  "001 %s :Welcome to the Internet Relay Network %s!\r\n",
	  user->nick, user->nick);
  dprintf(user->socket,
	  "002 %s :Your host is man.u.irc.net, running manirc-0.1!\r\n",
	  user->nick);
  dprintf(user->socket,
	  "003 %s :This server has been started this f*** day!\r\n",
	  user->nick);
}

int	check_registered(int i, t_user *user, t_server *serv, char *n)
{
  int	x;

  x = 0;
  if (serv->user_index[i] && serv->user_index[i]->nick
      && !strcmp(serv->user_index[i]->nick, n))
    {
      x++;
      if (x > 1)
	{
	  ssend(user->socket, "462 USER : You may not reregister\r\n");
	  return (1);
	}
    }
  return (0);
}

void	user(void *params, t_user *user, t_server *serv)
{
  int	i;
  char	n[256];
  char	host[256];
  char	s[256];
  char	name[256];

  memset(n, 0, 256);
  if (!user->nick)
    return ;
  if ((sscanf(params, "%s %s %s %s", n, host, s, name)) != 4)
    {
      ssend(user->socket, "461 USER : Not enough parameters\r\n");
      return ;
    }
  for (i = 3; i < MAX_CONN; i++)
    {
      if (check_registered(i, user, serv, n))
	return ;
    }
  send_greetings(user);
}
