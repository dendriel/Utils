/*
 * brief: CLI library example - prototype functions header
 * author: vitor.rozsa
 * date: 19/02/2013
 */
#ifndef CLI_H_INCLUDED
#define CLI_H_INCLUDED

#include "cliStructs.h"

/*
 * brief Will start CLI library; Allocate memory for N CLI instances and
 *  set global flag that enable CLI to be used.
 */
int cli_lib_init(const unsigned int instances_number);

/*
 * brief Will disable global flag; Will free instances data.
 *  //TODO free the structures inner data if needed.
 */
void cli_lib_halt(void);

/*
 * brief Initialize a new instance of CLI.
 */
int cli_start_instance(const unsigned int port, const unsigned int max_user, const char *hello_message, st_cli_cmd *cmd_list);

/*
 * brief Will free a CLI instance by the given ID.
 */
void cli_free_instance(const int cli_instance_id);

/*
 * brief Will launch CLI interface and grant access to the registered functions.
 */
int cli_launch(const int cli_id);

#endif /* CLI_H_INCLUDED */
