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


char								*change_nickname(void *a, void *b)
{
	char 							*ret;
	t_client 					*c;
	t_request 				*r;

	ret = NULL;
	c = (t_client *)a;
	r = (t_request *)b;
	if (r->arg && r->arg[0])
	{
		!(ret = malloc(256)) ? error("malloc") : bzero(ret, 256);
		sprintf(ret, "NICK %s\r\n", r->arg[0]);
		c->nickname = strdup(r->arg[0]);
		return (ret);
	}
	return (strdup("NICK\r\n"));
}

char								*list_channels(void __attribute__((unused))*a, void *b)
{
	char 							*ret;
	t_request 				*r;

	ret = NULL;
	r = (t_request *)b;
	if (r->arg && r->arg[0])
	{
		!(ret = malloc(256)) ? error("malloc") : bzero(ret, 256);
		sprintf(ret, "LIST %s\r\n", r->arg[0]);
		return (ret);
	}
	return (strdup("LIST\r\n"));
}

char								*join_channel(void *a, void *b)
{
	char 							*ret;
	t_client 					*c;
	t_request 				*r;

	ret = NULL;
	c = (t_client *)a;
	r = (t_request *)b;
	if (r->arg && r->arg[0])
	{
		!(ret = malloc(256)) ? error("malloc") : bzero(ret, 256);
		sprintf(ret, "JOIN %s\r\n", r->arg[0]);
		c->channel = strdup(r->arg[0]);
		return (ret);
	}
	return (strdup("JOIN\r\n"));
}

char								*leave_channel(void __attribute__((unused))*a, void *b)
{
	char 							*ret;
	t_request 				*r;

	ret = NULL;
	r = (t_request *)b;
	if (r->arg && r->arg[0])
	{
		!(ret = malloc(256)) ? error("malloc") : bzero(ret, 256);
		sprintf(ret, "PART %s\r\n", r->arg[0]);
		return (ret);
	}
	return (strdup("PART\r\n"));
}