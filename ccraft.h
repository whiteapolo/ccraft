#ifndef CCRAFT_H
#define CCRAFT_H

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

typedef struct {
	char **args;
	int len;
} Cmd;

Cmd *cmd_new();
#define cmd_append(cmd, ...) _cmd_append(cmd, __VA_ARGS__, NULL)
void _cmd_append(Cmd *cmd, ...);
void cmd_run(Cmd *cmd);

Cmd *cmd_new()
{
	return calloc(1, sizeof(Cmd));
}

void _cmd_append(Cmd *cmd, ...)
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

void cmd_print(const Cmd *cmd)
{
	printf("[CMD]");

	for (int i = 0; i < cmd->len; i++) {
		strchr(cmd->args[i], ' ') ? printf(" '%s'", cmd->args[i]) : printf(" %s", cmd->args[i]);
	}

	printf("\n");
}

void cmd_run(Cmd *cmd)
{
	cmd->args = realloc(cmd->args, sizeof(char *) * (cmd->len + 1));
	cmd->args[cmd->len] = NULL;

	cmd_print(cmd);

	if (!fork()) {
		execvp(cmd->args[0], cmd->args);
	} else {
		wait(NULL);
	}
}

#endif
