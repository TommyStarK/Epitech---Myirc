/*
** server.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 04:32:00 2015 Emmanuel Chambon
** Last update Sun Apr 12 04:57:57 2015 Emmanuel Chambon
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

void			set_handler(t_server *serv)
{
  serv->cmd[0] = "NICK";
  serv->cmd[1] = "USER";
  serv->cmd[2] = "LIST";
  serv->cmd[3] = "JOIN";
  serv->cmd[4] = "PART";
  serv->cmd[5] = "NAMES";
  serv->cmd[6] = "PRIVMSG";
  serv->cmd[7] = "SENDFILE";
  serv->cmd[8] = "ACCEPTFILE";
  serv->cmd[9] = "QUIT";
  serv->cmd[10] = NULL;
  serv->cmd_handler[0] = &nick;
  serv->cmd_handler[1] = &user;
  serv->cmd_handler[2] = &list;
  serv->cmd_handler[3] = &join;
  serv->cmd_handler[4] = &part;
  serv->cmd_handler[5] = &names;
  serv->cmd_handler[6] = &msg;
  serv->cmd_handler[7] = &sendfile;
  serv->cmd_handler[8] = &acceptfile;
  serv->cmd_handler[9] = &quit;
  serv->cmd_handler[10] = NULL;
}

void			set_server(t_server *serv, char *port)
{
  int			i;

  serv->channels = NULL;
  serv->users_alone = NULL;
  serv->port = strdup(port);
  serv->socket = bind_port(port);
  for (i = 0; i < MAX_CONN; i++)
    serv->user_index[i] = NULL;
  FD_ZERO(&serv->master);
  FD_SET(serv->socket, &serv->master);
  set_handler(serv);
}

t_server		*init_server(char *port)
{
  t_server		*serv;
  struct sigaction	si;

  if (!(serv = malloc(sizeof(t_server))))
    error("malloc");
  set_server(serv, port);
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
  return (serv);
}
