/*
** usersutils.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 05:53:09 2015 Emmanuel Chambon
** Last update Sun Apr 12 19:48:12 2015 Emmanuel Chambon
*/

#include "server.h"

void		channel_push(t_channel *node, t_channel **list)
{
  t_channel	*tmp;

  tmp = *list;
  if (!tmp)
    *list = node;
  else
    {
      while (tmp->next)
	tmp = tmp->next;
      tmp->next = node;
    }
  node->next = NULL;
}

void		channel_destroy(t_channel *channel)
{
  if (channel && channel->name)
    free(channel->name);
  if (channel && channel->users)
    user_release(channel->users);
  if (channel)
    free(channel);
}

void		channel_release(t_channel *channel)
{
  t_channel	*tmp;
  t_channel	*backup;

  tmp = channel;
  while (tmp)
    {
      backup = tmp->next;
      channel_destroy(tmp);
      tmp = backup;
    }
}

t_channel	*channel_pop(t_channel *channel, t_channel *container)
{
  t_channel	*tmp;
  t_channel	*backup;

  if (container == channel)
    {
      tmp = channel->next;
      channel_destroy(channel);
      return (tmp);
    }
  else
    {
      for (tmp = container; tmp != NULL; tmp = tmp->next)
	{
	  if (tmp == channel)
	    {
	      backup->next = channel->next;
	      channel_destroy(channel);
	      return (container);
	    }
	  backup = tmp;
	}
    }
  return (NULL);
}
