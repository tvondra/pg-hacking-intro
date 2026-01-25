Exercise: Simple data types
===========================

Postgres has an extensible type system, but in principle you can divide
the data types in two dimensions:

* passed by value vs. passed by reference
* fixed-length vs. variable length

There are no variable-length types passed by value, which means there
are three combinations:

* passed by value / fixed length (e.g. integer)
* passed by reference / fixed length (e.g. macaddr)
* passed by reference / variable length (e.g. text)

This exercise is about the first category, which is the simplest to
work with. The next exercise will be about the third category.

You can see all this in the `pg_type` catalog, which lists all the data
types known to Postgres.

The types between SQL and C are quite different, and the C type system
is not as extensible as in Postgres. These type systems are not easily
compatible, e.g. the casting is much more restrictive in C, so there
needs to be some "compatibility" layer.

Similarly for the function call interface - the call conventions are
very different (SQL allows overloading, ....).

The compatibility layer between SQL and C types is represented by the
"Datum" data type, which can represent any SQL data type. If you only
have a `Datum` value, you don't know if it's for a by-value type (and
how many bytes are valid) or for by-reference type (i.e. a pointer).

You need enough "context" - either implicit (when accessing a function
argument, you know the type) or explicit (when processing tuples, you
have tuple descriptor with info about types of attributes).


Steps:

1. Open the `hacking.c` file, and add copy the `hacking_function` into
   a new `hacking_function_2` one.

2. In the SQL script, copy the function definition, add a new `int`
   argument and make sure it references the new `hacking_function_2`.

3. Modify the C function to have this piece of code:

   ```
   int32  value = PG_GETARG_INT32(0);
   
   elog(WARNING, "value = %d", value);
   ```

4. Rebuild / reinstall the extension (you have to drop / create it, and
   reconnect to load the new `.so`).

5. Try calling the function with the `int` argument.

   ```
   SELECT hacking_function(1244);
   ```

   And you should get a warning message.

6. Explore what `PG_GETARG_INT32` actually does:

   ```
   git grep PG_GETARG_INT32 -- *.h
   ```

   The interesting parts are in `fmgr.h` (for int and many other
   built-in data types), where it says this:
   
   ```
   #define PG_GETARG_INT32(n)	 DatumGetInt32(PG_GETARG_DATUM(n))
   ```

7. `PG_GETARG_DATUM` extracts the argument `0` from function arguments,
   which are hidden by another macro `PG_FUNCTION_ARGS`.

   ```
   #define PG_GETARG_DATUM(n)	 (fcinfo->args[n].value)
   ```
   
   Ultimately it's accessing `FunctionCallInfoBaseData`, i.e. an array
   of `Datum` values, passed to the function (and some more data).
   The "fmgr" means "function manager" and it's the layer at the
   boundary between SQL and C functions.

8. `DatumGetInt32` is what does the actual casting. You can see what it
    does in `postgres.h` - it mostly just casts to/from a `Datum` value.

9. SQL functions can handle arguments in smarter ways, using `PG_NARGS`
    and `PG_ARGISNULL` to handle calls with NULLs and different numbers
    of arguments etc.
