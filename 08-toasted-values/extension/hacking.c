/*
 * hacking.c - a skeleton of a minimal extension
 */

#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "varatt.h"

PG_MODULE_MAGIC;

/* example function */
PG_FUNCTION_INFO_V1(hacking_function);
PG_FUNCTION_INFO_V1(hacking_function_2);
PG_FUNCTION_INFO_V1(hacking_function_3);

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

Datum
hacking_function_3(PG_FUNCTION_ARGS)
{
	text	*msg = (text *) PG_DETOAST_DATUM(PG_GETARG_DATUM(0));
	char	*msg_str = VARDATA_ANY(msg);
	int		msg_size = VARSIZE_ANY_EXHDR(msg);

	elog(WARNING, "message = %s, size = %d", msg_str, msg_size);

	PG_RETURN_CSTRING(text_to_cstring(msg));
}
