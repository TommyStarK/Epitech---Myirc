/*
** common.h for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 04:27:07 2015 Emmanuel Chambon
** Last update Sun Apr 12 00:28:30 2015 Emmanuel Chambon
*/

#ifndef _COMMON_H_
# define _COMMON_H_

# include <stdlib.h>
# include <stdarg.h>
# include <errno.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdint.h>
# include <signal.h>
# include <termios.h>
# include "network.h"

# define RB_SIZE		64

typedef struct s_ring_buffer	t_ring_buffer;

struct				s_ring_buffer
{
  char				rb[RB_SIZE];
  char				*rrb;
  char				*wrb;
  int					available;
};

/*
**	utils.c
*/
void				error(const char *);
/*
**	sockutils.c
*/
void				*ipvx(struct sockaddr *);
void   			ssend(int, char *);
void				snd(char *, int *, int);
/*
**	rbutils.c
*/
t_ring_buffer			*rb_init();
void				rb_free(t_ring_buffer *);
int				rb_available(t_ring_buffer *);
void				rb_write(t_ring_buffer *, char *);
void				rb_write_c(t_ring_buffer *, char);
char				*rb_read(t_ring_buffer *);

#endif /* !_COMMON_H_ */
