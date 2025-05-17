#ifndef CCRAFT_H
#define CCRAFT_H

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

#define C0  "\033[0m"     /*  RESET        */

#define C1  "\033[0;31m"  /*  RED          */
#define C2  "\033[0;32m"  /*  GREEN        */
#define C3  "\033[0;33m"  /*  YELLOW       */
#define C4  "\033[0;34m"  /*  BLUE         */
#define C5  "\033[0;035m" /*  MAGENTA      */
#define C6  "\033[0;36m"  /*  CYAN         */
#define C7  "\033[0;37m"  /*  WHITE        */
#define C8  "\033[0;90m"  /*  GRAY         */

#define B1  "\033[1;91m"  /*  BOLD RED     */
#define B2  "\033[1;92m"  /*  BOLD GREEN   */
#define B3  "\033[1;93m"  /*  BOLD YELLOW  */
#define B4  "\033[1;94m"  /*  BOLD BLUE    */
#define B5  "\033[1;95m"  /*  BOLD MAGENTA */
#define B6  "\033[1;96m"  /*  BOLD CYAN    */
#define B7  "\033[1;97m"  /*  BOLD WHITE   */
#define B8  "\033[1;90m"  /*  BOLD GRAY    */

typedef struct {
	char **args;
	int len;
} Cmd;

static Cmd *cmd_new();
#define cmd_append(cmd, ...) _cmd_append(cmd, __VA_ARGS__, NULL)
static void _cmd_append(Cmd *cmd, ...);
static void cmd_run(Cmd *cmd);

static Cmd *cmd_new()
{
	return calloc(1, sizeof(Cmd));
}

static void _cmd_append(Cmd *cmd, ...)
{
	va_list ap;
	va_start(ap, cmd);

	const char *arg = va_arg(ap, const char *);

	while (arg) {
		cmd->args = realloc(cmd->args, sizeof(char *) * ++cmd->len);
		cmd->args[cmd->len - 1] = strdup(arg);
		arg = va_arg(ap, const char *);
	}

	va_end(ap);
}

static void cmd_print(const Cmd *cmd)
{
	printf("[" C2 "CMD" C0 "]");

	for (int i = 0; i < cmd->len; i++) {
		if (strchr(cmd->args[i], ' ')) {
			printf(" '%s'", cmd->args[i]);
		} else {
			printf(" %s", cmd->args[i]);
		}
	}

	printf("\n");
}

static void cmd_run(Cmd *cmd)
{
	cmd->args = realloc(cmd->args, sizeof(char *) * (cmd->len + 1));
	cmd->args[cmd->len] = NULL;

	cmd_print(cmd);

	pid_t pid = fork();
	int exit_code;

	if (pid == -1) {
		printf("[" C1 "ERROR" C0"] fork couln't create child");
	} else if (pid == 0) {
		execvp(cmd->args[0], cmd->args);
	} else {
		waitpid(pid, &exit_code, 0);
	}

	if (exit_code == 0) {
		printf("[" C2 "INFO" C0"] exited normally with code %d\n", exit_code);
	} else {
		printf("[" C1 "ERROR" C0"] exited abnormally with code %d\n", exit_code);
	}
}

#endif
