/*
** EPITECH PROJECT, 2018
** IRC
** File description:
** client
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "irc.h"

static void close_client(env_t *env, client_t *client)
{
	printf("closing client %s"CRLF, client->nickname);
	dprintf(client->socket, RPL_QUIT, client->nickname);
	FD_CLR(client->socket, &(env->fd_active));
	close(client->socket);
	LIST_REMOVE(client, entries);
	free(client);
}

static void send_channel_message(env_t *env, client_t *client)
{
	fd_set fd_write = env->fd_active;
	client_t *np;

	if (select(FD_SETSIZE, &fd_write, NULL, NULL, &(env->tv)) == -1)
		perror("select");
	LIST_FOREACH(np, &(env->head), entries)
//		if (FD_ISSET(np->socket, &fd_write))
			if (strcmp(np->channel, client->channel) == 0)
				dprintf(np->socket, RPL_MESSAGE, client->nickname, client->message);
}

void client_read(env_t *env, client_t *client)
{
	ssize_t r;

	printf("reading client"CRLF);
	r = read(client->socket, client->message, MSGLEN);
	if (r == 0 || client->message[0] == EOT)
		close_client(env, client);
	else {
		client->message[r - CRLF_LEN] = '\0';
		if (client->message[0] == COMMAND)
			manage_commands(env, client);
		else
			send_channel_message(env, client);
	}
}
