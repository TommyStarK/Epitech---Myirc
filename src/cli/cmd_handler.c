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

void            unknown_cmd_handler(t_client *c, t_request *r)
{
  int           i;
  int           s;
  char          *f;

  f = strdup(r->cmd);
  if (s = strlen(f), r->arg && r->arg[0])
  {
    for (i = 0; r->arg[i]; i++)
    {
      !(f = realloc(f, s + strlen(r->arg[i]) + 2)) ? error("realloc") : 0;
      r->arg[i] ? strcat(f, " ") : 0;
      f[s += 1] = 0;
      !(f = strcat(f, r->arg[i])) ? error("strcat") : 0;
      f[s += strlen(r->arg[i])] = 0;
      }
    }
  !(f = realloc(f, s + 3)) ? error("realloc") : 0;
  f = strcat(f, "\r\n\0");
  ssend(c->client->fd, f);
  free_arrays("sstr", f, r->cmd, r->arg, r);
}

int 						unknown_cmd(t_client *this, t_request *r)
{
  if (this->connected)
    {
      if (r->cmd[0] == '/')
      {
        unknown_cmd_handler(this, r);
        return (1);
      }
      else
      {
        !(r->cmd = realloc(r->cmd, strlen(r->cmd) + 3)) ? error("reallc") : 0; 
        r->cmd = strcat(r->cmd, "\r\n\0");
        ssend(this->client->fd, r->cmd);
        free_arrays("str", r->cmd, r->arg, r);
      }
      return (1);
    }
  fprintf(stderr, "%s: Connect to IRC server first.\n", r->cmd);
  free_arrays("str", r->cmd, r->arg, r);
  return (1);
}

int             known_cmd(t_client *this, t_request *r, int index)
{
  char          *formated_cmd;

  formated_cmd = this->cmd[index].op(this, r);
  if (!strncmp(formated_cmd, "00PS", 4))
    {
      printf("%s\n", formated_cmd);
      free_arrays("sstr", formated_cmd, r->cmd, r->arg, r);
      return (1);
    }
  ssend(this->client->fd, formated_cmd);
  free_arrays("sstr", formated_cmd, r->cmd, r->arg, r);
  return (index == 9 ? 9 : 1);
}