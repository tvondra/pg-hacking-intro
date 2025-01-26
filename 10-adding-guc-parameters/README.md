Exercise: Adding GUC parameters
===============================

GUC stands for Grand Unified Configuration, and it's also used as
"config parameter" in the postgresql.conf file. Adding a new parameter
can be handy during development, to allow "enabling/disabling" the new
behavior in an easy way.

A new GUC can be added in two ways - in an extension (easy) and in the
core server (harder). There are differences, e.g. GUC from an extension
has to have a "prefix".

In the following exercise we'll add a new GUC - first to the core, then
in an extension.

Steps:

1. Find and open the file `guc_tables.c`, which defines which GUCs are
   built into the core server itself. Skim through the file, to get
   some basic idea what it contains - some basic "definitions" first
   (e.g. labels, constants, ...) followed by tables of options with
   different types (bool, int, ...).

2. Find lines with `ConfigureNames` definitions. What types of options
   we support, what types? How do the tables end?

3. Let's add a new config option - a true/false flag to drive something.
   The easiest way is to find a similar entry, copy and modify it.
   Copy `enable_seqscan` option, rename it to `enable_my_feature` or
   whatever else.

4. You'll need to point the GUC to a new variable. `enable_seqscan` uses
   variable defined in `cost.h` and `cost.c`, so just copy those too.
   Now every place that includes `cost.h` can use the C variable backing
   the GUC option.

5. Rebuild the server, restart the instance, try setting the new `GUC`.

6. The `config_bool` GUC option type is defined in `guc_tables.h`, see
   what are the fields. What are the hooks for?

7. Now let's do a similar thing from an extension. There's an API for
   adding GUC options - try
   
   ```
   git grep DefineCustom
   ```

   and see how e.g. `passwordcheck` extension does that.

8. Try adding a new GUC option to the extension. You'll need to copy
   both the `DefineCustomIntVariable` and `MarkGUCPrefixReserved` calls,
   and define the C variable referenced by the define call.
   
9. Use the GUC in one of the functions (e.g. add it to `elog` call), set
   the option and see the function uses the new value.

10. Both approaches allow who/when can set the option (e.g. PGC_USERSET)
    and logical group (e.g. DEVELOPER_OPTIONS).
