# setql
Query data in csv files using list-comprehension style syntax

## Description
Use list comprehesion like syntax influenced similar to that found in Python to query records in a 'database' which at the moment is simple CSV files.

## Goals/ Design Notes

* Interactive mode that prints out operations before comiting them to files
* At least one alternative output form for tables (json|pdf|html)
* Summary of data that can be printed at any time by the user. ie categories and optionally predicate that members of each 
  group fulfill
* Support for set-wise operations
* Pick items from source set in a random order when element constraints are not specified by default similar to `arb`
  in [SETL](https://en.wikipedia.org/wiki/SETL) 
* Automatically generate structure that holds results that do not meet a certain constraint or are left over when 
  generating the user specified set making it available for later operations
* Try and coerce string and int types like in AWK when an operator that only supports numeric types is used(eg < or in 
  arithmetic operators)
* Instead of including builtins sets for numbers provide way to easily specify sets with only numbers in them and 
  their relationships as in Haskell list comprehensions. Implement membership as generator for values 
* Array based and dot notation for accessing column fields similar to Lua tables.

## Progress

* Implemented most of the the set data structure which uses an AVL tree for efficiently storing records. Current implementation
only works with `int` but is to be replaced with a `Union`.
* Grammar of language loosely defined in lang.bnf and began description in .y file for use with GNU bison.
* Still need to:
  - Implement set operations (union, intersection, difference and complement)
  - Convert use of `int` type to `Union`
  - Implement hash table to enable storage of user and environment variables for interpreter.
  - Create thin wrapper around parser to enable reading commands from file and interactive session.
  - Add code that parses csv files (in progress)

