
East const!  A controversial decision.  The rationale is that the most important thing is the type, not whether it is const or not, so type comes first.  East const also places `const` in the same place the language places `*` and `&`.

Lower case identifiers with underscores to separate words.

Study and respect the formatting already in place.  For CLion users, there is an exported code style scheme at `CLion Code Style.xml` that can be imported for the project.  
 
No `using` statements in headers.  `using` is encouraged in implementation files to lighten up the syntax.

Boost Test.  Test suite names are prefixed with "suite_".  Test case names are prefixed with "case_".  Rationale: Boost Test creates namespaces with the test suite and case name which causes name clashes if the name is the same as the identifiers under test.

Headers should be listed in CMakeLists.txt so that IDEs can include the headers in their source file index.

Only system headers are included with double quotes.  System headers are:
- standard library headers
- dependencies found outside of this project tree

Member variables end with an underscore.

A line cannot be longer than 120 characters.
