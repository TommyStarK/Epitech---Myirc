/*
** connect.c for MyIRC in
**
** Made by THOMAS MILOX
** Login   <loxmi@epitech.net>
**
** Started on  Sat Apr 11 21:49:24 2015 THOMAS MILOX
** Last update Sun Apr 12 13:10:56 2015 Emmanuel Chambon
*/

#include "client.h"

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
      error("getaddrinfo");
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

char 								*connect_server_handler(t_client *c)
{
	char 							*ret;

	ret = NULL;
	c->connected = 1;
	*c->fdmax = c->client->fd + 1;
	FD_SET(c->client->fd, c->rfds);
	!(ret = malloc(256)) ? error("malloc") : bzero(ret, 256);
	sprintf(ret, "%s %s\r\n", "NICK", c->nickname);
	ssend(c->client->fd, ret);
	free(ret);
	!(ret = malloc(256)) ? error("malloc") : bzero(ret, 256);
	sprintf(ret, "%s %s 0 * :%s\r\n", "USER", c->nickname, c->nickname);
	return (ret);
}

char								*connect_server(void *this, void *request)
{
	char 							*ret;
	char 							**tmp;
	t_client					*c;
	t_request 				*r;

	ret = NULL;
	c = (t_client *)this;
	r = (t_request *)request;
	if (!r->arg || !r->arg[0])
		return (strdup("00PS: Error [/server] invalid cmd.\r\nCf help."));
	if (!(tmp = str_to_tab(r->arg[0], ':')))
		return (strdup("00PS: str_to_tab() failed :(."));
	r->arg = tmp;
	free_arrays("t", tmp);
	if (!r->arg[1])
	{
		if (!(r->arg = realloc(r->arg, sizeof(r->arg) + sizeof(char *) * 2)))
			error("realloc");
		r->arg[1] = strdup("6667");
		r->arg[2] = NULL;
	}
	if (!connect_it(c->client, r->arg[0], r->arg[1]))
		return (strdup("00PS: Error cannot connect specifed IP/PORT :(."));
	ret = connect_server_handler(c);
	return (ret);
}
