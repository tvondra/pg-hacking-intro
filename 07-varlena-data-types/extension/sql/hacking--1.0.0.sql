/* intro to hacking extension */

CREATE OR REPLACE FUNCTION hacking_function()
    RETURNS void
    AS 'hacking', 'hacking_function'
    LANGUAGE C IMMUTABLE;

CREATE OR REPLACE FUNCTION hacking_function(int)
    RETURNS void
    AS 'hacking', 'hacking_function_2'
    LANGUAGE C IMMUTABLE;

CREATE OR REPLACE FUNCTION hacking_function(text)
    RETURNS cstring
    AS 'hacking', 'hacking_function_3'
    LANGUAGE C IMMUTABLE;
