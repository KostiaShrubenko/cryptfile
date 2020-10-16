To compile my project two libraries has to be installed first as static ones.
They are "libtommath" and "libtomcrypt".

First you install "libtommath":
- You can find it here: https://github.com/libtom/libtommath/releases/tag/v1.2.0
- Install "Source code"
- Open "makefile.mingw" and assign to variable "PREFIX" your path to mingw environment.
- Also, if needed change compiler to your in variable "CC"
- Now open terminal and type "gmake -f makefile.mingw install".

Second you install "libtomcrypt":
- You can find it here: https://github.com/libtom/libtomcrypt/releases/tag/v1.18.2
- Install "Source code"
- Open "makefile.mingw" and assign to variable "PREFIX" your path to mingw environment.
- Also, if needed change compiler to your in variable "CC"
- Now open terminal and type "gmake -f makefile.mingw install".

Third and last step, installation of program.
- open "src\Makefile" and change variable "ENV" to your path to mingw environment.
- Also, if needed change compiler to yours in variable "CC"
- In terminal type "gmake -f Makefile install"


To test program you can use image.jpg

P.S. I dont know why but app works perfectly when it is called from shell in VS Code.
When it is called from windows command prompt it work when destination is specified. 
But if you omit "-f" (destination option) it doesnt execute any routine.
