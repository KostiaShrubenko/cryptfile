Today (20.10) I noticed that I have been using the AES algorithm totally in the wrong way. 
I thought it accepts a data of any size. But actually it has to be a block with 16(bytes). 
So I redisigned encryption and decryption.

To compile my project just open command line and type: "gmake install"

NOTE: the default compiler set as "gcc". If you dont have it in your PATH variables,
you have to open "Makefile" and assign to "CC" path to your compiler. The same with make application,
the default is "gmake", you can change it in variable "MAKE" it is needed to compile external modules.


To test program you can use image.jpg

