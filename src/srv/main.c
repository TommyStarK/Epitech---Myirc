/*
** main.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 04:16:38 2015 Emmanuel Chambon
** Last update Thu Apr  9 15:31:36 2015 Emmanuel Chambon
*/

#include "server.h"

int		usage()
{
  fprintf(stderr, "[USAGE]: ./server [PORT]\n");
  return (1);
}

int		main(int ac, char **av)
{
  t_server	*serv;

  if (ac != 2)
    return (usage());
  serv = init_server(av[1]);
  handler_server(serv);
  release_server(serv);
  return (0);
}
