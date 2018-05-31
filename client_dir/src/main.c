/*
** EPITECH PROJECT, 2018
** IRC
** File description:
** client main
*/

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <resolv.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "client.h"

static int connect_server(void)
{
	char *hostname = strtok(NULL, ":");
	if (hostname == NULL) {
		printf("Invalid hostname\n");
		return -1;
	}
	char *port = strtok(NULL, ":");
	printf("Hostname: %s\tPort: %s\n", hostname, port);
	struct hostent *host = gethostbyname(hostname);
	int sd = socket(PF_INET, SOCK_STREAM, 0);
	if (sd == -1)
		perror("socket");
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	if (port == NULL)
		addr.sin_port = htons(80);
	else
		addr.sin_port = htons((short unsigned int)atoi(port));
	addr.sin_addr.s_addr = *(unsigned int*)host->h_addr_list[0];
	if (connect(sd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("connect");
		return -1;
	}
	printf("Connected\n");
	return sd;
}

static int client(void)
{
	ssize_t r;
	char input[INPUTLEN];
	char response[RESPLEN];
	int sd = -1;

	while (1) {
		r = read(0, input, INPUTLEN);
		if (r < 2)
			continue;
		input[r - 1] = 0;
		if (strcmp("/server", strtok(input, " ")) == 0) {
			printf("Connecting to server...\n");
			sd = connect_server();
			r = read(sd, response, RESPLEN);
			if (r == -1)
				continue;
			response[r - 2] = 0;
			printf("%s\n", response);
		} else if (sd != -1) {
			dprintf(sd, "%s"CRLF, input);
			r = read(sd, response, RESPLEN);
			if (r == -1)
				continue;
			response[r - 2] = 0;
			printf("%s\n", response);
		}
	}
	return 0;
}

int		main(void)
{
	return client();
}
