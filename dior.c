#include "dior.h"
#include <unistd.h>

void main(void)
{
	Cmd *cmd = cmd_new();
	cmd_append(cmd, "cc", "dior.c", "-o", "exe");
	cmd_run(cmd);
}
