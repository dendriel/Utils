/*
 * brief: main program used to develop and test the cli library
 * author: vitor.rozsa
 * date: 15/02/2013
 */
#include <stdio.h>
#include <unistd.h>

#include "cli.h"
#include "cliStructs.h"

int cli1_sum(const int a, const int b)
{
	return a + b;
}

int cli1_sub(const int a, const int b)
{
	return a - b;
}

void cli2_print(const char *str, const char a)
{
	printf("str: %s - char %c\n", str, a);
}

void cli2_hello(const char *str)
{
	printf("Hello message is: %s\n", str);
}

st_cli_cmd cli1_cmd_list[] = {
		{"sum", "> Usage: sum numA + numB", cli1_sum},
		{"sub", "> Usage: sub numA - numB", cli1_sub}
};

int main(int argc, char *argv[])
{
	int cli_interface[5];
	int ret;

	ret = cli_lib_init(5);
	if (ret < 0) {
		fprintf(stderr, "Failed to initialize CLI!\n");
		return -1;
	}

	cli_interface[0] = cli_start_instance(7608, 5, "first cli instance!!\n", cli1_cmd_list);
	if (cli_interface[0] < 0) {
		fprintf(stderr, "Failed to initialize CLI!\n");
		cli_lib_halt();
		return -1;
	}
	else {
		printf("cli_instance_id = %d\n", cli_interface[0]);
	}

	cli_interface[1] = cli_start_instance(6609, 5, "second cli instance!!\n", cli1_cmd_list);
	if (cli_interface[1] < 0) {
		fprintf(stderr, "Failed to initialize CLI!\n");
		cli_lib_halt();
		return -1;
	}
	else {
		printf("cli_instance_id = %d\n", cli_interface[1]);
	}


	cli_launch(cli_interface[0]);
	//cli_launch(cli_interface[1]);

	while(1) {
		printf("lol!!\n");
		sleep(5);
	}

	cli_lib_halt();

	return 0;
}

