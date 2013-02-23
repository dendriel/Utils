/*
 * brief: CLI library example - source code
 * author: vitor.rozsa
 * date: 19/02/2013
 *
 * note: This CLI library is supposed to have multiple contexts
 *  of CLI interfaces.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "cli.h"
#include "cliStructs.h"
#include "cliDefines.h"

/* Private Functions Prototypes*/
static void fill_instances_to_default(st_cli_instance *cli_instances, const int instances_number);
static void free_instance(st_cli_instance *cli_instance);
static void dump_instances_cfg(st_cli_instance *cli_instances, const int instances_number);
static void dump_instance(st_cli_instance *cli_instances);
static int find_free_cli_instance(void);
static void *server_thread(void *data);
static int create_comm_stream(const unsigned int port, const unsigned int max_user);
static int server_read_cmd(const int client_fd, char *cmd);
static int server_split_cmd(const int client_fd, char *cmd, char *cmd_args[]);
static int server_execute_cmd(const int client_fd, char *cmd);

/* Global Definitions */
static st_cli_instance *cli_instances_data = NULL;
static bool cli_initialized = false;
static int cli_instances_number;

/*******************************************************/
/* Global Functions */

int cli_lib_init(const unsigned int instances_number)
{
	if (cli_initialized == true) {
		return -1;
	}

	cli_instances_data = (st_cli_instance *) malloc(instances_number * sizeof(st_cli_instance));
	if (!cli_instances_data) {
		fprintf(stderr, "Failed to allocate memory for cli_instances_data!\n");
		return -1;
	}

	memset(cli_instances_data, 0, (sizeof(st_cli_instance) * instances_number));

	cli_instances_number = instances_number;
	cli_initialized = true;

	fill_instances_to_default(cli_instances_data, cli_instances_number);

	// testing purpose only!!!
	if(0) {
		// testing purpose only!!!
		dump_instances_cfg(cli_instances_data, cli_instances_number);
	}

	return 0;
}

void cli_lib_halt(void)
{
	if (cli_initialized == false) {
		return;
	}

	cli_initialized = false;
	cli_instances_number = 0;

	free(cli_instances_data);
}

int cli_start_instance(const unsigned int port, const unsigned int max_user, const char *hello_message, st_cli_cmd *cmd_list)
{
	int server_fd;
	int cli_instance_id;

	if (cli_initialized == false) {
		return -1;
	}

	if (port > 65535) {
		fprintf(stderr, "Port out of range! Port: [%d]\n", port);
		return -1;
	}

	if (hello_message == NULL) {
		fprintf(stderr, "Hello Message can't have a null value!\n");
		return -1;
	}

	cli_instance_id = find_free_cli_instance();
	if (cli_instance_id < 0) {
		fprintf(stderr, "There is no free CLI instance to be used!\n");
		return -1;
	}

	server_fd = create_comm_stream(port, max_user);
	if (server_fd < 0) {
		fprintf(stderr, "Failed to create and configure the socket stream!\n");
		return -1;
	}

	strncpy(cli_instances_data[cli_instance_id].hello_message, hello_message, CLI_SIZEOF_HELLO_MESSAGE);
	cli_instances_data[cli_instance_id].server_fd = server_fd;
	cli_instances_data[cli_instance_id].initialized = true;
	cli_instances_data[cli_instance_id].launched = false;
	cli_instances_data[cli_instance_id].cmd_list = cmd_list;

	return cli_instance_id;
}

void cli_free_instance(const int cli_instance_id)
{
	if ( (cli_instance_id > cli_instances_number) ||
			(cli_instances_data[cli_instance_id].initialized == false) ||
			(cli_instances_data[cli_instance_id].launched == true)) {
		return;
	}

	free_instance(&cli_instances_data[cli_instance_id]);
}

int cli_launch(const int cli_id)
{
	int ret;

	if ( (cli_initialized == false) || (cli_instances_data[cli_id].initialized == false) ||
			(cli_instances_data[cli_id].thread_id > -1) ) {
		return -1;
	}

	ret = pthread_create(&cli_instances_data[cli_id].thread_id, NULL, server_thread, (void*) &cli_instances_data[cli_id]);
	if (ret != 0) {
		fprintf(stderr, "Failed to launch the new thread.\n");
		return -1;
	}

	return 0;
}

/*******************************************************/
/* Private Functions */

/*
 * brief Receive and fork connections.
 */
//TODO: http://linux.die.net/man/2/accept - poll connections and fork it
static void *server_thread(void *data)
{
	st_cli_instance *cli_instance = NULL;
	char cmd[CLI_SIZEOF_READ_CLIENT_CMD];
	char **cmd_args = NULL;
	int client_fd;
	int ret;

	int i = 0; //testing!!!

	cli_instance = (st_cli_instance *) data;

	// testing purpose only!
	printf("NEW THREAD LAUNCHED!!\n\n");
	// testing purpose only!
	dump_instance(cli_instance);
	// testing purpose only!
	fprintf(stderr, "will accept in %d\n", cli_instance->server_fd);

	client_fd = accept(cli_instance->server_fd, (struct sockaddr*)NULL, NULL);
	if (client_fd < 0) {
		fprintf(stderr, "Server failed to accept connection. Error [%d]\n", errno);
		return 0;
	}

	do {
		ret = server_read_cmd(client_fd, cmd);
		if (ret != CLI_OK) {
			continue;
		}

		ret = server_split_cmd(cmd, cmd_args);
		if (ret != CLI_ERROR) {
			for (i = 0; i < ret; i++) {
				printf("> %s\n", cmd_args[i]);
			}
		}

		ret = server_execute_cmd(client_fd, cmd);

	} while (ret != CLI_EXIT);

	close(client_fd);

	return 0;
}

