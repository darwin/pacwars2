          ____         ______
         /    \         \   \\
        |               |     |   Scripting engine
         \____  ____ ___|     |
              \/   \\  \\____/      v 0.94a
        /      |_____|___|   \
       |      /\    \   |     |
        \____/  \___/\___/     \
          by Theur - Przemyslaw Podsiadly 1998-99

#include <std_disclaimer.h>

   "I do not accept responsibility for any effects, adverse or otherwise,
    that this code may have on you, your computer, your sanity, your dog,
    and anything else that you can think of. Use it at your own risk."

                     GENERAL RELEASE INFORMATION
    Contents:
1. Introduction
2. General information
  2.1. Documentation info
  2.2. Examples
  2.3. Unsupported features
  2.4. Debugging
3. Compiler specific information (including INSTALATION)
  3.1. GCC (DJGPP & Linux) version
    3.11. The distribution.
    3.12. Installation
    3.13. Miscellaneous
  3.2. Win32 (MS Visual C++) version
    3.21. The distribution.
    3.22. Instalation
    3.23. Miscellaneous
  3.3. Source distribution
    3.31. The distribution.
    3.32. Instalation
    3.33. Miscellaneous
4. SeeRC compiler
  4.1. The console version of SeeRC (GCC-compatible releases)
  4.2. Win32 version : WSeeRC
5. History
6. Conclusion

  //                                                                 \\
 |                     1.  Introduction                                |
  \___________________________________________________________________/

  The idea was to make a script engine for any purpose you would need.
