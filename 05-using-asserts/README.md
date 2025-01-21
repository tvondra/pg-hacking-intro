Exercise: Using asserts
=======================

Asserts are a convenient way to enforce conditions (invariants) during
development. This should prevent "can't happen" cases and verify "must
happen" cases.

This is an "extension" of the asserts provided by standad library,
integrated into the Postgres configue / build system. In production
builds asserts are disabled.

Steps:

1. Open the `hacking.c` file, and add this to the `hacking_function`:

   ```
   Assert(false);
   ```

   This will always fail and call "abort()". So rebuild/reinstall the
   extension, and run tests using `make installcheck`. Did it fail?

2. If it didn't fail, that's probably because you did not enable asserts
   when building Postgres. So use `--enable-casserts` configure flag and
   rebuild Postgres 

   ```
   ./configure --enable-cassert  ... CPPFLAGS="-O0 -ggdb3"
   ./make -s -j4 install
   ```

   And try running the tests again. It should fail.

   Note: The `CPPFLAGS` is optional - it disables optimizations, which
   makes it easiert to analyze core dumps.

3. It's often useful to investigate why/where an ABORT happened, which
   you can approach in various ways. First, inspect the server log,
   which may have information about the command that failed.

4. Alternatively, attach debugger (e.g. `gdb`) to the backend before
   running the command that fails. The PID may be easily determined
   using the `pg_backend_pid()` function, and `gdb` can attach to the
   process using the `-p` switch.

   ```
   SELECT pg_backend_pid();
    pg_backend_pid 
   ----------------
            802583
   (1 row)
   ```

   ```
   gdb -p 802583
   ```

   Make the backend execute using the `c` command, and invoke the SQL
   function that triggers the assert.

   ```
   SELECT hacking_function()
   ```

   The `gdb` session should interrupt, allowing you to investigate.


6. Alternatively, you may enable `core` files, so that the failing
   backend dumps memory into a file. To do that, you may need to (a)
   set the limit on core file size, and (b) specify where the core
   files should be written.

   For (a), run `ulimit -c` and if it says `0`, then modify
   `/etc/security/limits.conf` to make it `unlimited`. You may need
   to start a new session and restart the instance from it.

   For (b), run `sysctl kernel.core_pattern` and make sure it points
   to existing directory. There's a list of format specifiers for the
   field in [kernel](https://www.kernel.org/doc/Documentation/sysctl/kernel.txt).
   For example this will ensure core files are written to `/mnt/data/cores`
   directory, and each `core` file will have PID suffix:
   
   `sysctl -w kernel.core_pattern=/mnt/data/cores/core.%p`

   With this in place (and instance restarted), run `make installcheck`
   again. You should get a core file.

6. Now inspect the core file using `gdb` by speficying the paths to the
   postgres binary and the core file.

   ```
   gdb ~/builds/master/bin/postgres /mnt/data/cores/core.34432
   ```

7. Now you can inspect a complete state of the process at the moment of
   the crash. The process it not running, of course, but you can use
   gdb commands like `print`, `up`, `down` etc.
