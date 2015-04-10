/*
** sockutils.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 05:16:53 2015 Emmanuel Chambon
** Last update Thu Apr  9 05:23:11 2015 Emmanuel Chambon
*/

#include "common.h"

void		*ipvx(struct sockaddr *s)
{
  if (s->sa_family == AF_INET)
    return &(((struct sockaddr_in*)s)->sin_addr);
  return &(((struct sockaddr_in6*)s)->sin6_addr);
}
