/*
** commands.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Sun Apr 12 04:58:15 2015 Emmanuel Chambon
** Last update Sun Apr 12 12:52:33 2015 Emmanuel Chambon
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
  if (user->nick)
    free(user->nick);
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

void	user(void *params, t_user *user, t_server *serv)
{
  int	i;
  int	x;
  char	n[256];
  char	host[256];
  char	s[256];
  char	name[256];

  x = 0;
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
      if (serv->user_index[i] && serv->user_index[i]->nick
	  && !strcmp(serv->user_index[i]->nick, n))
	{
	  x++;
	  if (x > 1)
	    {
	      ssend(user->socket, "462 USER : You may not reregister\r\n");
	      return ;
	    }
	}
    }
  send_greetings(user);
}

void		list(void *params, t_user *user, t_server *serv)
{
  t_channel	*tmp;
  char		*niddle;

  niddle = NULL;
  if (strlen(params) > 0)
    {
      params++;
      niddle = strdup(params);
    }
  ssend(user->socket, "321 List Start: Channel:Users Name\r\n");
  for (tmp = serv->channels; tmp; tmp = tmp->next)
    {
      if ((niddle && strstr(tmp->name, niddle)) || !niddle)
	{
	  ssend(user->socket, "322 ");
	  ssend(user->socket, tmp->name);
	  ssend(user->socket, "\n\r");
	}
    }
  ssend(user->socket, "323 End of /LIST\n\r");
}

void		join(void *params, t_user *user, t_server *serv)
{
  t_channel	*tmp;
  t_channel	*chan;

  if (!strlen(params))
    {
      ssend(user->socket, "461 JOIN : Not enough parameters\r\n");
      return ;
    }
  params++;
  if (((char *)params)[0] != '&' && ((char *)params)[0] != '#')
    {
      ssend(user->socket, "403 JOIN : Not such channel\r\n");
      return ;
    }
  for (tmp = serv->channels; tmp; tmp = tmp->next)
    {
      if (!strcmp(tmp->name, params))
	{
	  user_transfer(user, &tmp->users);
	  user->chan = strdup(tmp->name);
	  names(params, user, serv);
	  return ;
	}
    }
  if (!(chan = malloc(sizeof(t_channel))))
    error("malloc");
  chan->name = strdup(params);
  chan->users = NULL;
  user_transfer(user, &chan->users);
  user->chan = strdup(chan->name);
  channel_push(chan, &serv->channels);
  names(params, user, serv);
}

void		part(void *params, t_user *user, t_server *serv)
{
  t_channel	*tmp;
  t_user	*use;

  if (!strlen(params))
    {
      ssend(user->socket, "461 PART : Not enough parameters\r\n");
      return ;
    }
  params++;
  if (((char *)params)[0] != '&' && ((char *)params)[0] != '#')
    {
      ssend(user->socket, "403 PART : Not such channel\r\n");
      return ;
    }
  for (tmp = serv->channels; tmp; tmp = tmp->next)
    {
      if (!strcmp(tmp->name, params))
	{
	  for (use = tmp->users; use; use = use->next)
	    {
	      if (use == user)
		{
		  user_transfer(user, &serv->users_alone);
		  user->chan = 0;
		  return ;
		}
	    }
	  if (!use)
	    {
	      ssend(user->socket, "442 PART : You're not on that channel\r\n");
	      return ;
	    }
	}
    }
}

void	names(void _UNUSED_*params, t_user *user, t_server _UNUSED_*serv)
{
  t_user	*tmp;

  if (!user->chan)
    {
      ssend(user->socket, "442 NAMES : You're not on that channel\r\n");
      return ;
    }
  ssend(user->socket, "353 ");
  ssend(user->socket, user->chan);
  ssend(user->socket, ": [@");
  for (tmp = *user->list; tmp; tmp = tmp->next)
    {
      ssend(user->socket, tmp->nick);
      if (tmp->next)
	ssend(user->socket, " ");
    }
  ssend(user->socket, "]\r\n");
  ssend(user->socket, "366 End of /NAMES list\n\r");
}

void                            msg(void *params, t_user *user, t_server *serv) {}
void                            sendfile(void *params, t_user *user, t_server *serv) {}
void                            acceptfile(void *params, t_user *user, t_server *serv) {}

void	quit(void *params, t_user *user, t_server *serv)
{
  FD_CLR(user->socket, &serv->master);
  remove_connection(user, serv);
  return ;
}

void                            def(void *params, t_user *user, t_server *serv) {}
