/*
** main.c for MyIRC in
**
** Made by THOMAS MILOX
** Login   <loxmi@epitech.net>
**
** Started on  Fri Apr 10 18:05:41 2015 THOMAS MILOX
** Last update Fri Apr 10 21:20:14 2015 Emmanuel Chambon
*/

#include "client.h"

int             end_client(t_client *this, char *input)
{
  free(input);
  if (this->connected)
    close(this->client->fd);
  printf("Goodbye %s ;)\n", this->nickname);
  return (0);
}

int             connect_it(t_socket *s, const char *ip, const char *port)
{
  addinf  hints;
  addinf  *info;
  addinf  *l;
  int     ret;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if ((ret = getaddrinfo(ip, port, &hints, &info)) != 0)
      error(gai_strerror(ret));
  l = info;
  while (l)
    {
      if ((s->fd = socket(l->ai_family, l->ai_socktype, l->ai_protocol)) == -1)
        l = l->ai_next;
      if (connect(s->fd, l->ai_addr, l->ai_addrlen) == -1)
        l = l->ai_next;
      break ;
    }
  freeaddrinfo(info);
  return (!l ? 0 : 1);
}

t_socket				*init_client(t_client *this)
{
  t_socket			*ret;
  static t_fct	cmd[] = {
    {"/server", &connect_server},
    {"/nick", &change_nickname},
    {"/list", &list_channels},
    {"/join", &join_channel},
    {"/part", &leave_channel},
    {"/users", &list_users},
    {"/msg", &send_msg_to_user},
    {"/send_file", &send_file_to_user},
    {"/accept_file", &accept_file_from_user},
    {"/quit", &quit_client},
    {NULL, NULL}
  };
  if (!(ret = malloc(sizeof(t_socket))))
    error("malloc");
  this->cmd = cmd;
  this->connected = 0;
  this->nickname = getlogin();
  return (ret);
}

int 						main()
{
  int 					i;
  size_t 				n;
  char 					*input;
  t_client 			this;

  n = 0;
  input = NULL;
  this.client = init_client(&this);
  while (write(1, "irc> ", 5) && getline(&input, &n, stdin))
    {
      i = handle_cmd(&this, input);
      if (i == 9)
      	break ;
      else if (i < 0)
      	return (-1);
    }
  return (end_client(&this, input));
}
