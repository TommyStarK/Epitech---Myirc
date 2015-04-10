/*
** server.c for  in /home/chambo_e/epitech/PSU_2014_myirc
1;2802;0c**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 04:32:00 2015 Emmanuel Chambon
** Last update Fri Apr 10 19:04:42 2015 Emmanuel Chambon
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
  return (sock);
}

void			close_handler(int sig)
{
  if (sig == SIGINT)
    g_run = 0;
}

void			release_server(t_server *serv)
{
  if (serv && serv->port)
    free(serv->port);
  if (serv && serv->socket)
    close(serv->socket);
  /* if (serv && serv->channels) */
  /*   release_channels(); */
  if (serv && serv->users_alone)
    user_release(serv->users_alone);
  if (serv)
    free(serv);
}

t_server		*init_server(char *port)
{
  t_server		*serv;
  struct sigaction	si;

  if (!(serv = malloc(sizeof(t_server))))
    error("malloc");
  serv->channels = NULL;
  serv->users_alone = NULL;
  serv->port = strdup(port);
  serv->socket = bind_port(port);
  FD_ZERO(&serv->master);
  FD_SET(serv->socket, &serv->master);
  g_run = 1;
  si.sa_handler = close_handler;
  sigemptyset(&si.sa_mask);
  si.sa_flags = SA_RESTART;
  if (sigaction(SIGINT, &si, NULL) == -1)
    {
      fprintf(stderr, "Warning: sigaction failed: %s\n", strerror(errno));
      fprintf(stderr, "Memory will not be freed and socket won't be close");
      fprintf(stderr, " at server close\n");
    }
  if (listen(serv->socket, BACKLOG))
    error("listen");
  return (serv);
}
