/*
** utils.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Sat Apr 11 18:16:31 2015 EMMANUEL CHAMBON
** Last update Sat Apr 11 18:16:43 2015 THOMAS MILOX
*/

#include "client.h"

void            non_canon_mode(char c)
{
  struct termios t;

  tcgetattr(STDIN_FILENO, &t);
  if (!c)
  {
    t.c_lflag &= ~ICANON;
    t.c_lflag &= ~ECHO;
    t.c_cc[VMIN] = 1;

  }
  else if (c == 1)
    {
      t.c_lflag |= ICANON;
      t.c_lflag |= ECHO;
    }
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

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
