/*
 * hacking.c - a skeleton of a minimal extension
 */

#include "postgres.h"
#include "fmgr.h"

PG_MODULE_MAGIC;

/* example function */
PG_FUNCTION_INFO_V1(hacking_function);
PG_FUNCTION_INFO_V1(hacking_function_2);

void
_PG_init(void)
{
	/* module load callback */
}

Datum
hacking_function(PG_FUNCTION_ARGS)
{
	elog(WARNING, "hello world");

    Assert(false);

	ereport(ERROR,
			(errcode(ERRCODE_INSUFFICIENT_PRIVILEGE),
			 errmsg("you don't have right to run this function"),
			 errdetail("access to this function is restricted, because ....")),
			 errhint("... do this to grant access to this function"));

	PG_RETURN_VOID();
}

Datum
hacking_function_2(PG_FUNCTION_ARGS)
{
	int32	value = PG_GETARG_INT32(0);

	elog(WARNING, "value = %d", value);

	PG_RETURN_VOID();
}
