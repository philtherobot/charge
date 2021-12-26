# Stirrup Design

## Goals

Let's first posit what are the various goals that Stirrup tries to meet.

### Usage scenarios

Stirrup is primarily intended to support the creation of small programs.  Those small programs could be:
- Programs that would best be implemented with a general purpose programming language because there is a need for variables, functions and classes.  These things cannot be easily done in Bash or Windows Batch.  Maybe because it genuinely is difficult or you do not know how to write the program in Python or a shell script language but you do know how to write it in C++. 
- Programs that require lots of interaction with executables. Piping executables between themselves and with files, capturing the output, feeding input, etc.
- Programs that manipulate the filesystem.  Copy, delete and list files for example.

### Target audience

Stirrup should be usable by programmers with a little bit of C++ experience, by expert C++ programmers and everybody in between.  Because Stirrup is not the standard library, Stirrup may not be well suited to people being introduced to C++: the student would be learning a niche library instead of C++ itself.

### Portability

Stirrup is multi-platform.  We want to avoid the issues related to difference in the toolchains and platforms from one platform to another: the code written should be the same and have the same results regardless of the platform.

We should note that the portability goals of Stirrup reach all the way to the end-of-line style: Stirrup should take away the EOL issues as much as possible.

### Internationalization

This is the 21th century, at the time of writing, we are near the year 2021. People all over the world are using computers.  It is time we take text encoding seriously in C++.  It is thus a goal of Stirrup to bring proper text processing.  At a minimum, Stirrup must let the programmer:
- Operate with any the file names allowed by the operating system.
- Input and output text is encoded in UTF-8.

Maybe, as a stretch goal, input and output text is encoded according to the user's settings.  

## Resulting design

We can start deriving broad design decisions given the goals set for Stirrup.

### Programming style

Because Stirrup targets small, quickly written, maybe one-time-use programs, its resulting code should be terse and the functions and tools at your disposal should be powerful.

Because Stirrup support multi-platform programming, Stirrup should let you write portable code by default.  Platform-specific details can be addressed with advanced tools in Stirrup.  Stirrup programs are portable at the expense of making assumptions and making some things harder. For example, standard input/output is text, not binary, by default.

Because Stirrup aims to support quickly written programs that are also correct, the code should be terse and to the point.  Every part of Stirrup should easily be composable.  Functions are "return by value".

Because Stirrup wants to enable correct and easy to troubleshoot programs, errors result in thrown exceptions.  Most programs targeted for being written in Stirrup are only concerned with the happy path.  An error usually means complete failure of the program.  Errors cannot be ignored and will always be reported.  Using exceptions also result in shorter, more to the point, code.   

Stirrup steers away from the usual "performance first" approach found in C++.  Performance is not a goal of Stirrup.  The consequence is that Stirrup is not suited for tiny CPUs.  In fact, Stirrup is only concerned with desktop-class and up computers.  It is just happenstance if Stirrup works on small hardware.

In summary:
- Portable by default.
- Return by value.
- Exception error handling.   
- Performance, in consumed memory and execution time, is secondary.
- Desktop-class and up operating systems.

### String type

// todo-php: explain why Stirrup uses u32string
because we want type-safe programming, at least around strings, to help programmers not mess up text encodings, we should separate char*.  there is no way of knowing what encoding is a char*.  either we use a new string type that embed the encoding format or we use a u32string. 

// todo-php: what's Stirrup's stance on multi-threading? 
// todo-php: type safety, what's the stance? 
// todo-php: why and is it OK that Stirrup still uses so much of the C++ std lib?

## Streams

Portable text files input and output is made difficult by differences in end-of-line style among platforms.  

Stirrup wants to support binary file input/output.  This can be achieved by reading and writing plain `char*` types.  On the POSIX platforms, this works well.  Binary input/output under Windows requires that the file handle be set in the "binary" to avoid getting the `LF` character converted to a `CR` + `LF` pair.  This conversion will mess up the binary the program is trying to write.

Stirrup makes the following decision: by default, all streams are text.  This matches the Windows C++ runtime-time decision and supports compatibility across platforms.  A program written in Stirrup with its streams API is portable and adapts to the platform it is running on.  Text files on Windows uses `CRLF` and `LF` on POSIX.

Binary stream input/output is possible through another API. 
