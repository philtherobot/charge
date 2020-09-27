# Stirrup scope

How much scope do I want to put into Stirrup?  I always dreamt of a library that was easier to use.  Certain things such as shell process interaction is needed by Charge and the programs I would like to write with Charge, so placing that service ins library is a really great idea.

Given I want a library of useful things, how much work would I be embarking on?  What are rules or the general idea of how scope for Stirrup is decided?  Do I go full new API and reinvent everything or just patch the standard library where it fails. or somewhere is between?

## Evaluating what is the full scope

Let's try to get a good idea of how much needs coding for a fully new idiosyncrasy-free API.  The target audience is Charge itself and scripts to automate tasks on a computer.

### What needs to be coded

- Shell program launching including I/O with said program
- Top-level exception trap (replace `main`)
- Errors are exceptions by default instead of by choice 
- Get hostname
- Get home directory
- Program arguments processing
- Formatting to string

### What we can reuse from the standard library

- Algorithms
- std::regex
- Containers
- Optional
- Tuples
