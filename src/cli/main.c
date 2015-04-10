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

int 						known_cmd(t_client *this, char *cmd, int index)
{
	(void)this;
	(void)cmd;
	printf("[%d]\n", index);
	return (index);
}

int 						unknown_cmd(t_client *this, char *cmd)
{
	if (this->connected)
	{
		printf("CONNECTED TO SERVER\n");
	}
	return (USR_OFFLINE(cmd));
}

int 						handle_cmd(t_client *this, char *cmd)
{
	int 					index;

	cmd[strlen(cmd) - 1] = '\0';
	for (index = 0; index < 10; ++index)
	{
				if (!strcmp(cmd, this->cmd[index].cmd))
			return (known_cmd(this, cmd, index));
	}
	return (unknown_cmd(this, cmd));
}

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
		{"/quit", &quit_client},
		{NULL, NULL}
	};
	if (!(ret = malloc(sizeof(t_socket))))
		return (NULL);
	this->cmd = cmd;
	this->connected = 0;
	this->nickname = getlogin();
	return (ret);
}


int 						end_client(t_client *this, char *input)
{
	free(input);
	if (this->connected)
		close(this->client->fd);
	printf("Goodbye %s ;)\n", this->nickname);
	return (0);
}
int 						main()
{
	int 					i;
	size_t 				n;
	char 					*input;
	t_client 			this;

	n = 0;
	input = NULL;
	if (!(this.client = init_client(&this)))
		return (ERROR(ERR_INIT_CLIENT));
	while (write(1, "irc> ", 5) && getline(&input, &n, stdin))
    {
    	i = handle_cmd(&this, input);
      if (i == 9)
      	break ;
      else if (i < 0)
      	return (ERROR(ERR_RUNNING));
    }
	return (end_client(&this, input));
}
