Exercise: Basic logging
=======================

Let's add some basic logging to the C code, writing messages either to
server log or to the client. This is mostly about `elog` and `ereport`
functions, defined in Postgres.

Steps:

1. Open the `hacking.c` file, and add this to the `hacking_function`:

   ```
   elog(WARNING, "hello world");
   ```

   Then rebuild the extension by doing `make install` (and also reopen
   connection to the database, to load the new `.so`).

2. Now run the function from `psql`:

   ```
   SELECT hacking_function();
   ```

   You should see the warning.

3. Search for the `elog` definition, e.g. using `git grep` again:

   ```
   git grep elog -- *.h
   ```

   which should point you to `src/include/utils/elog.h`.

4. Try changing the level to `ERROR`, and run the function again and
   check the server log.

5. Try setting `log_error_verbosity = verbose`, run the function and
   check the server log again.

6. The `elog` API is simple, but very limited - no translation support,
   no context information, ... It's meant to be used only for internal
   messages the user is not expected to see (e.g. can't happen errors).

   There's a more elaborate `ereport` interface, addressing all those
   limitations:

   ```
   ereport(ERROR,
           (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
            errmsg("compression method lz4 not supported"),
            errdetail("This functionality requires the server to be built with lz4 support.")));
   ```

   It's defined in the same `elog.h` header, and it supports "context"
   in  the form of `errcode`, `errmsg`, `errdetail`, `errhint` etc.
   It's porobably easier to look for places that emit similar message
   (e.g. check privileges, data types, ...).

   The errcodes are defined in the SQL standard and you may find a list
   in the `src/backend/utils/errcodes.txt` file.

7. Modify the extension to use the `ereport` interface. Add `errhint`
   and `errdetail` with additional details that should be logged.

8. Try runinng the tests using `make installcheck` again, fix them.
