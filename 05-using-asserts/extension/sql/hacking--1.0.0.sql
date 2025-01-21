/* intro to hacking extension */

CREATE OR REPLACE FUNCTION hacking_function()
    RETURNS void
    AS 'hacking', 'hacking_function'
    LANGUAGE C IMMUTABLE;
