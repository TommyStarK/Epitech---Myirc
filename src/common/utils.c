/*
** utils.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 15:30:53 2015 Emmanuel Chambon
** Last update Thu Apr  9 15:31:14 2015 Emmanuel Chambon
*/

#include "common.h"

void		error(char *error)
{
  perror(error);
  exit(EXIT_FAILURE);
}
