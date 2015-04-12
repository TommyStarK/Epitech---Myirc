/*
** main.c for MyIRC in
**
** Made by THOMAS MILOX
** Login   <loxmi@epitech.net>
**
** Started on  Fri Apr 10 18:05:41 2015 THOMAS MILOX
** Last update Sun Apr 12 13:11:09 2015 Emmanuel Chambon
*/

#include "client.h"

int             read_answer(t_client *this)
{
  int     i;
  char    tmp[RB_SIZE];

 memset(tmp, 0, RB_SIZE);
 if ((i = recv(this->client->fd, tmp, rb_available(this->rs), 0)) > 0)
  {
    rb_write(this->rs, tmp);
    if (rb_at(this->rs, -1) == '\n' && rb_at(this->rs, -2) == '\r')
      {
        printf("%s", rb_read(this->rs));
        return (1);
      }
    }
  else
    !i ? printf("00PS: Connection closed by server.\n") : error("recv");
  return (1);
}

int             handle_cmd(t_client *this)
{
  char          c;
  int           index;
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
 fflush(stdout);
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

void            run(t_client *this)
{
  int           i;
  int           ret;
  int           fdmax;
  fd_set        read_fds;

  fdmax = 1;
  FD_ZERO(&read_fds);
  FD_SET(0, &read_fds);
  this->fdmax = &fdmax;
  this->rfds = &read_fds;
  for (;;)
  {
    if (select(fdmax, &read_fds, NULL, NULL, NULL) == -1)
      error("select");
    for (i = 0; i <= *this->fdmax; i++)
    {
      if (FD_ISSET(i, &read_fds))
      {
        ret = (!i ? handle_cmd(this) : read_answer(this));
        if (ret == 9 || ret < 0)
          return ;
      }
    }
  }
}

t_socket				*init_client(t_client *this)
{
  t_socket			*ret;
  static t_fct	cmd[] = {
    {"/server", &connect_server},
    {"/nick", &change_nickname},
    {"/list", &list_channels},
    {"/join", &join_channel},
    {"/part", &leave_channel},
    {"/users", &list_users},
    {"/msg", &send_msg_to_user},
    {"/send_file", &send_file_to_user},
    {"/accept_file", &accept_file_from_user},
    {"/quit", &quit_client},
    {NULL, NULL}
  };
  if (!(ret = malloc(sizeof(t_socket))))
    error("malloc");
  this->cmd = cmd;
  this->fdmax = NULL;
  this->rfds = NULL;
  this->connected = 0;
  this->channel = strdup("Lobby");
  this->nickname = getlogin();
  return (ret);
}

int 						main()
{
  t_client 			this;

  this.client = init_client(&this);
  this.client->fd = -1;
  this.rb = rb_init();
  this.rs = rb_init();
  non_canon_mode(0);
  run(&this);
  non_canon_mode(1);
  if (this.connected)
    close(this.client->fd);
  printf("Goodbye %s ;)\n", this.nickname);
  if (this.connected)
    free_arrays("ss", this.nickname, this.channel);
  return (0);
}
