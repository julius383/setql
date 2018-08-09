* Interactive mode that prints out operations before comiting them to files
* Include limited command line mode for very large files [optional]
* Auto detection of categories when user picks a field
* Deduplicate before commiting to file
* Inform user when more that 10 files are going to be written
* At least one alternative output form (json/pdf)
* Summary of data that can be printed at any time by the user. ie categories and
  optionally predicate that members of each group fulfill
* Support for set-wise operations
* Advanced propositional parser for specifying members of each group eg 
  {x \in Boys, y \in Girls: |X| = |Boys| / 2, |Y| =  |Girls| / 2}
* Constraints are can be applied both to members and the set to produce a final
  result
* Pick items from source set in a random order when element constraints are not
  specified by default
* Random shuffle implemented as an example of one of the many functions
* Test cases to demonstrate functionalities eg classfying students based on
  grades
* Automatically generate structure that holds results that do not meet a certain
  constraint or are left over when generating the user specified set
* Default group of supported operators for certain types eg <,> etc for numbers
  and string< string> or even limited regex support eg numbers 11...6 
* Ask user for unique field in record if multiple fields are present or
  optionally try and guess
* When evaluating an expression scan and replace variables with set members, if
  domain entry in environment map is empty raise an error
* Try and convert field to int type when an operator that only supports numeric
. types is used(eg < or in arithmetic operators)
* Use flat array for multidimensional csv files and put records in array and
. access them accordingly without user knowing
* Rewind for a section of code
* Instead of including builtins sets for numbers provide way to easily specify
  sets with only numbers in them and their relationships (Haskell list
  comprehensions). Implement membership as generator for values and allow for
  custom definition by user.
* Drop-in replacement to sql, start by compiling to sql expressions then later
  hooking directly into database engine, sqlite is a good candidate.
* Initial csv backend for data, with parser also written with bison/flex
* Array based and dot notation for accessing column fields
* 3 major data types:
    * Numbers
    * Strings
    * Set
    * Bool

