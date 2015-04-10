/*
** rbutils.c for  in /home/chambo_e/epitech/PSU_2014_myirc
**
** Made by Emmanuel Chambon
** chambo_e  <chambon.emmanuel@gmail.com>
**
** Started on  Thu Apr  9 14:11:57 2015 Emmanuel Chambon
** Last update Fri Apr 10 19:28:30 2015 Emmanuel Chambon
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

void			rb_write(t_ring_buffer *ring, char *str)
{
  int			i;

  ring->wrb = (ring->wrb) ? ring->wrb : ring->rb;
  for (i = 0; str[i]; i++)
    {
      *ring->wrb = str[i];
      ring->wrb = &ring->rb[(((size_t)ring->wrb + 1) - (size_t)ring->rb) % RB_SIZE];
    }
}

char			*rb_read(t_ring_buffer *ring)
{
  char			*str;
  int			size;
  int			i;

  size = ((ring->wrb >= ring->rrb)
	  ? (int)(ring->wrb - ring->rrb)
	  : (RB_SIZE - (int)(ring->rrb - ring->wrb)));
  if (!(str = malloc(size + 1)))
    error("malloc");
  memset(str, 0, size + 1);
  for (i = 0; i < size; i++)
    {
      str[i] = *ring->rrb;
      ring->rrb = &ring->rb[(((size_t)ring->rrb + 1) - (size_t)ring->rb) % RB_SIZE];
    }
  return (str);
}

void			rb_display(t_ring_buffer *ring)
{
  int			i;

  for (i = 0; i < RB_SIZE; i++)
    {
      write(1, &ring->rb[i], 1);
    }
}
