/*
** EPITECH PROJECT, 2018
** IRC
** File description:
** Chat irc
*/

#ifndef IRC_H_
# define IRC_H_

# include <inttypes.h>
# include <sys/queue.h>
# include <sys/time.h>

# ifndef EOT
#  define EOT 4
# endif

# define MSGLEN 512
# define NICKLEN 255
# define CHANLEN 255
# define CRLF "\r\n"
# define CRLF_LEN 2

# define MAXCHAN 1024

# define COMMAND '/'

# define RPL_WELCOME "220 :Welcome to the Internet Relay Network %s!%s@%d"CRLF
# define RPL_QUIT "220 :See you soon %s"CRLF
# define RPL_MESSAGE "220 :[%s] %s"CRLF
# define RPL_LISTSTART "321 Channels :User Name"CRLF
# define RPL_LIST "322 %s :%s"CRLF
# define RPL_LISTEND "323 :End of list"CRLF
# define ERR_NONICKNAMEGIVEN "431 :No nickname given"CRLF
# define RPL_NICKNAMECHANGED "220 :Nickname changed to %s"CRLF
# define ERR_BADCHANNAME "479 :Bad channel name"CRLF
# define RPL_CHANUSER "287 :Successfully joined %s"CRLF
# define RPL_LEFTCHAN "220 :Successfully left %s"CRLF
# define ERR_NOSUCHCHANNEL "403 :%s :Wrong channel name"CRLF
# define ERR_NOSUCHNICK "401 :%s :Unknown nickname"CRLF
# define ERR_NOTEXTTOSEND "412 :No text to send"CRLF
# define RPL_PRIVATEMSG "220 :User %s sent you: %s"CRLF

typedef uint16_t port_t;

typedef struct s_client
{
	int	socket;
	char	message[MSGLEN + 1];
	char	nickname[NICKLEN + 1];
	char	channel[CHANLEN + 1];
	LIST_ENTRY(s_client) entries;
}	client_t;

typedef struct s_env
{
	int		socket;
	port_t		port;
	struct listhead *headp;
	LIST_HEAD(listhead, s_client) head;
	struct timeval	tv;
	fd_set		fd_active;
	int		fd_max;
}	env_t;

typedef struct s_cmd
{
	char	*cmd;
	void	(*fct)(env_t *env, client_t *client);
}	cmd_t;

int server(port_t);
void client_read(env_t*, client_t*);
void manage_commands(env_t*, client_t*);

#endif /* !IRC_H_ */
