#define CCRAFT_IMPLEMENTATION
#include "ccraft.h"

void main(void)
{
	Cmd *cmd = cmd_new();
	cmd_append(cmd, "cc", "main.c", "-o", "exe");
	cmd_run(cmd);
}
