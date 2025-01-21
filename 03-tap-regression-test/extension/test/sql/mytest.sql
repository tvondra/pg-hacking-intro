\set ECHO none

-- disable the notices for the create script (shell types etc.)
SET client_min_messages = 'WARNING';
\i sql/hacking--1.0.0.sql

\set ECHO all

SELECT hacking_function();

SELECT 1;
