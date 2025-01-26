Exercise: Memory contexts
=========================

Handling memory allocations in C is difficult and error-prone. You have
to track exactly what you allocated and then free it at the appropriate
time. It's easy to end up with either memory leaks, double-free or
use-after-free bugs etc.

This is especially true true for systems where the different pieces of
memory have vastly different life spans. Some memory needs to exist for
as long as the backend is running, other pieces are per transaction,
query, operation, row, ...

This is why Postgres implements the concept of "memory context", which
is "collection" of allocations, grouped by life span. Memory context
form a hierarchy, from long-lived to short-lived.

There is a small number of pre-defined memory contexts:

```
// memutils.h
extern PGDLLIMPORT MemoryContext TopMemoryContext;
extern PGDLLIMPORT MemoryContext ErrorContext;
extern PGDLLIMPORT MemoryContext PostmasterContext;
extern PGDLLIMPORT MemoryContext CacheMemoryContext;
extern PGDLLIMPORT MemoryContext MessageContext;
extern PGDLLIMPORT MemoryContext TopTransactionContext;
extern PGDLLIMPORT MemoryContext CurTransactionContext;
```

for contexts that are expected to exist. The names should hint what the
purpose is. There is also `CurrentMemoryContext` which always points to
the "current" context (e.g. what the caller sets before calling
a function). To switch to a different memory context the idiom is:

```
MemoryContext oldcxt = MemoryContextSwitchTo(newcxt);

... allocate stuff in current context ...

MemoryContextSwitchTo(oldcxt);
```

You may create a new memory context (e.g. to wrap all allocations in
a function, and free them at once). There are multiple implementations,
the most widely used / general purpose is `AllocSet` (see `aset.c`).

Creating a context is simple:

```
MemoryContext ctx = AllocSetContextCreate(CurrentMemoryContext,
                                          "name of the context",
                                          ALLOCSET_DEFAULT_SIZES);
```

After this, you can access the context using the API in `memutils.h`.

To manage memory in a context, you must not use `malloc` / `free` or any
of that glibc API. Instead, use `palloc` / `pfree`, or the API defined
in `palloc.h`.

Memory contexts also serve as "cache" on top of `malloc`. That is, when
you do `palloc`, the context will allocate a larger block of memory using
`malloc` and then use it to handle many `palloc` calls before the next
`malloc` call. Also, `pfree` does not return memory to glibc, it just
returns it to the context (and it can use it for a `palloc` with similar
size).

To destroy a memory context, use either `MemoryContextDelete` or
(if you want to keep using it) `MemoryContextReset`.

In the following exercise we'll modify the `hacking_function` to create
a new memory context, do the allocations in it, and then discard the
allocated memory.

Steps:

1. Open the `hacking_function` from previous exercise, and create a new
   memory context using `AllocSetContextCreate` above. Also add
   `MemoryContextDelete` at the end.

2. Switch to the memory context (and then back) using the
   `MemoryContextSwitchTo` idiom mentioned above.

3. Add some `palloc` calls between the switch-to calls, to allocate
   memory in the new memory context. It could be a couple bytes,
   kilobytes or MBs.

4. There's a handy function `MemoryContextStats` for inspecting memory
   context state - how much memory is allocated, etc. Try adding a call
   with either `TopMemoryContext` or the new context, and see what
   happens. The output goes to server log.
   
5. The function can also be called from gdb, which is useful while
   investigating issues. Attach GDB to running backend and try this:
   
   ```
   call MemoryContextStats(TopMemoryContext);
   ```

6. Memory contexts have built-in protections in development builds
   (with asserts enabled), e.g. randomization of memory. This is useful
   to identify use-after-free bugs etc. because the patterns are quite
   typical.
   
   Try defining a struct with int32/int64/char[] fields, allocate it
   using palloc, free it (or destroy the context), and try accessing it.
   Or inspect the contents using gdb.  
   
7. Memory contexts also have optional valgrind support, if you build
   with `CPPFLAGS="-DUSE_VALGRIND"`. This is useful for investigating
   strange memory corruption issues - valgrind tells you not only where
   the crash happened, but when was the memory allocated/freed etc.
   It's much slower, though (roughly 100x slower).

8. Are memory contexts a solution to memory leaks? Can you think about
   cases when we'd still leak memory?
