/*
 * hacking.c - a skeleton of a minimal extension
 */

#include "postgres.h"
#include "access/heapam.h"
#include "access/relscan.h"
#include "access/table.h"
#include "access/tupdesc.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/memutils.h"
#include "utils/rel.h"
#include "varatt.h"

PG_MODULE_MAGIC;

/* example function */
PG_FUNCTION_INFO_V1(hacking_function);
PG_FUNCTION_INFO_V1(hacking_function_2);
PG_FUNCTION_INFO_V1(hacking_function_3);
PG_FUNCTION_INFO_V1(hacking_function_4);

void
_PG_init(void)
{
	/* module load callback */
}

Datum
hacking_function(PG_FUNCTION_ARGS)
{
	MemoryContext old_ctx, ctx;
	char	*tmp;

	ctx =  AllocSetContextCreate(CurrentMemoryContext,
                                "hacking function context",
                                ALLOCSET_DEFAULT_SIZES);

	old_ctx = MemoryContextSwitchTo(ctx);

	tmp = (char *) palloc(8192*10);
	pfree(tmp);

	MemoryContextStats(ctx);
	MemoryContextSwitchTo(old_ctx);
	MemoryContextDelete(ctx);

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
	text	*msg;
	char	*msg_str;
	int		msg_size;
	char   *ret;

	MemoryContext oldctx;

	/* create a local context for our temporary stuff */
	MemoryContext ctx = AllocSetContextCreate(CurrentMemoryContext,
											  "my test context",
											  ALLOCSET_DEFAULT_SIZES);

	/* switch to the new local context */
	oldctx = MemoryContextSwitchTo(ctx);

	msg = (text *) PG_DETOAST_DATUM(PG_GETARG_DATUM(0));
	msg_str = VARDATA_ANY(msg);
	msg_size = VARSIZE_ANY_EXHDR(msg);

	for (int i = 0; i < 100; i++)
	{
		char   *ptr = palloc(1328);
		pfree(ptr);
	}

	/* switch "back" to the original context */
	MemoryContextSwitchTo(oldctx);

	/* print top-level context stats, then stats for our context */
	MemoryContextStats(TopMemoryContext);
	MemoryContextStats(ctx);

	elog(WARNING, "message = %s, size = %d", msg_str, msg_size);

	/* have to do the conversion before destroying the source */
	ret = text_to_cstring(msg);

	/* delete the next context */
	MemoryContextDelete(ctx);

	/* print the top-level stats again */
	MemoryContextStats(TopMemoryContext);

	PG_RETURN_CSTRING(ret);
}


Datum
hacking_function_4(PG_FUNCTION_ARGS)
{
	Oid			relid = PG_GETARG_OID(0);
	Relation	rel;
	int32		cnt = 0;
	TupleDesc	tdesc;
	TableScanDesc	scan;
	HeapTuple		tuple;
	

	/* open relation */
	rel = table_open(relid, AccessShareLock);

	/* get the tuple descriptor */
	tdesc = RelationGetDescr(rel);

	/* begin sequential scan (no index, no keys) */
	scan = table_beginscan(rel, GetActiveSnapshot(), 0, NULL);

	/* iterate through the tuples */
	while ((tuple = heap_getnext(scan, ForwardScanDirection)) != NULL)
	{
		Datum	value;
		bool	isnull;

		elog(WARNING, "natts = %d", HeapTupleHeaderGetNatts(tuple->t_data));
		cnt++;

		value = heap_getattr(tuple, 1, tdesc, &isnull);

		elog(WARNING, "value %ld isnull %d", value, isnull);
	}

	table_endscan(scan);

	table_close(rel, AccessShareLock);

	PG_RETURN_INT32(cnt);
}
