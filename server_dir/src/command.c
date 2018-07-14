/*
** EPITECH PROJECT, 2018
** IRC
** File description:
** command
*/

#include <stdio.h>
#include <string.h>

#include "irc.h"

static void change_nickname(env_t *env, client_t *client)
{
	char *nick = strtok(NULL, " ");
	if (nick == NULL)
		dprintf(client->socket, ERR_NONICKNAMEGIVEN);

	memset(client->nickname, 0, NICKLEN);
	memcpy(client->nickname, nick, strlen(nick));
	dprintf(client->socket, RPL_NICKNAMECHANGED, client->nickname);
}

static void list_channels(env_t *env, client_t *client)
{
	client_t *np;

	dprintf(client->socket, RPL_LISTSTART);
	LIST_FOREACH(np, &(env->head), entries)
		if (np->channel[0] != '\0')
			dprintf(client->socket, RPL_LIST, np->channel);
	dprintf(client->socket, RPL_LISTEND);
}

static void join_channel(env_t *env, client_t *client)
{
	char *channel = strtok(NULL, " ");
	if (channel == NULL)
		dprintf(client->socket, ERR_BADCHANNAME);

	memset(client->channel, 0, CHANLEN);
	memcpy(client->channel, channel, strlen(channel));
	dprintf(client->socket, RPL_CHANUSER, client->channel);
}

static void leave_channel(env_t *env, client_t *client)
{
	dprintf(client->socket, RPL_CHANUSER, client->channel);
	memset(client->channel, '\0', CHANLEN);
}

static void list_users(env_t *env, client_t *client)
{
	client_t *np;

	dprintf(client->socket, RPL_LISTSTART);
	LIST_FOREACH(np, &(env->head), entries)
		dprintf(client->socket, RPL_LIST, np->channel, np->nickname);
	dprintf(client->socket, RPL_LISTEND);
}

static void list_channel_users(env_t *env, client_t *client)
{
	char *channel = strtok(client->message, " ");
	if (channel == NULL)
		dprintf(client->socket, ERR_NOSUCHCHANNEL, "None");

	client_t *np;
	dprintf(client->socket, RPL_LISTSTART);
	LIST_FOREACH(np, &(env->head), entries)
		if (strcmp(np->channel, channel) == 0)
			dprintf(client->socket, RPL_LIST, np->channel, np->nickname);
	dprintf(client->socket, RPL_LISTEND);
}

static void message_user(env_t *env, client_t *client)
{
	char *user = strtok(client->message, " ");
	if (user == NULL)
		dprintf(client->socket, ERR_NOSUCHNICK, "None");

	char *message = strtok(client->message, " ");
	if (user == NULL)
		dprintf(client->socket, ERR_NOTEXTTOSEND);

	client_t *np;
	LIST_FOREACH(np, &(env->head), entries)
		if (strcmp(np->nickname, user) == 0)
			dprintf(client->socket, RPL_PRIVATEMSG, client->nickname, message);
}

static const cmd_t cmds[] =
{
	{"/nick", &change_nickname},
	{"/NICK", &change_nickname},
	{"/list", &list_channels},
	{"/join", &join_channel},
	{"/part", &leave_channel},
	{"/users", &list_users},
	{"/names", &list_channel_users},
	{"/msg", &message_user},
	{NULL, NULL},
};

void manage_commands(env_t *env, client_t *client)
{
	char	*cmd = strtok(client->message, " ");

	for (size_t i = 0; cmds[i].cmd; i++) {
		if (strcmp(cmds[i].cmd, cmd) == 0)
			cmds[i].fct(env, client);
	}
}
