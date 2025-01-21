Exercise: Basic extension
=========================

This exercise creates a minimal extension skeleton, with a single SQL
function that does nothing.

Extensions are useful because they allow adding code without having to
rebuild / reinitialize cluster. If you can, start experimenting with
an extension instead of hacking on the core right away, it will make
your life much easier.

Hint: Start by looking for extensions that alredy do something similar,
and use them as a starting point.

Steps:

1. Go to the "extension" directory. The extension is called "hacking".

2. Explore the "hacking.control" file, with metadata. This is used by
   the server to know which version to install and various other
   features of the extension.

   For more details about all the available options see:
   https://www.postgresql.org/docs/current/extend-extensions.html#EXTEND-EXTENSIONS-FILES
   
3. Explore the "hacking.c" file. This is a minimal source, defining a
   function that is callable through SQL and that returns "nothing".

4. Explore the "sql/hacking--1.0.0.sql" script, which is what defines
   the SQL definitions for the C code - in this case a single function.
   The naming convention uses semantic versioning and upgrade scripts.

5. Explore the "Makefile" which defines how to build and install the
   extension, and various other things (testing, ...). This is a pretty
   regular makefile, except that it utilizes PGXS to interact with
   the Postgres installation (e.g. to determine where to install
   various files).
   
   For details about PGXN see:
   https://www.postgresql.org/docs/current/extend-pgxs.html

6. Run `make install` in the extension directory.

7. Connect to the database and execute `CREATE EXTENSION hacking`.

8. Run `SELECT hacking_function()`.
