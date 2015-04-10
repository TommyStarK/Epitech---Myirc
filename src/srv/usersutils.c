/*
** usersutils.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 05:53:09 2015 Emmanuel Chambon
** Last update Fri Apr 10 09:51:44 2015 Emmanuel Chambon
*/

#include "server.h"

void		users_push_back(t_user *node, t_user **list)
{
  t_user	*tmp;

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

void		user_destroy(t_user *user)
{
  if (user && user->ip)
    free(user->ip);
  if (user && user->nick)
    free(user->nick);
  if (user && user->socket)
    close(user->socket);
  if (user && user->rb)
    rb_free(user->rb);
  if (user)
    free(user);
}

t_user		*users_pop(t_user *user, t_user *container)
{
  t_user	*tmp;
  t_user	*backup;

  if (container == user)
    {
      tmp = user->next;
      user_destroy(user);
      return (tmp);
    }
  else
    {
      tmp = container;
      while (tmp != NULL)
	{
	  printf("%s\n", tmp->ip);
	  if (tmp == user)
	    {
	      backup->next = user->next;
	      user_destroy(user);
	      return (container);
	    }
	  backup = tmp;
	  tmp = tmp->next;
	}
    }
  return (NULL);
}
