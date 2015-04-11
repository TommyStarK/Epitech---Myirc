/*
** network.h for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 04:36:36 2015 Emmanuel Chambon
** Last update Sat Apr 11 18:49:12 2015 Emmanuel Chambon
*/

#ifndef _NETWORK_H_
# define _NETWORK_H_

# include <sys/types.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>

# define BACKLOG	5

/*
**	socket.c
*/
void		ssend(int, char *);
void		snd(char *, int *, int);

#endif /* !_NETWORK_H_ */
