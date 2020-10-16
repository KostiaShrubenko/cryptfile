

ENV = C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64
CC = gcc
EXECUTABLE = cryptfile

CFLAGS = -std=c99
EXTRALIBS = -L"$(ENV)\lib" -ltomcrypt -ltommath
EXTRAINCLUDES = -I"$(ENV)\include"
LOCALINCLUDES = -I".\cypher\inc" -I".\interface\inc" -I".\header\inc" -I.

OBJECTS = cryptfile.o cypher.o interface.o header.o

HEADERS = cryptfile.h config.h cypher/inc/cypher.h interface/inc/interface.h interface/inc/interface_datatypes.h header/inc/header.h

$(OBJECTS): $(HEADERS)

install: $(OBJECTS)
	cmd /c if not exist "..\exe" mkdir "..\exe"
	cmd /c if not exist "..\exe\outfiles" mkdir "..\exe\outfiles"
	copy /Y "Help.txt" "..\exe" 
	$(CC) $(CFLAGS) -o ../exe/$(EXECUTABLE) $(OBJECTS) $(EXTRALIBS)

cryptfile.o: cryptfile.c
	$(CC) $(CFLAGS) $(LOCALINCLUDES) $(EXTRAINCLUDES) -c cryptfile.c 

cypher.o: cypher/src/cypher.c
	$(CC) $(CFLAGS) $(LOCALINCLUDES) $(EXTRAINCLUDES) -c cypher/src/cypher.c

interface.o: interface/src/interface.c
	$(CC) $(CFLAGS) $(LOCALINCLUDES) -c interface/src/interface.c

header.o: header/src/header.c
	$(CC) $(CFLAGS) $(LOCALINCLUDES) -c header/src/header.c
