/*
 * brief: CLI library example - structures header
 * author: vitor.rozsa
 * date: 19/02/2013
 */
#ifndef CLI_STRUCTS_H_INCLUDED
#define CLI_STRUCTS_H_INCLUDED

#include <stdbool.h>
#include <pthread.h>

#include "cliDefines.h"

typedef struct {
	char cmd_str[CLI_SIZEOF_CMD_STRING];
	char brief[CLI_SIZEOF_BRIEF_STRING];
	void *function;
} st_cli_cmd;

typedef struct {
	char hello_message[CLI_SIZEOF_HELLO_MESSAGE];
	int server_fd;
	bool initialized;
	bool launched;
	pthread_t thread_id;
	st_cli_cmd *cmd_list;
} st_cli_instance;

#endif /* CLI_STRUCTS_H_INCLUDED */