/*
 * brief Read user commands.
 */
static int server_read_cmd(const int client_fd, char *cmd)
{
	int ret;
	char buf[CLI_SIZEOF_READ_CLIENT_CMD];

	memset(buf, 0, sizeof(buf));

	ret = recv(client_fd, buf, CLI_SIZEOF_READ_CLIENT_CMD, 0);
	if (ret < 0) {
		fprintf(stderr, "Failed to read client data!\n");
		return CLI_ERROR;
	}

	strncpy(cmd, buf, CLI_SIZEOF_READ_CLIENT_CMD);

	return CLI_OK;
}

/*
 * brief Returns a list that the first elements
 *  will be the function to be run and the following elements will be the
 *  arguments. ??????? maybe..
 */
static int server_split_cmd(char *cmd, char **cmd_args)
{
	int ind = 0;
	char *saveptr = NULL;
	const char *delim = " ";
	char *elem = NULL;

	elem = strtok_r(cmd, delim, &saveptr);
	if (elem == NULL) {
		return CLI_OK;
	}

	printf("point [0] elem: %s\n", elem);

	*cmd_args = (char *) malloc(sizeof(char));
	printf("point [0] elem: %s\n", elem);
	cmd_args[ind] = elem;
	ind++;

	printf("point [1]\n");

	elem = strtok_r(cmd, delim, &saveptr);
	while (elem != NULL) {
		cmd_args[ind] = (char *) realloc(cmd_args, sizeof(char));
		cmd_args[ind] = elem;
		ind++;
		printf("point [2]\n");
	}
	printf("point [3]\n");
	return (ind + 1);
}

/*
 * brief Execute user commands.
 */
static int server_execute_cmd(const int client_fd, char *cmd)
{
	return CLI_EXIT;
}

/*
 * brief Will fill all CLI instances with default values.
 */
static void fill_instances_to_default(st_cli_instance *cli_instances_data, const int instances_number)
{
	int i;

	for (i = 0; i < instances_number; i++) {
		free_instance(&cli_instances_data[i]);
	}
}

/*
 * brief Fill a CLI instances with default values.
 */
static void free_instance(st_cli_instance *cli_instance)
{
	if (cli_instance->launched == true) {
		return;
	}

	cli_instance->initialized = false;
	cli_instance->launched = false;
	memset(cli_instance->hello_message, '\0', CLI_SIZEOF_HELLO_MESSAGE);
	cli_instance->server_fd = -1;
	cli_instance->thread_id = -1;
}

/*
 * brief Will dump all CLI instances configurations
 */
static void dump_instances_cfg(st_cli_instance *cli_instances_data, const int instances_number)
{
	int i;

	for (i = 0; i < instances_number; i++) {
		printf("\ninstance id - [%d]\n", i);
		dump_instance(&cli_instances_data[i]);
	}
}

/*
 * brief Dump CLI structure data - TODO: testing purpose only, remove this function
 */
static void dump_instance(st_cli_instance *cli_instances_data)
{
	printf("cli_instances_data.initialized   - %d\n", cli_instances_data->initialized);
	printf("cli_instances_data.launched   - %d\n", cli_instances_data->launched);
	printf("cli_instances_data.hello_message - \"%s\"\n", cli_instances_data->hello_message);
	printf("cli_instances_data.server_fd     - %d\n", cli_instances_data->server_fd);
	printf("cli_instances_data.thread_id     - %d\n", (int)cli_instances_data->thread_id);
}

/*
 * brief Look for a free cli instance id to be used
 */
static int find_free_cli_instance(void)
{
	int i;

	for (i = 0; i < cli_instances_number; i++) {
		if (cli_instances_data[i].initialized == false) {
			return i;
		}
	}

	return -1;
}

/*
 * brief Will create and configure a socket to communicate.
 */
static int create_comm_stream(const unsigned int port, const unsigned int max_user)
{
	int server_fd;
	int optval;
	struct sockaddr_in serv_addr;

	memset(&serv_addr, 0, sizeof(serv_addr));

	// create socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0 ) {
		fprintf(stderr, "Failed to created the server socket! Error: [%d]\n", errno);
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	optval = 1;
	// will set socket options to reuse address
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
		fprintf(stderr, "Failed to set socket options!\n");
		return -1;
	}

	// bind socket to an address and port
	if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "Failed to bind socket! Error: [%d]\n", errno);
		close(server_fd);
		return -1;
	}

	// enable the socket to listen for connections
	if (listen(server_fd, max_user) < 0) {
		fprintf(stderr, "Server has failed to listen for connections. Error: [%d]\n", errno);
		close(server_fd);
		return -1;
	}

	return server_fd;
}
