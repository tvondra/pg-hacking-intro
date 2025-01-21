Exercise: SQL regression test
=============================

This exercise adds a trivial SQL regression test, to do simple testing
of the extension code.

SQL tests simply define (a) file with SQL queries/commandss, executed
in a single session/connection, and (b) another file with expected
output. This is a great way to do basic testing of changes that affect
results of SQL queries etc.

Steps:

1. Go to the "extension" directory. This is the same extension as in
   the previous exercise.

2. Try running `make check` and `make installcheck`.

   `make check` is not supported for out-of-tree extension, and fails

   `make installcheck` is supported, but requires a running instance

3. If you don't have an instance running, create one and try running
   `make installcheck` again. It should succeed (there are no tests).

4. The `Makefile` expects files with SQL tests in the `test/sql`
   directory, so create `test/sql/mytest.sql` and add some SQL commands
   to it. For example a trivial command:

       SELECT 1;

5. Now run `make installcheck` again. It should fail, because there is
   no file with expected output yet - add `test/expected/mytest.out`
   and run `make installcheck` again.

   This time it fails because the actual and expected outputs differ.
   See `regressions.diff` with a comparison of the files.

6. Modify the expected output file to have the correct output, as
   produced by the SQL script. Run `make installcheck` again, this time
   it should pass.

7. The test is not testing any actual extension code yet. Modify the
   SQL script to include this

   ```
   CREATE EXTENSION hacking;
   SELECT hacking_function();
   ```

   and run `make installcheck` again. If you haven't done `make install`
   earlier, it may fail because of that - so install it.

   It may also fail because the expected output is not updated yet, so
   update it and run the test, to make sure it passes.
