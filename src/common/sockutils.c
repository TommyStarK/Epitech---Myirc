/*
** sockutils.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 05:16:53 2015 Emmanuel Chambon
** Last update Sat Apr 11 20:53:55 2015 Emmanuel Chambon
*/

#include "common.h"

void		*ipvx(struct sockaddr *s)
{
  if (s->sa_family == AF_INET)
    return &(((struct sockaddr_in*)s)->sin_addr);
  return &(((struct sockaddr_in6*)s)->sin6_addr);
}

void		ssend(int fd, char *data)
{
  int		len;

  len = strlen(data);
  snd(data, &len, fd);
}

void		snd(char *buf, int *len, int sock)
{
  int		tot;
  int		l;
  int		r;

  l = *len;
  tot = 0;
  while (tot < *len)
    {
      r = send(sock, buf + tot, l, 0);
      if (r == -1)
	{
	  perror("recv");
	  break ;
	}
      tot += r;
      l -= r;
    }
  *len = tot;
}
