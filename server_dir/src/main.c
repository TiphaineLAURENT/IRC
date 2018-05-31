/*
** EPITECH PROJECT, 2018
** IRC
** File description:
** main
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include "irc.h"

static int help(void)
{
	fprintf(stderr, "USAGE:\t./server port\n");
	return 2;
}

static const struct option long_options[] = {
	{"help", no_argument, 0, 'h'},
	{0, 0, 0, 0}
};

static int parse(int argc, char **argv)
{
	int c;

	while (1) {
		int option_index = 0;
		c = getopt_long(argc, argv, "h", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 'h':
			return help();
		default:
			return help();
		}
	}

	return 0;
}

int main(int argc, char **argv)
{
	if (parse(argc, argv) == 2)
		return 84;
	if (argc != 2)
		return help() * 42;
	return server((port_t)atoi(argv[1]));
}
