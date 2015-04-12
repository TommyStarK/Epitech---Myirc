/*
** main.c for MyIRC in
**
** Made by THOMAS MILOX
** Login   <loxmi@epitech.net>
**
** Started on  Fri Apr 10 18:05:41 2015 THOMAS MILOX
** Last update Fri Apr 10 21:20:14 2015 Emmanuel Chambon
*/

#include "client.h"

void            non_canon_mode(char c)
{
  struct termios t;

  tcgetattr(STDIN_FILENO, &t);
  if (!c)
  {
    t.c_lflag &= ~ICANON;
    t.c_lflag &= ~ECHO;
    t.c_cc[VMIN] = 1;

  }
  else if (c == 1)
    {
      t.c_lflag |= ICANON;
      t.c_lflag |= ECHO;
    }
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void            run(t_client *this)
{
  int           i;
  int           ret;
  int           fdmax;
  fd_set        read_fds;

  fdmax = this->client->fd + 1;
  FD_ZERO(&read_fds);
  FD_SET(this->client->fd, &read_fds);
  this->fdmax = &fdmax;
  this->rfds = &read_fds;
  for (;;)
  {
    if (select(fdmax, &read_fds, NULL, NULL, NULL) == -1)
      error("select");
    for (i = 0; i <= fdmax; ++i)
    {
      if (FD_ISSET(i, &read_fds))
      {
        ret = (!i ? handle_cmd(this) : 0);
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
  this->channel = strdup("home");
  this->nickname = getlogin();
  return (ret);
}

int 						main()
{
  t_client 			this;

  this.client = init_client(&this);
  this.client->fd = 0;
  this.rb = rb_init();
  non_canon_mode(0);
  run(&this);
  non_canon_mode(1);
  if (this.connected)
    close(this.client->fd);
  printf("Goodbye %s ;)\n", this.nickname);
  return (0);
}
