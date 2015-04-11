/*
** common.h for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 04:27:07 2015 Emmanuel Chambon
** Last update Fri Apr 10 19:12:48 2015 Emmanuel Chambon
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
# include "network.h"

# define RB_SIZE		64

typedef struct s_ring_buffer	t_ring_buffer;

struct				s_ring_buffer
{
  char				rb[RB_SIZE];
  char				*rrb;
  char				*wrb;
  int				available;
};

typedef struct 					s_request
{
	char 						*cmd;
	char						**arg;
} 								t_request;

/*
**	utils.c
*/
void				error(const char *);
char 				**str_to_tab(char *, char);
void 				free_it(char **);
void 				free_arrays(char *, ...);
/*
**	sockutils.c
*/
void				*ipvx(struct sockaddr *);
/*
**	rbutils.c
*/
t_ring_buffer			*rb_init();
void				rb_free(t_ring_buffer *);
int				rb_available(t_ring_buffer *);
void				rb_write(t_ring_buffer *, char *);
char				*rb_read(t_ring_buffer *);

#endif /* !_COMMON_H_ */
