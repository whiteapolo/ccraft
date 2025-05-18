#define FLINT_IMPLEMENTATION
#include "flint.h"

void main(int argc, char **argv)
{
	flint_rebuild_yourself();

	Flint_Cmd cmd;
	flint_cmd_init(&cmd);
	flint_cmd_append(&cmd, "cc", "main.c", "-o", "exe");
	// flint_cmd_append(&cmd, "-O3");
	flint_cmd_run(&cmd);
}
