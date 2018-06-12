##
## EPITECH PROJECT, 2018
## IRC
## File description:
## Main MAKEFILE
##

SHELL	:= /bin/bash

MAKE	:= make
CP	:= cp
RM	:= rm -f

all:	server client

server: FORCE
	$(MAKE) -C server_dir
	$(CP) server_dir/server .

client: FORCE
	$(MAKE) -C client_dir
	$(CP) client_dir/client .

FORCE:

clean:
	$(MAKE) -C server_dir clean
	$(MAKE) -C client_dir clean

fclean:
	$(MAKE) -C server_dir fclean
	$(MAKE) -C client_dir fclean
	$(RM) {server,client}

re:	fclean all

.PHONY: all clean fclean re
