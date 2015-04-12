/*
** commands.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Sun Apr 12 04:58:15 2015 Emmanuel Chambon
** Last update Sun Apr 12 20:08:23 2015 Emmanuel Chambon
*/

#include "server.h"

void	sendfile(void _UNUSED_*params, t_user *user, t_server _UNUSED_*serv)
{
  ssend(user->socket, "421 Unknown command\r\n");
}

void	acceptfile(void _UNUSED_*params, t_user *user, t_server _UNUSED_*serv)
{
  ssend(user->socket, "421 Unknown command\r\n");
}
