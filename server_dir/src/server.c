/*
** EPITECH PROJECT, 2018
** IRC
** File description:
** server
*/

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/queue.h>
#include <string.h>
#include <stdio.h>

#include "irc.h"

static void add_client(env_t *env)
{
	static int user_nb = 0;
	struct sockaddr_in client_sin;
	socklen_t	client_sin_len;
	client_t	*client = malloc(sizeof(client_t));
	if (client == NULL)
		return ;

	printf("adding client"CRLF);
	client_sin_len = sizeof(client_sin);
	client->socket = accept(env->socket, (struct sockaddr *)&client_sin, &client_sin_len);
	memset(client->message, '\0', NICKLEN + 1);
	memset(client->channel, '\0', CHANLEN + 1);
	sprintf(client->nickname, "user%d", user_nb++);
	LIST_INSERT_HEAD(&(env->head), client, entries);
	FD_SET(client->socket, &(env->fd_active));
	env->fd_max += 1;
	dprintf(client->socket, RPL_WELCOME, client->nickname, client->nickname, env->port);
}

static void add_server(env_t *env)
{
	struct sockaddr_in sin;

	env->socket = socket(PF_INET, SOCK_STREAM, 0);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(env->port);
	sin.sin_addr.s_addr = INADDR_ANY;
	bind(env->socket, (struct sockaddr*)&sin, sizeof(sin));
	listen(env->socket, 42);
	FD_SET(env->socket, &(env->fd_active));
	env->fd_max += 1;
}

static void init_env(env_t *env, port_t port)
{
	env->port = port;
	LIST_INIT(&(env->head));
	env->tv.tv_sec = 5;
	env->tv.tv_usec = 0;
	FD_ZERO(&(env->fd_active));
	env->fd_max = 0;
	add_server(env);
}

static void loop(env_t *env)
{
	client_t	*np;
	fd_set		fd_read;

	while (1) {
		fd_read = env->fd_active;
		if (select(FD_SETSIZE, &fd_read, NULL, NULL, &(env->tv)) == -1)
			perror("select");
		if (FD_ISSET(env->socket, &fd_read))
			add_client(env);
		LIST_FOREACH(np, &(env->head), entries)
			if (FD_ISSET(np->socket, &fd_read))
				client_read(env, np);
	}
}

int server(port_t port)
{
	env_t env;
	init_env(&env, port);

	loop(&env);

	return 0;
}
