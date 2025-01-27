Exercise: Accessing tuples
==========================

In the previous exercise we opened a relation and printed some basic
info from the tuple descriptor. In this example we'll actually access
tuples - we'll scan the relation (using sequential scan), count the
tuples, and possibly also parse the tuples.

Steps:

1. To access tuples, we need to do the same basic stuff as in the
   earlier exercise - open/lock the relation, etc. But then we also
   need to start a "scan" using `table_beginscan`.
   
   This is part of the `table AM` API defined in `access/tableam.h`, so
   maybe take a look at that. Then search for places already doing this
   (`pgrowlocks.c`). We're however going to mix this with "heap" API,
   because we know that's the storage format. But it's not quite right.

2. Once we have a scan, you can call `heap_next()` to get `HeapTuple`
   from the scan, until you get NULL.
   
   Simply increment a counter, and return it from the function (so the
   function should now count rows, not attributes). Overall it should
   look like this:
   
   ```
	Datum
	hacking_function_4(PG_FUNCTION_ARGS)
	{
		// get OID of relation for argument 0

		// open the relation (table_open)

		// begin a scan on the relation (table_beginscan)

		// iterate tuples (heap_getnext)

		// return close the relation (table_close)

		// return the count (PG_RETURN_INT32)
	}

   ```

3. Rebuild/reinstall the extension, try calling the function. Did it
   work, did it print some warnings?

4. Add the missing `table_endscan` call.

5. Instead of just counting the rows, you can try doing something else
   with the tuples. For example, you can use `HeapTupleHeaderGetNatts`
   to show the number of attributes in a tuple.
   
   Question: When could this be different from the tuple descriptor?

   Similarly, you can access attributes using the `heap_getattr`
   function, which returns `Datum` value and sets `isnull` flag. Search
   for places doing that, and try doing that.

6. Another thing you might try is defining scan keys (i.e. conditions).
   For that you need to allocate `ScanKeyData` and pass it to the
   `beginscan` function.

   See for example how `aclchk.c` does this. It's on catalogs, but the
   general approach is exactly the same.

7. These examples mix `heap` and `table AM` code, because we know the
   relation is `heap` and thus the rows are `HeapTuple`. But in general
   that would not work, and we should use the `table AM` consistently.
   But that uses a concept of "slot" as an abstract tuple format. The
   executor however does exactly that.
