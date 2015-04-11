/*
** commands.c for MyIRC in
**
** Made by THOMAS MILOX
** Login   <loxmi@epitech.net>
**
** Started on  Fri Apr 10 18:28:54 2015 THOMAS MILOX
** Last update Fri Apr 10 18:29:01 2015 THOMAS MILOX
*/

#include "client.h"

char								*connect_server(void *this, void *request)
{
	char 							*ret;
	t_client					*c;
	t_request 				*r;

	ret = NULL;
	c = (t_client *)this;
	r = (t_request *)request;
	if (!r->arg || !r->arg[0] || !r->arg[1])
		return (ret = strdup("00PS: Error [/server] invalid cmd.\r\nCf help."));
	if (!connect_it(c->client, r->arg[0], r->arg[1]))
		return (ret = strdup("00PS: Error cannot connect specifed IP/PORT :(."));		
	c->connected = 1;
	if (!(ret = malloc(256)))
		error("malloc");
	bzero(ret, 256);
	sprintf(ret, "%s%c%s%c%s\r\n", "SERVER", ' ', r->arg[0], ' ', r->arg[1]);
	return (ret);
}

char								*change_nickname(void *a, void *b)
{
	a = a;
	b = b;
	return (NULL);
}

char								*list_channels(void *a, void *b)
{
	a = a;
	b = b;
	return (NULL);
}

char								*join_channel(void *a, void *b)
{
	a = a;
	b = b;
	return (NULL);
}

char								*leave_channel(void *a, void *b)
{
	a = a;
	b = b;
	return (NULL);
}