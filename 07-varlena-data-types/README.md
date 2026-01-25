Exercise: Varlena data types
===========================

Remember that there are three combinations for data types in PostgreSQL:

* passed by value / fixed length (e.g. integer)
* passed by reference / fixed length (e.g. macaddr)
* passed by reference / variable length (e.g. text)

This exercise is about the third category.

Variable length data types, often called as varlena, refer to data whose
length can vary. There are several types with varying lengths, one example
which will be used in this exercise is `text`.

You can find other varlena type and more information about varlena struct by
running the following command.

```
   git grep 'struct varlena' -- '*\.h'
```

You'll find more varlena types specifically in `c.h`.

Steps:

1. Open the `hacking.c` file, and create a `hacking_function_3` function
   similar to the `hacking_function_2` from previous exercise.

2. In the SQL script, copy the function definition from previous exercise,
   change its argument type to `text` and make sure it references the new
   `hacking_function_3`.

3. Modify the C function to have this piece of code:

   ```
   struct varlena	*msg = PG_GETARG_RAW_VARLENA_P(0);
   ```

   `PG_GETARG_RAW_VARLENA_P` is similar to `PG_GETARG_INT32` that we used
   before. Main difference is that it returns the argument as `varlena` instead
   of an integer.

   You'll find appropriate `PG_GETARG_*` macros in `fmgr.h` for most data
   types. Choose the one that would work with your argument type.

   Notice that there are already `PG_GETARG_TEXT_*` macros which can be
   used with `text` type.

4. Modify the C function to add the following code:
   ```
   char			*msg_str = VARDATA_ANY(msg);
   int			msg_size = VARSIZE_ANY_EXHDR(msg);

   elog(WARNING, "message = %s, size = %d", msg_str, msg_size);
   ```

   `VARDATA_ANY` and `VARSIZE_ANY_EXHDR` are utility macros to extract the data
   and size from a varlena. Check `varatt.h` for more such macros.

5. Do not only log the text, but also return the string. Change 
   the `PG_RETURN_VOID` line with:
   ```
   PG_RETURN_CSTRING(msg_str);
   ```

   Similar to `PG_GETARG_*` macros, you can check other variants of
   `PG_RETURN_*` macros. 


6. Rebuild / reinstall the extension (you have to drop / create it, and
   reconnect to load the new `.so`).

7. Try calling the function with the `text` argument.

   ```
   SELECT hacking_function('hello world');
   ```

   And you should get a warning message along with the returned value.

8. However, it likely prints some garbage data after the string. Why?
   Try fixing that.