This can be for AI in the game, for macros in an editor or whatever.
This is for things you cannot (or don't want to) have compiled
(hard-coded) inside your program. The idea was to make it easy to
learn, and though it's a C library, SeeR is (or will be) an almost
complete C compiler.

 The existing releases of SeeR (?? - version number,i.e. 08)
 -- Precompiled versions --
 seer??dj  or seer???d  DJGPP 2.x
 seer??lx  or seer???l  LINUX GCC
 seer??vc  or seer???v  MS Visual C++ (tested with version 5)
  Ready to compile in your project PRECOMPILED library. Package contains
 also the SeeRC compiler and some examples.
 -- Source version --
 seer??s    DJGPP 2.x, DJGPP
 The full source version, contains all examples, but ready to
 compile only for DJGPP.

 For instalation notes, see respective version info in "Compiler Specific
 Information" section.

  //                                                                 \\
 |                          2. General information                     |
  \___________________________________________________________________/

=== 2.1. Documentation Info ===
===============================

See seer docs (seer.txt,.inf,.html,.rtf,.txi and so on) and examples on
how to use SeeR.
See also seer.h for additional info.

=== 2.2. Examples ===
=====================

 All packages contain the same examples. Each example is by default a text
console program using standard ANSI I/O functions (like printf).
 When exploring SeeR, look at examples in order given below:

No   sources     description
 1. exhello.c    "Hello world!", importing function `printf'
    exhello.sc

 3. exsimple.c   importing scanf and own function sqrt, loads already
    exsimple.sc  compiled script from file example3.cs (compiled by SeeRC),
                 usage of doubles. Script is just a normal C program...

 4. exinside.c   Source of script written as char* variable (use of
                 Compile_Text(...)), accessing variable with scVar
                 macro, and internal-header inside!

 5. excpp.cpp    Interaction between C++ and a script - accessing variables
    excpp.sc     and function-members of typical C++ class. The script is
                 objective!

 6. exclassc.c   Interaction between C and an objective script. The script
    excpp.sc     is objective and works as if it had to do with C++.
                 It accesses function-members of a class, that is only
                 a C struct and bunch of functions. So C and objective
                 script (the same as in excpp) work together. Usage of
                 dispatchers.

 7. exscript.c   The script type demo. How to easly communicate
    exscript.sc  between scripts (here exscript.sc rules all cows
    exscrcow.sc  and a dog).
    exscrdog.sc

 8. exmulti.c    Multitasking demo introducing fork, priorities
    exmulti.sc   and builtin internal header: Kernel and Instances;
                 (it's very dirty and has to be rewritten)

 9. exstdlib.c   Example of stdlib header: class string. The exstlib.sc
    exstdlib.sc  script is run by makefile at the end:
                  ../bin/seerc -r exstdlib.sc Enjoy!!!
                 Does some variations on string manipulation. Try:
                  exstdlib Either play2me or 4me4everbe4me

 10.exlotr.cpp   Multitasking RPG system example. Shows how an event-driven
    exlotr.sh    system would be implemented. A special imported function
    exlfrodo.sc  'act' is able to interrupt script while waiting for events
    exlsam.sc    and give the control to other scripts or main program.
                 Free interpretation of J.R.R. Tolkien "Lord Of The Rings";)

All Allegro examples have been removed and will be included in separate
package. Allegro is a game library by Shawn Hargreaves & friends.
Visit http://www.talula.demon.co.uk/allegro/

=== 2.4. Unsupported features ===
=================================

Short list of unsupported features:

 -unions and enums ,
 -goto instruction ,
 -bitfields ,

Other limitations:
 -unable to use declare variables with [] (instead use * or specify size
  explicitly)
    so
        main(int argc, char *argv[])
    should be
        main(int argc, char **argv)

See seer.txt to read more on this subject.

=== 2.5. Debugging ===
======================

 When you encounter any errors while running/compiling script, you can
try debug version of SeeR library. This version gives you the ability to
create a file during execution describing what SeeR is doing. It also
contains SeeR-Asm version of the compiled script. To start creating
this file use following functions:
 scOpen_Debug (char *filename);           Opens debug file, turns
                                          debugging ON
 scToggle_Debug (bool deb);               if (deb==true) turns
                                          debugging ON
                                          (scOpen_Debug must have been
                                          called before)
                                          else OFF
 scClose_Debug();                         turns debugging OFF and
                                          closes debug file.

 Example:
 scOpen_Debug("debug.txt");
 s=scCompile_File("script.sc");
 scClose_Debug();

 In non-debug version these functions do nothing, of course. You can check,
if actual SeeR build is a debug one using scSeeR_Info(scInfo_Build).

 You can also use SeeRC compiler with debugging on, which will generate
*.deb file.

  //                                                                 \\
 |                   3. Compiler specific information                  |
  \___________________________________________________________________/

=== 3.1. GCC (DJGPP & Linux & BeOS) version ===
========================================

3.11. The distribution.
  The library is distributed in seer??dj.zip (lib precompiled for DJGPP)
and seer??lx.zip (lib precompiled for Linux gcc).
The archive contains:
lib/libseer.a                   GCC lib  - optimized "release" version
lib/libseerd.a                  GCC lib  - unoptimized "debug" version
examples/makefile               makefile for examples
examples/*                      example files (.c,.cpp and .sc files)
docs/readme.txt                 this file
docs/seer.html,.inf             SeeR reference (in both HTML and INFO format)
seer.h                          main header file
bin/seerc{.exe}                 SeeRC compiler, console version (->4.1)

3.12. Installation
  The lib files are in lib directory. Copy them to your compiler lib
directory. For DJGPP, it is %DJDIR%/lib (where %DJDIR% is a directory
where you have installed DJGPP).
  The include file seer.h should be copied to your compiler include
directory. For DJGPP, it is %DJDIR%/include.
  In UNIX environment (like Linux or BeOS), you should give seerc the
execute right (chmod +x seerc).
  To compile examples, type make. All examples should be created, unless
some external libraries were not found by compiler (->2.2)

3.13. Miscellaneous
  To use SeeR in your programs, you should first include "seer.h", and
link with libseer.a (-lseer option). Make sure seer.h and libseer.a are
in the same directory as you program or in compiler's include and lib
directories.

=== 3.2. Win32 (MS Visual C++) version ===
==========================================
3.21. The distribution.
  The library is distributed in seer??vc.zip (lib precompiled for
MS Visual C++).
The archive contains:
examples/*                      example files (.c,.cpp and .sc files)
lib/seer.dll                    optimized "release" version
lib/seer.lib                    interface to seer.dll
lib/seerd.dll                   debug version (required by WSeeRC)
lib/seerd.lib                   interface to seerd.dll
docs/readme.txt                 this file
docs/seer.html,.rtf             SeeR reference (in both HTML and RTF format)
seer.h                          main header file
bin/WSeeRC.exe                  WSeeRC compiler, the dialog Win32 version (->4.2)
bin/WSeeRC.dll                  WSeeRC compiler dll
VC/ExHello.dsp ...              project files for examples
VC/SeeRExmp.dsw                 workspace for examples

3.12. Instalation
  The lib files are in lib directory. Copy them to your compiler lib
directory, e.g.  \devstudio\vc\lib .
  The include file seer.h should be copied to your compiler include
directory, e.g.  \devstudio\vc\include .
  To compile examples, load SeeRExmp.dsw and compile each project separately.
  You may also copy all provided dlls to windows\system directory.

3.13. Miscellaneous
  To use SeeR in your Win32 programs, you should first include "seer.h", and
link with seer.lib. Make sure seer.h and seer.lib are in the same directory
as you program or in compiler's include and library directories. To run such
a program you will need seer.dll in your programs directory or in
windows\system.

=== 3.3. Source distribution ===
================================
3.31. The distribution.
  The library is distributed in seer??sr.zip or seer???s.zip.
The archive contains:
examples/*                      example files (.c,.cpp and .sc files)
lib/                            library directory
objs/                           directory for object files
docs/readme.txt                 this file
docs/seer.html,.rtf             SeeR reference (in both HTML and RTF format)
seer.h                          main header file
bin/                            binary SeeRC compiler directory
seerc/                          SeeRC sources for console and windows
src/                            sources of SeeR inside
VC/ExHello.dsp ...              project files for examples for VisualC
VC/SeeR.dsw                     workspace for SeeR,test and examples
test.c                          testing program
test.sc                         testing script
seer.h                          main header file
makefile.*                      makefiles
fixunix                         shell script for making package ready for
                                compilation in Unix environment (requires
                                fromdos and todos utils)

3.12. Instalation

 *For Unix-like (Linux, BeOS)
  First prepare the files:
   chmod +x fixunix
   ./fixunix .
  Then compile:
   make -f makefile.lx

 *For DJGPP:
  Just compile:
   make -f makefile.dj

 *For Visual C++:
  Load SeeR.dsw workspace. Then compile SeeR project in every mode
  (debug, release, wseerc). Compile WSeeRC in release mode.
  Then you can try test and examples.

3.13. Miscellaneous
  For additional information concerning further use see other distributions
information.

  //                                                                 \\
 |                        4. SeeRC compiler                            |
  \___________________________________________________________________/

 SeeRC is a program that compiles script sources (*.sc) to scripts (*.cs).

All versions of SeeRC declare internal header called stdio, which contains:
  import int printf(char *,...);
  import int sprintf(char*,char *,...);
  import int scanf(char *,...);
  import int getchar();
  import int scrandom(int);
  import int rand();
  import int srand(int seed);
  import int strcpy(char *,char *);
  import int strcmp(char *,char *);
  import int strtol(char *,char **,int);
  import int atoi(char *);
  import double atof(char *);
  import unsigned int strtoul(char *,char **,int);
  import double strtod(char *,char **);
  import char *strdup(char *);
  import double sqrt(double);
  import double sin(double);
  import double cos(double);
  import void* malloc(int);
  import void free(void *);

It also exports standard SeeR stdlib header, together with Kernel
and Instance.

4.1. The console version of SeeRC (GCC-compatible releases)

Syntax:

  seerc {Input files} {Options}
  seerc {Options} -r {Input file} {parameters-for-function-main}

Filenames can be a list of files or file masks.

Options:
 -a {author} assign an author to the script
 -d          compilation to produce also *.deb files that contain SeeR-Asm
             code of processed script.
 -h          help
 -i          see the script infos (SeeR version,interface objects),
 -o {output} output filename specification (other then *.cs)
 -r, -run {other-options} filename.sc||.cs arguments-for-main(argc,argv)
 -s          display compilation message on the screen
 -t {tile}   assign a title to the script
 -v          displays SeeR version
 -O special option:
  -Oe        generate EXPR opcode for multitasking scripts
  -Os:num    set stack size to num bytes

The '-r' option allows you to run the script. The 'main' function is called
like in normal C program. You should remember, that you can call only
functions exported by SeeRC! Example:
 seerc -r script.sc parameter1 parameter2
  or pre-compiled script:
 seerc -r script.cs parameter1 parameter2
In UNIX environment you can make scripts also by adding the first line:
 #!/bin/seerc -r
Now, if script has eXecute attribute, you can run it, and as shell seerc
should be executed, that will compile the script and run it (here seerc
is in /bin/ directory). So now SeeRC can be used the same way as many
script interpreters, like shell or perl (see example exstdlib)

4.2. Win32 version : WSeeRC
 This is a dialog based application. It seems to be self-explanatory.
It has although some limitiations, like is unable to run a script.

  //                                                                 \\
 |                      5. History                                     |
  \___________________________________________________________________/
1999.09.23 (SeeR 0.94a) -
         even more objective - constructor/destructor/operator handling,
         functions returning struct,
         function differencing (functions with same name but different
         parameters are different, like in C++),
         static variables in functions,
         reference type (e.g. int &),
         SeeRC is able to execute scripts (-r option),
         default initializers implemented ( e.g. int c(int a,int b=1) ),
         bit-negation operator ~,
         unsigned implemented,
         can use both :: and . in member function declaration,
         struct declaration nesting,
         script type for easy accessing variables and functions of other script,
         const modifier implemented,
         typeof operator implemented,
         StdLib internal header (string and vector classes - now you can
         easly operate on them),
         in compiled script only actually used import symbols are
         stored (not all, like in earlier versions),
         big code optimizations,
         EXPR opcode separates expressions/statements,
         2 new examples: exstdlib and exlotr
         and many more...
         fixes:
         many (too many to list them here)
1999.07.12 (SeeR 0.93a) -
         SeeR is now objective - classes implemented!,
         new or rewritten examples,
         new feature - dispatchers,
         common source directory structure for VC and DJGPP,
         new almost portable version of callasm
         fixes:
         bug with a##b in macros rendered as "a b",
         bugs with error signaling (some error statements were
         interpreted in a wrong way),
         forgot to include stdarg.h in seer.h,
         writing to imported char variables improved

1999.03.07 (SeeR 0.92a) -
         C++ interface in seer.h,
         fixes:
         about 10x faster compiling !,
         more initialization bugs removed,
         array size wasn't allowed to be constant expression other than single
           integer (so int x[4*12] was not allowed),
         initializating pointers with tables,
         fork {} (without parameter) didn't work sometimes (internal compiler error),
         long decided to be 64bit constant, yet unsupported, use int instead of
           32-bit long),
         some operations on fixed didn't work,
         unary operator * didn't work (!),
         duplicate identifier checking,
         bug in operators || and &&, when used with imported or complex vars.

1999.02.18 (SeeR 0.91a) -
         fixes:
         initialization of strings in structures or arrays,
         scGet_Instance_Status was not exported in DLL,
         small bug in macros with strings containing '\',
         better test program,
         arrays [x][y] were thought to be [y][x]
         in macros #c didn't work, if c contained '"'
         DJGPP and VC (and possibly other C compilers) don't pack the struct
          - all not char members has to be aligned, and sizeof(struct) must sometimes
           be multiplies of 4. SeeR always packed the structures.

1999.01.01 (SeeR 0.9a) -
         the ultimate compilation cleanup - rewrite all code connected with
         variable type handling (it's almost all compilation functions),
         casting implemented ( like (int*)a ),
         initialization of structures and tables finally implemented.
         fixes:
         ?: operator bug when using with imported variables,
         undefined identifier used as parameter to function reported
         as "Too many parameters",
         imported variables of typedef'd type were losing its' import flag,
         multiple-lines macros didn't work,
         switch(fun(...)) {...} didn't work OK,
         return didn't work for functions returning void,
         division by zero detected,
         fork sometimes copied the wrong stack to the child,
         the child process was not freed when finished,
         and many others... (uff)

1998.08.26 (SeeR 0.8a) -
         Win32 version (MS Visual C++ 5), changes in priorities,
         initializing internal Kernel header optional (scExport_Kernel),
         operations on schedulers
         fixes:
         many major bugs connected with memory handling (while I was porting
         SeeR to Win32, thanks to MSVC debugger I traced many nasty bugs),
         still remains a problem with MSVC's ASSERTION warnings in debug
         version (when freeing script after scCompile_Script -
         anybody can help?)

1998.07.26 (SeeR 0.7a) -
         fork feature (so one instance can fork into sub-instances
         running parallelly),
         priorities feature (so some scripts "kernels" can operate on
         advanced internal SeeR functions, while "user" scripts are
         not allowed to e.g kill other instances or use some user-defined
         functions),
         preprocessor keywords #title and #author for easier handling
         runtime errors of instances (error message contains title
         of instance where error occured),
         all new allegro example6 based: Allegro's ex24 runs as a script
         with almost no change int it.
         fixes:
         example3:scanf("%lf",..) instead of scanf("%G",..) makes it work
                  in Linux and some versions of DJGPP,
         tables passed as pointers,
         pointer arythmethic (pointer+value),
         crashed when freeing memory allocated above 0x7fffffff,
         calculating size of data segment,
         calling imported functions returning double (in optimized version only),
         error "function declared and not defined" wasn't reported before,
         preprocessor in #ifs redefined all defines

1998.07.10 (SeeR 0.62a) -
         major operator++ and operator-- bug fixed, parameter parsing
         bug in SeeRC compiler

1998.06.27 (SeeR 0.61a) -
         the last release before my summer holidays, some bugfixes
         (mainly in error reporting), implementations in documentation

1998.06.12 (SeeR 0.6a) -
         able to call function before definition (but after declaration),
         new example of accessing C++ class variables and member functions
         fixes:
         -major fix in preprocessor on macros,
         -report some syntax errors
         -accessing script's variables (macro scVar) fixed
         -imported functions returning fixed

1998.06.09 (SeeR 0.5a) -
         floating types implemented with many major changes in code,
         doesn't exit on compilation error: just reports it to the caller
         fixes:
         -accessing variables of size different from 4(ie char)

1998.05.24 (SeeR 0.41a) -
         assembler interface `__vcpu(...)',
         multitasking keyword `secured',
         WAIT macro (__vcpu(43))-forces SeeR to interrupt script
         (in multitasking environment),
         NULL defined,
         arrays defined in script work,
         new multitasking Allegro demo,
         fixes:
         -functions returning sth different from int didn't work as params
          to functions!
         -final (hopefully) parser fix with last line duplication
         -"include after include" and line-numbering bug fix
         -sometimes crashed, when some structures allocated above
          0x7fffffff-fixed

1998.05.12 (SeeR 0.4a) -
         include added : able to include file or internal header (as char*)
         multitasking
         fixes:
         -using result of imported function as parameter to function
          (previously caused Internal compiler error)
         -some optimizations and fixes in code generation
         -parser loading when last line not empty

1998.04.29 (SeeR 0.3a) -
         structs works (NOT FULLY TESTED YET), changed way to access
         imported variables (so it's able to read it's address),
         Allegro example (example6 - check it out if you use this
         great library!)
         major fixes:
         -parser reading comments,
         -calling imported functions without parameters

1998.04.26 -
         fixed some errors (e.g. a/b != b/a), SeeRC compiler added,
         switch/case added, Compile_Text now complete, more examples.

1998.04.21 (SeeR 0.2a) -
         able to use imported variables, able to use infinite
         parameters (...), debug version and more examples added.

1998.04.15 -
         some bugfixes

1998.04.09 (SeeR 0.1a) -
         first public alpha release

1998.02.16 -SeeR 0.0 was born...

  //                                                                 \\
 |                      6. Conclusion                                  |
  \___________________________________________________________________/

That's all for now. Enjoy !!!
  This software is free , but if you use it
        -= PUT MY NAME (and the lib's ) IN THE CREDITS!! =-
and let me know about your program...
I'd be happy to see what you've used it for...

I need your feedback - mail to
------->          ppodsiad@elka.pw.edu.pl
Do you have any suggestions on how it should work? Don't hesitate and write!

                Enjoy!
                        Theur - Przemyslaw Podsiadly

By Przemyslaw Podsiadly (just Przemek)
12 Modrzewiowa Street,
08-110 Siedlce, Poland.

ppodsiad@elka.pw.edu.pl

page:      http://home.elka.pw.edu.pl/~ppodsiad/
SeeR page: http://home.elka.pw.edu.pl/~ppodsiad/seer/
