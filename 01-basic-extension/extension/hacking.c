/*
 * hacking.c - a skeleton of a minimal extension
 */

#include "postgres.h"
#include "fmgr.h"

PG_MODULE_MAGIC;

/* example function */
PG_FUNCTION_INFO_V1(hacking_function);

void
_PG_init(void)
{
	/* module load callback */
}

Datum
hacking_function(PG_FUNCTION_ARGS)
{
	PG_RETURN_VOID();
}
