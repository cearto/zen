GAPS Users -

This directory contains all code for the GAPS software library.
There are several subdirectories:

    docs - HTML documentation files.
    pkgs - source and include files for all packages (software libraries).
    lib - archive library (.lib) files.
    makefiles - make files. 

If you are using linux or cygwin and have gcc and OpenGL installed, 
you should be able to compile the code by typing "make clean; make"
in this directory.  The scripts file has the basic makefiles which 
could be edited for other operating systems and compilers.

We also provide Visual Studio 2005 solution files for make .lib files in
Window. The files are in directory "pkgs".

To write a program that simply uses GAPS, then you should include
"-I XXX/gaps/pkgs" in your compile flags (CFLAGS) and "-L XXX/gaps/lib" 
in your link flags (LDFLAGS), where XXX is the directory where you 
installed the gaps software.  

The software is distributed under the MIT license (see LICENSE.txt)
and thus can be used for any purpose without warranty of any kind.

- Tom Funkhouser



