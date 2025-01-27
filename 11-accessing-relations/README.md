Exercise: Accessing relations
=============================

Let's do something more complicated - let's access a relation (=table).
In this exercise we'll learn to lock a relation and access the tuple
descriptor, in the next exercise we'll learn to read rows from it.

To access a relation, we need to lock the relation and get a descriptor
(metadata describing the structure). Then we need to do whatever we
want to do (e.g. print the descriptor info), and finally close the
relation (which unlocks it).

Steps:

1. Define a new SQL function that gets a `regclass` data type. This is
   a special data type that accepts a relation name, and translates it
   into an `oid`. Point it at a new C function that can access the
   parameter using `PG_GETARG_OID`.

2. Get the `OID` and open it using `table_open`. which returns a
   `Relation` struct with all kinds of metadata of the table. Look at
   the definition of `RelationData` in `src/include/utils/rel.h`. Also
   look at `src/include/access/table.h`.

3. The necessary lock level depends on what you intend to do with the
   relation. For our purpose `AccessShareLock` is good enough.

4. Make sure to also close the relation using `table_close`.

5. We want to print information about structure of the table, i.e.
   attributes and types. That is represented by `TupleDesc`, and you
   can obtain it from `Relation` using `RelationGetDescr`.

   ```
   TupleDesc tdesc = RelationGetDescr(rel);
   ```

6. Then you need to iterate over the attributes. There are `tdesc->natts`
   attributes in the descriptor, 0-indexed. To get i-th attribute, use
   `TupleDescAttr` which returns `FormData_pg_attribute` struct. See
   `src/include/catalog/pg_attribute.h` for details of the fields.

7. For each attribute, print `attnum`, `attname`, `atttypid`, `attlen`
   and anything else you deem interesting.

8. Build the extension and try running it on arbitrary table.
