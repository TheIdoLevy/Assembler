
assembler: assembler.o fileOperations.o preAssembler.o dataStructs.o utils.o memoryHandle.o firstScan.o secondScan.o errors.o validators.o 
	gcc -ansi -Wall -pedantic assembler.o fileOperations.o preAssembler.o dataStructs.o validators.o utils.o memoryHandle.o firstScan.o secondScan.o errors.o -o assembler

assembler.o: assembler.c fileOperations.h preAssembler.h firstScan.h
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o

fileOperations.o: fileOperations.c fileOperations.h errors.h
	gcc -c -ansi -Wall -pedantic fileOperations.c -o fileOperations.o

preAssembler.o: preAssembler.c preAssembler.h dataStructs.h fileOperations.h utils.h validators.h
	gcc -c -ansi -Wall -pedantic preAssembler.c -o preAssembler.o

dataStructs.o: dataStructs.c dataStructs.h
	gcc -c -ansi -Wall -pedantic dataStructs.c -o dataStructs.o

memoryHandle.o: memoryHandle.c memoryHandle.h utils.h dataStructs.h errors.h validators.h
	gcc -c -ansi -Wall -pedantic memoryHandle.c -o memoryHandle.o

utils.o: utils.c utils.h
	gcc -c -ansi -Wall -pedantic utils.c -o utils.o

validators.o: validators.c dataStructs.h firstScan.h errors.h
	gcc -c -ansi -Wall -pedantic validators.c -o validators.o

firstScan.o: firstScan.c dataStructs.h validators.h firstScan.h errors.h  fileOperations.h utils.h secondScan.h preAssembler.h memoryHandle.h
	gcc -c -ansi -Wall -pedantic firstScan.c -o firstScan.o

secondScan.o: secondScan.c dataStructs.h fileOperations.h utils.h validators.h firstScan.h memoryHandle.h
	gcc -c -ansi -Wall -pedantic secondScan.c -o secondScan.o

errors.o: errors.c errors.h
	gcc -c -ansi -Wall -pedantic errors.c -o errors.o

clean:
	rm -rf *.o *.am *.ob *.ent *.ext
