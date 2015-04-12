/*
** commands.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Sun Apr 12 04:58:15 2015 Emmanuel Chambon
** Last update Sun Apr 12 20:06:45 2015 Emmanuel Chambon
*/

#include "server.h"

void		allocate_channel(t_user *user, t_server *serv, char *params)
{
  t_channel	*tmp;
  t_channel	*chan;

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
  if (user->chan)
    free(user->chan);
  user->chan = strdup(chan->name);
  channel_push(chan, &serv->channels);
  names(params, user, serv);
}

void		join(void *params, t_user *user, t_server *serv)
{
  if (!user->nick)
    return ;
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
  allocate_channel(user, serv, params);
}

void		departure_channel(t_user *user, t_server *serv, char *params)
{
  t_channel	*tmp;
  t_user	*use;

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

void		part(void *params, t_user *user, t_server *serv)
{
  if (!user->nick)
    return ;
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
  departure_channel(user, serv, params);
}
