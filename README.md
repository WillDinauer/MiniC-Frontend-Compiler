README.md for the minic project
William Dinauer, Dartmouth CS57, Spring 2022

Note: if/else/while statements are assumed to always be followed by a block ('{' stmt* '}') as necessary for the C++ AST file. This removes the need for handling the 'dangling else' problem.

Syntactic analysis for correct typing in function calls is not checked for here.

The program will find and print every syntactical error (variables used that were never defined; multiple declarations of a single variable), so long as there are no syntax errors according to the grammar
NOTE: the program prints theses syntactical errors and THEN also prints the AST. 

Instead of finding every error, the parser stops once it finds a single syntax error.

To create the minic executable, call 'make' (testing is done by calling './minic < [testfile]')

To clean the repository, call 'make clean'