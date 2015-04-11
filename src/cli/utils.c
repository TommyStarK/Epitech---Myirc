/*
** utils.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Sat Apr 11 18:16:31 2015 Emmanuel Chambon
** Last update Sat Apr 11 18:16:43 2015 Emmanuel Chambon
*/

#include "client.h"

void            free_it(char **it)
{
  int           i;

  i = 0;
  if (!i || !(*it))
    return ;
  while (it[i])
    {
      free(it[i]);
      ++i;
    }
  free(it);
}

void            free_arrays(char *fmt, ...)
{
  va_list       ap;
  char          *s;
  char          **t;
  t_request       *r;

  va_start(ap, fmt);
  while (*fmt++)
    {
      if (*fmt == 's')
	free(s = va_arg(ap, char *));
      else if (*fmt == 't')
	free_it(t = va_arg(ap, char **));
      else if (*fmt == 'r')
	free(r = va_arg(ap, t_request *));
    }
  va_end(ap);
}
