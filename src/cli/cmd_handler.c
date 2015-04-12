/*
** cmd_handler.c for MyIRC in
**
** Made by THOMAS MILOX
** Login   <loxmi@epitech.net>
**
** Started on  Sat Apr 11 01:41:50 2015 THOMAS MILOX
** Last update Sat Apr 11 01:42:04 2015 THOMAS MILOX
*/

#include "client.h"

void            pars_hdl(t_request *ret, char **cmd, char *in, int flag)
{
  int           i;

  i = 0;
  if (flag)
  {
    (void)cmd;
    ret->cmd = strdup(in);
    ret->arg = NULL;
    return ;
  }
  (void)in;
  ret->cmd = strdup(cmd[i++]);
  while (cmd[i++]);
  if (!(ret->arg = malloc(sizeof(char *) * i)))
    error("malloc");
  for (i = 1; cmd[i] != NULL; i++)
  {
    ret->arg[i - 1] = strdup(cmd[i]);
  }
  ret->arg[i - 1] = NULL;
}

t_request       *parse_cmd(char *in)
{
  char          **tmp;
  t_request     *ret;

  tmp = NULL;
  if (!(ret = malloc(sizeof(t_request))))
    error("malloc");
  if (!in || !(*in) || (*in) == '\n')
  {
    ret->cmd = strdup("\r\n\0");
    ret->arg = NULL;
    return (ret);
  }
  in[strlen(in) - 1] = '\0';
  tmp = str_to_tab(in, ' ');
  *tmp[0] == '/' ? pars_hdl(ret, tmp, in , 0) : pars_hdl(ret, tmp, in , 1);
  free_arrays("ts", tmp, in);
  return (ret);
}

int 						known_cmd(t_client *this, t_request *r, int index)
{
	char 					*formated_cmd;

	formated_cmd = this->cmd[index].op(this, r);
	if (!formated_cmd)
  	return(printf("%d -%s\n", index, r->cmd));
	if (!strncmp(formated_cmd, "00PS", 4))
		{
			printf("%s\n", formated_cmd);
			free_arrays("sstr", formated_cmd, r->cmd, r->arg, r);
			return (0);
		}
  printf("SEND: [%s]\n", formated_cmd);
  ssend(this->client->fd, formated_cmd);
  printf("SENT: [%s]\n", formated_cmd);
	free_arrays("sstr", formated_cmd, r->cmd, r->arg, r);
  return (index == 9 ? 9 : 1);
}

int 						unknown_cmd(t_client *this, t_request *r)
{
  if (this->connected)
    {
      printf("CONNECTED TO SERVER\n");
      free_arrays("str", r->cmd, r->arg, r);;
      return (1);
    }
  fprintf(stderr, "%s: Connect to IRC server first.\n", r->cmd);
  free_arrays("str", r->cmd, r->arg, r);
  return (1);
}


int 						handle_cmd(t_client *this)
{
  char          c;
  int 					index;
  t_request     *r;

  c = (char)getchar();
  if (c == 127)
  {
    write(1, "\b \b", 3);
    rb_delete_last(this->rb);
    return (1);
  }
  c != 27 ? putchar(c) : getchar();
  c != 27 ? rb_write_c(this->rb, c) : getchar();
  if (rb_available(this->rb) && c != '\n')
    return (1);
  r = parse_cmd(rb_read(this->rb));
  for (index = 0; index < 10; ++index)
    {
      if (!strcmp(r->cmd, this->cmd[index].cmd))
      	return (known_cmd(this, r, index));
    }
  return (unknown_cmd(this, r));
}
