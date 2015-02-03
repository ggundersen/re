re
=
re is a regular expression engine written in C.

Supported operators
- `.` for concatenation
- `*` for Kleene star (zero or more)
- `|` for alternation

Postfix expressions
- Currently, re only compiles postfix expressions

Example expressions
- `ab.` matches `ab`
- `ab|` matches either `a` or `b`
- `ab*|` matches either `a` or zero or many `b`s

Usage
- `./re [regular expression] [input]`
- To disambiguate the expression from the Unix pipe or `|`, use quotation marks when appropriate.
- e.g. `./re 'ab|' a
