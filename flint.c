#include "flint.h"
#include <unistd.h>

void main(void)
{
	Cmd *cmd = cmd_new();
	cmd_append(cmd, "cc", "flint.c", "-o", "exe");
	cmd_run(cmd);
}
