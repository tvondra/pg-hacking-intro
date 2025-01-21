Exercise: TAP regression test
=============================

This exercise adds a basic regression test written in Perl using TAP
(Test Anything Protocol).

Each TAP test is a single Perl script, checking results of various
actions (e.g. result of a query, exit code of a binary ...).  Compared
to SQL regression tests (in previous exercise), TAP allows imperative
programming and thus more complex workflows, multiple instances,
restarts, etc.

We'll create a minimum TAP test, that initializes an instance with a
custom configuration, checks a result of a result, restarts the
instance and checks the log. And then shuts the instance.

Steps:

1. Go to the "extension" directory, create an empty file

   `t/001_basic.sql`

   and run `make installcheck`. It should try to run the test and fail,
   complaining about the file not defining any tests.

2. Add this minimal skeleton of a TAP test to the file. It create an
   instance, starts it, stops it, and finishes testing.

```
use strict;
use warnings FATAL => 'all';

use PostgreSQL::Test::Cluster;
use PostgreSQL::Test::Utils;
use Test::More;

my $node = PostgreSQL::Test::Cluster->new('main');

$node->init;

$node->start;

... some tests ...

$node->stop;

done_testing();
```

    Try running `make installcheck` again. It should fail, there are
    still no "checks" in the test.

3. To run a query, you'll need to use a function `safe_sql`, defined in
   the PostgreSQL Cluster module.

   ```
   $node->safe_psql($dbname, $sql) => stdout
   ```

   Use it to (a) create the extension in a database (you may use either
   "postgres" or create a new one), and (b) run the function defined in
   the extension.

   Searching for definitions in the PostgreSQL git repository can be
   easily done using `git grep`, for example like this:

   ```
   git grep safe_psql -- *.pm
   ```

   There's also a lot of existing TAP tests, which you can find using

   ```
   git grep safe_psql -- *.pl
   ```

   Use them as an inspiration, i.e. copy one of them.


4. You'll also need to add a "check" for the TAP test to actually test
   anything. One of the existing TAP tests certainly does that using
   Perl functions as `like(...)` or `is(...)`.


5. Once you add the missing commands, try running `make installcheck`.


6. There's a lot of TAP tests already, mostly in the `src/test/...`
   subdirectories. Go and explore e.g. `src/test/recovery` (which tests
   recovery and physical replication) or `src/test/subscription`, which
   tests logical replication, etc.
