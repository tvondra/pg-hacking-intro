Exercise: TOASTed data types
============================

This is a follow up to the previous exercise about `varlena` data types,
that allow values with arbitrary length. Well, up to 1GB. Still, that's
much more than can fit into a single data page (8kB by default).

So how does Postgres handle that? Let's explore that a bit!

Steps:

1. Create a table with a single `text` column, and insert a long random
   value into it:

   ```
   CREATE TABLE t (a text);
   INSERT INTO t SELECT string_agg(md5(i::text),'') FROM generate_series(1,1000) s(i);

   -- see how much disk space it uses
   \d+ t

   -- see how long it is
   SELECT length(a) FROM t;
   ```

2. Try calling the `hacking_function(text)` on the value:

   ```
   SELECT hacking_function(a) FROM t
   ```

   This should fail, printing some garbage. How come?

3. Postgres stores over-sized values (that can't fit onto a single 8kB
   data page) in a TOAST table. It can also compress them transparently,
   using either pglz or lz4. Each table with attributes that might need
   TOAST have an associated TOAST relation.

   ```
   SELECT toastrelid::regclass FROM pg_class WHERE relname = 't';
   
   SELECT * FROM pg_toast.pg_toast_24801;
   ```

4. If a value gets TOASTed (longer than some threshold ...), it's replaced
   with a TOAST pointer which contains the key into the TOAST relation.
   Which is why the function printed nonsense.

5. Explore the possible ways an attribute may be stored. This depends on
   the data type, but also on the column options.

   See [docs](https://www.postgresql.org/docs/current/storage-toast.html) for
   details about PLAIN/EXTENDED/EXTERNAL/MAIN, compression etc.

6. All the details and macros for accessing `varlena` types are in `varatt.h`
   and you can see how it maps to the storage features we just discussed.
   There are variants for EXTERNAL, COMPRESSED, ...

7. It's time to fix the function, to print valid data. The important part
   is explained by this comment:
   
   ```
   /*
    * In consumers oblivious to data alignment, call PG_DETOAST_DATUM_PACKED(),
    * VARDATA_ANY(), VARSIZE_ANY() and VARSIZE_ANY_EXHDR().  Elsewhere, call
    * PG_DETOAST_DATUM(), VARDATA() and VARSIZE().  Directly fetching an int16,
    * int32 or wider field in the struct representing the datum layout requires
    * aligned data.  memcpy() is alignment-oblivious, as are most operations on
    * datatypes, such as text, whose layout struct contains only char fields.
    *
    * Code assembling a new datum should call VARDATA() and SET_VARSIZE().
    * (Datums begin life untoasted.)
    *
    * Other macros here should usually be used only by tuple assembly/disassembly
    * code and code that specifically wants to work with still-toasted Datums.
    */
   ```

8. Modify the function to use `PG_DETOAST_DATUM(), VARDATA() and VARSIZE()`.

   Try doing `git grep PG_DETOAST_DATUM` to see how to call it.

9. Have you noticed there's `DatumGetTextP`? How would you use this?

10. Try to find the code that actually does the detoasting.

    You'll need to get through multiple layers of indirection
    (macro -> AM callback -> ...), but ultimately you should get to
    
    * `detoast_attr` in `detoast.c`
    * `heap_fetch_toast_slice` in `heaptoast.c`

11. There's also `PG_DETOAST_DATUM_SLICE` / `DatumGetTextPSlice`. Why
    would it be interesting?

12. It's possible to define functions with `internal` data type, and
    use it to pass "raw" pointers (and use `PG_GETARG_POINTER`). In
    what situations might that be useful?
