/*
** strtowordtab.c for MyIRC in
**
** Made by THOMAS MILOX
** Login   <loxmi@epitech.net>
**
** Started on  Fri Apr 10 23:37:55 2015 THOMAS MILOX
** Last update Fri Apr 10 23:42:21 2015 THOMAS MILOX
*/

#include "client.h"

int	count_words(char *str, char sep)
{
  int	i;
  int	w;

  w = 1;
  i = 0;
  while (str[i])
    {
      if (str[i] == sep)
				w++;
      i++;
    }
  return (w);
}

int	word_char(char *str, char sep, int i)
{
  int	j;

  j = 0;
  while ((str[i] != sep) && str[i])
    {
      i++;
      j++;
    }
  return (j);
}

void	*my_xmalloc(size_t size)
{
  char	*dest;

  dest = malloc(size);
  if (!dest)
    fprintf(stderr, "Error: malloc().\n");
  return (dest);
}

char	**str_to_tab(char *str, char sep)
{
  int	w;
  int	i;
  int	c;
  int	count;
  char	**t;

  c = 0;
  i = 0;
  count = 0;
  w = count_words(str, sep);
  if (!(t = my_xmalloc((w + 1) * sizeof(char *))))
    return (NULL);
  while (count < w)
    {
      c = word_char(str, sep, i);
      if (!(t[count] = my_xmalloc((c + 1) * sizeof(char))))
				return (NULL);
      t[count][c] = '\0';
      strncpy(t[count++], &str[i], c);
      i = i + c;
      while (str[i] == sep)
				i++;
    }
  t[count] = NULL;
  return (t);
}
