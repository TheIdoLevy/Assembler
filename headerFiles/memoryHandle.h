#ifndef MEMORYHANDLE_H
#define MEMORYHANDLE_H

#include <stdio.h>
#include "dataStructs.h"

#define INITIAL_MEM_SIZE 1000

/* 2^21 max words in memory */
#define MAX_MEM_CAPACITY (128*128*128 - 1)

#define WORD_SIZE 24

/* ARE filed bits */
#define ARE_100 "100"
#define ARE_010 "010"
#define ARE_001 "001"

/**
* MemEntry is used to represent an entry in dataMem/instmem. One entry is one word long, or 24 chars long 
* (excluding '\0'). It will hold strings representing binary representations of instructions and memory addresseses
*/
typedef struct 
{
	char data[WORD_SIZE+1];
} MemEntry;


/**
* Mem represents data memory/ instruction memory
* memory: an array of entries, which is the memory array itself
* memCounter: acts as instruction counter/ data counter
* memCapacity: current size of mem
*/
typedef struct
{
	MemEntry *memory;
	int memCounter;
	int memCapacity;
} Mem;


/**
* This function initiates memory. If type signifies data, the data mem is initiallized (memCounter = 0). Otherwise
* inst mem is intiallized (memCounter = 1)
*
* @param type the type of memory to be initiallized (instruction or data)
* @param mallocError used in order to catch dynamic data allocation errors
*
* @return pointer to the new initiallized memory
*/
Mem *initMemory(int type, int *mallocError);


/**
* This function dynamically resizes memory
*
* @param mem memory to bew resized
* @param mallocErr used to detect error in dynamic memory allocation
*/
void resizeMemory(Mem *mem, int *mallocErr);


/**
* This function inserts data to the next free location in memory
*
* @param mem memory into which data will be inserted
* @param data the data inserted into memory
* @param mallocErr used to detect error in dynamic memory allocation
*/
void addMemEntry(Mem *mem, char *data, int *mallocErr);


/**
* This function insets a number to the data memory
* 
* @param mem data memory
* @param the number to be inserted to memory
* @param used by other functions to detect errors in this function
*/
void insertNumToMem(Mem *mem, char *num, int *error);


/**
* This function inserts a symbol from the symbol table to memory
*
* @param mem the memory to be written into
* @param addrMthd the addressing method of curent operand
* @param word the name of the symol being inserted to memory
* @param symbolTable the table containing all symbo definitions
* @param extFile pointer to .ext file
* @param fName name of file being operated on
* @param lineNum the number of the line currently being parsed
* @param error a flag used to detect errors in the assembly of the file
*/
void insertSymbolToMem(Mem *mem, int addrMthd, char *word, Table *symbolTable,
			 FILE * extFile, char* fName, int lineNum, int *error);



/**
* This function inserts data to the instruction memory upon .data or .string defintion in file
*
* @param line the current line in file that is being parsed
* @param i the current index in line that is being read
* @param type can get d for data and s for string. This tells the function how to insert and validate the data
* @param fileName name of file being operated on
* @param lineNum the number of the line currently being parsed
* @param error a flag used to detect errors in the assembly of the file
* @param dataMem pointer to the data memory
*/
void insertData(char *line, int *i, char type, char* fileName, int lineNum, int *error, Mem *dataMem);


/**
* This file handles the freeing of all memory allocated during the assmbly of a file
*
* @param instMem pointer to the instruction memory
* @param data pointer to the data memory
* @param symbolTable pointer to the symbolTable
*/
void freeAllMemory(Mem * instMem, Mem * dataMem, Table * symbolTable);

#endif
