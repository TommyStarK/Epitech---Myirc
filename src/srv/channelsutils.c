/*
** usersutils.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 05:53:09 2015 Emmanuel Chambon
** Last update Sun Apr 12 10:10:52 2015 Emmanuel Chambon
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
