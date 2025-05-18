#ifndef FLINT_H
#define FLINT_H

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdbool.h>

#define flint_print_error(fmt, ...)	printf("[" C1 "ERROR" C0"] " fmt "\n", ##__VA_ARGS__)
#define flint_print_info(fmt, ...)	printf("[" C2 "INFO" C0"] " fmt "\n", ##__VA_ARGS__)

typedef struct {
	char **args;
	int len;
} Flint_Cmd;

void flint_rebuild_yourself();
Flint_Cmd flint_cmd_new();
#define flint_cmd_append(cmd, ...) _flint_cmd_append(cmd, __VA_ARGS__, NULL)
void _flint_cmd_append(Flint_Cmd *cmd, ...);
int flint_cmd_run(Flint_Cmd *cmd);
#define flint_run_cmd(arg, ...) _flint_run_cmd(arg, ##__VA_ARGS__, NULL)

#ifdef FLINT_IMPLEMENTATION

void _flint_cmd_append_va(Flint_Cmd *cmd, va_list ap);

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

Flint_Cmd flint_cmd_new()
{
	Flint_Cmd cmd = {0};
	return cmd;
}

void _flint_cmd_append(Flint_Cmd *cmd, ...)
{
	va_list ap;
	va_start(ap, cmd);
	_flint_cmd_append_va(cmd, ap);
	va_end(ap);
}

void _flint_cmd_append_va(Flint_Cmd *cmd, va_list ap)
{
	va_list ap1;
	va_copy(ap1, ap);

	const char *arg = va_arg(ap, const char *);

	while (arg) {
		cmd->args = realloc(cmd->args, sizeof(char *) * ++cmd->len);
		cmd->args[cmd->len - 1] = strdup(arg);
		arg = va_arg(ap, const char *);
	}

	va_end(ap1);
}

void flint_cmd_print_arg(const char *arg)
{
	if (strchr(arg, ' ')) {
		printf("'%s'", arg);
	} else {
		printf("%s", arg);
	}
}

void flint_cmd_print(const Flint_Cmd *cmd)
{
	printf("[" C2 "CMD" C0 "]");

	for (int i = 0; i < cmd->len; i++) {
		printf(" ");
		flint_cmd_print_arg(cmd->args[i]);
	}

	printf("\n");
}

int flint_cmd_run(Flint_Cmd *cmd)
{
	// add NULL termintator
	cmd->args = realloc(cmd->args, sizeof(char *) * (cmd->len + 1));
	cmd->args[cmd->len] = NULL;

	flint_cmd_print(cmd);

	pid_t pid = fork();
	int exit_code;

	if (pid == -1) {
		flint_print_error("fork couln't create child");
	} else if (pid == 0) {
		execvp(cmd->args[0], cmd->args);
	} else {
		waitpid(pid, &exit_code, 0);
	}

	if (exit_code) {
		flint_print_error(C1 "exited abnormally " C0 "with code " C1 "%d" C0, exit_code);
	}

	return exit_code;
}

int _flint_run_cmd(const char *arg, ...)
{
	va_list ap;
	va_start(ap, arg);

	Flint_Cmd cmd = flint_cmd_new();
	flint_cmd_append(&cmd, arg);
	_flint_cmd_append_va(&cmd, ap);

	return flint_cmd_run(&cmd);
}

bool flint_should_flint_rebuild_yourself()
{
	struct stat exe_stat;
	struct stat src_stat;

	stat("./flint", &exe_stat);
	stat("./flint.c", &src_stat);

	if (src_stat.st_mtim.tv_sec > exe_stat.st_mtim.tv_sec) {
		return true;
	}

	return false;
}

void flint_rebuild_yourself()
{
	if (!flint_should_flint_rebuild_yourself()) {
		return;
	}

	int status = flint_run_cmd("cc", "flint.c", "-o", "flint");

	if (status != 0) {
		exit(status);
	}

	exit(flint_run_cmd("./flint"));
}

#endif // FLINT_IMPLEMENTATION
#endif //FLINT_H
