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
	elog(WARNING, "hello world");

	ereport(ERROR,
			(errcode(ERRCODE_INSUFFICIENT_PRIVILEGE),
			 errmsg("you don't have right to run this function"),
			 errdetail("access to this function is restricted, because ....")),
			 errhint("... do this to grant access to this function"));

	PG_RETURN_VOID();
}
