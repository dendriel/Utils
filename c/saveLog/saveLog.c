 /***************************************************************************************************
 * Description: Log function with variable number of args.
 * Author: vitor.rozsa
 * Contact: vitor.rozsa@hotmail.com
 * Creation date: 09/01/2013
 * Note: You also can whant to see "Utils/c/debug/debug.h".
 **************************************************************************************************/

#include <stdio.h>
#include <stdarg.h>

/* Define macros */
#define LOG_NAME "test.log"
#define STDERR stderr

/* Declare functions */
void save_log(char* logdest, const char *format, ...);

/*
 * \b Main function. Save some data in the log file.
 */
int main(int args, char *argv[])
{
	char *l = "lol";

	save_log(NULL, "Vitor Rozsa! %s, %d, %s\n", l, 123, "lal");
	save_log("teste2", "tetasfafa %s %d %x", "tentadatrive", 904737, l);
	save_log("teste3.log", "adsadasdasda\n");

	return 0;
}

void save_log(char* log_dest, const char *format, ...)
{
	FILE *log_file = NULL;
	va_list arg;
	char *log_name = NULL;

	if(log_dest != NULL) {
		log_name = log_dest;
	}
	else {
		log_name = LOG_NAME;
	}

	/* Open or create log file. */
	if((log_file = fopen(log_name, "a+")) == NULL) {
		fprintf(STDERR, "Failed to open/create log file!\n");
		return;
	}

	/* Starts the arg list. */
	va_start(arg, format);

	if(vfprintf(log_file, format, arg) < 0) {
		fprintf(STDERR, "Failed to write in the file!\n");
	}

	/* Finish the arg list. */
	va_end(arg);

	/* Close file. */
	fclose(log_file);

	return;
}


/*__printf (const char *format, ...)
*{
*   va_list arg;
*   int done;
*
*   va_start (arg, format);
*   done = vfprintf (stdout, format, arg);
*   va_end (arg);
*
*   return done;
*}
*/
