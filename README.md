Currently, my application is only compatible with unix operational systems. So you will be able to
compile it for Linux and launch it. But if your try to launch it on windows it doesn't work as intended.
The thing is that unix systems uses character '/' as folder separator but the Windows uses '\'. So the app
launched on the Windows wouldn't be able to parce pathes properly.

Today (20.10) I noticed that I have been using the AES algorithm totally in the wrong way. 
I thought it accepts a data of any size. But actually it has to be a block with 16(bytes). 
So I redisigned encryption and decryption.

To compile my project just open command line and type: "make -f Makefile.unix install"

NOTE: the default compiler set as "gcc". If you dont have it in your PATH variables,
you have to open "Makefile" and assign to "CC" path to your compiler. The same with make application,
the default is "gmake", you can change it in variable "MAKE" it is needed to compile external modules.


To test program you can use image.jpg

