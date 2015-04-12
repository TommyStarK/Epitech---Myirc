/*
** server.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 04:32:00 2015 Emmanuel Chambon
** Last update Sun Apr 12 12:50:37 2015 Emmanuel Chambon
*/

#include "server.h"

int			bind_local(struct addrinfo *i, int *sock)
{
  int			trigger;

  trigger = 1;
  if ((*sock = socket(i->ai_family, i->ai_socktype, i->ai_protocol)) == -1)
    return (1);
  if (setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, &trigger, sizeof(trigger)))
    return (1);
  if (bind(*sock, i->ai_addr, i->ai_addrlen) == -1)
    {
      close(*sock);
      return (1);
    }
  return (0);
}

int			bind_port(char *port)
{
  struct addrinfo	h;
  struct addrinfo	*s;
  struct addrinfo	*i;
  int			sock;

  memset(&h, 0, sizeof(struct addrinfo));
  h.ai_family = AF_UNSPEC;
  h.ai_socktype = SOCK_STREAM;
  h.ai_flags = AI_PASSIVE;
  if (getaddrinfo(NULL, port, &h, &s))
    error("getaddrinfo");
  for (i = s; i != NULL; i = i->ai_next)
    {
      if (!bind_local(i, &sock))
	break ;
    }
  if (!i)
    error("bind");
  freeaddrinfo(s);
  if (listen(sock, BACKLOG))
    error("listen");
  return (sock);
}
