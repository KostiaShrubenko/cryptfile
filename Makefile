
CC = gcc
EXECUTABLE = cryptfile

CFLAGS = -std=c99

EXTRAMAKE = makefile.mingw

EXTRALIBS = -L".\extramodules\libtomcrypt" -L".\extramodules\libtommath" -ltomcrypt -ltommath
EXTRAINCLUDES = -I".\extramodules\libtomcrypt\src\headers"
LOCALINCLUDES = -I".\src\cypher\inc" -I".\src\interface\inc" -I".\src\header\inc" -I".\src"

INC = $(EXTRAINCLUDES) $(LOCALINCLUDES)

OBJECTS = src/cryptfile.o src/cypher/src/cypher.o src/interface/src/interface.o src/header/src/header.o

HEADERS = src/cryptfile.h src/config.h src/cypher/inc/cypher.h src/interface/inc/interface.h src/interface/inc/interface_datatypes.h src/header/inc/header.h

$(OBJECTS): $(HEADERS)


install: $(OBJECTS)
	cd .\extramodules\libtommath\ && gmake -f $(EXTRAMAKE) libtommath.a CC=$(CC)
	cd .\extramodules\libtomcrypt\ && gmake -f $(EXTRAMAKE) libtomcrypt.a CC=$(CC)
	cmd /c if not exist ".\exe" mkdir ".\exe"
	cmd /c if not exist ".\exe\outfiles" mkdir ".\exe\outfiles"
	copy /Y ".\src\Help.txt" ".\exe" 
	$(CC) $(CFLAGS) -o exe/$(EXECUTABLE) $(OBJECTS) $(EXTRALIBS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

.PHONY: clean
clean:
	cd .\extramodules\libtommath\ && gmake -f $(EXTRAMAKE) clean CC=$(CC)
	cd .\extramodules\libtomcrypt\ && gmake -f $(EXTRAMAKE) clean CC=$(CC)
	cd .\src && cmd /c del /Q /S *.o
