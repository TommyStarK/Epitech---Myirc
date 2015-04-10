/*
** rbutils.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 14:11:57 2015 Emmanuel Chambon
** Last update Fri Apr 10 05:25:22 2015 Emmanuel Chambon
*/

#include "common.h"

t_ring_buffer		*rb_init()
{
  t_ring_buffer		*buffer;

  if (!(buffer = malloc(sizeof(t_ring_buffer))))
    error("buffer");
  memset(buffer->rb, 0, RB_SIZE);
  buffer->rrb = buffer->rb;
  buffer->wrb = buffer->rb;
  buffer->available = RB_SIZE;
  return (buffer);
}

void			rb_free(t_ring_buffer *ring)
{
  if (ring)
    free(ring);
}

int			rb_available(t_ring_buffer *ring)
{
  return ((ring->wrb >= ring->rrb)
	  ? (RB_SIZE - (int)(ring->wrb - ring->rrb))
	  : (int)(ring->rrb - ring->wrb));
}
