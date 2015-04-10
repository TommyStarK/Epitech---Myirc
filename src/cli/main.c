/*
** main.c for MyIRC in
**
** Made by THOMAS MILOX
** Login   <loxmi@epitech.net>
**
** Started on  Fri Apr 10 18:05:41 2015 THOMAS MILOX
** Last update Fri Apr 10 18:05:48 2015 THOMAS MILOX
*/

#include "client.h"

void						*init_client(t_client *this)
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
		{NULL, NULL}
	};
	if (!(ret = malloc(sizeof(t_socket))))
		return (NULL);
	this->cmd = cmd;
	return (ret);
}

int 						main()
{
	t_client 			this;


	if (!(this.client = init_client(&this)))
		return (ERROR(ERR_INIT_CLIENT));
	return (0);
}
