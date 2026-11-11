Aigle D'Or remake (C) 2014 JOTD/Hialmar

Original code (C) 1984 by Loriciels

This is the developer package with full source + tools
You need to unpack it where the user package is to get all data files & DLLs

This is one of the first applications of my bas2c.py tool, a python tool to convert
BASIC programs to C. This one was rather tricky because of the original bas2c.py bugs, and the
need to create a bin2c.py tool to convert binary to ASM/bin.

(there's still an unfixed bug in bas2c.py when evaluating boolean expressions in other expressions since
C returns 1 when True, whereas BASIC returns -1: I had to change those few expressions manually for this time)

To compile the game you need the following:

- SDL 1.2 distribution: https://www.libsdl.org/download-1.2.php
- A gcc compiler
- GPRBuild: http://libre.adacore.com/download/configurations (get GNAT GPL 2014: gcc & gprbuild are included)

Set SDL_HOME environment variable where SDL is installed

If you want to open the .gpr project, you'll need Gnat Programming Studio available here: http://libre.adacore.com/tools/gps/
