/*
This file handles the basic operations on the data and instruction memory of the program
This includes initializing memory, resizing memory, inserting to memory, freeing memory, and inserting a symbol
from the symbol table to memory
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "dataStructs.h"
#include "memoryHandle.h"
#include "errors.h"
#include "validators.h"

Mem *initMemory(int type, int *mallocError)
{
	Mem *mem = (Mem *)malloc(sizeof(Mem));
	
	if(!mem)
	{
		*mallocError = 1;
		return NULL;
	}
	
	mem->memory = malloc(INITIAL_MEM_SIZE*sizeof(MemEntry));
	
	if(!mem->memory)
	{
		free(mem);
		*mallocError = 1;
		return NULL;
	}
	
	/* need to decide between data and inst */
	/* 0 for data mem, 1 for inst mem */
	mem->memCounter = (type == 0) ? 0 : 100;
	mem->memCapacity = INIT_MEM_SIZE;
	return mem;
}

void resizeMemory(Mem *mem, int *mallocErr)
{
	int newCapacity = mem->memCapacity*100;
	MemEntry *newMem = realloc(mem->memory, newCapacity * sizeof(MemEntry));
		
	if(!newMem)
	{
		/* When failed to allocate memory, must exit the program */
		printInternalError(ERR_CODE_7);
		exit(1);
	}
	
	/* Max mem size exceeded */
	if(newCapacity > MAX_MEM_CAPACITY)
	{
		*mallocErr = 1;
		printInternalError(ERR_CODE_11);
	}
	
	mem->memory = newMem;
	mem->memCapacity = newCapacity;
}

void addMemEntry(Mem *mem, char *data, int *mallocErr)
{
	char dataCopy[WORD_SIZE+1];
	if(mem->memCounter < mem->memCapacity)
	{
		memcpy(dataCopy, data, WORD_SIZE+1);
		strncpy(mem->memory[mem->memCounter].data, dataCopy, WORD_SIZE+1);
		mem->memory[mem->memCounter].data[WORD_SIZE] = '\0';
	} else 
		resizeMemory(mem, mallocErr);
	
	mem->memCounter++;
}


void insertNumToMem(Mem *mem, char *num, int *error)
{
		char *bin;
		bin = malloc(WORD_SIZE+1);
		if(!bin)
		{
			printInternalError(ERR_CODE_7);
			exit(1);
		}
		strcpy(bin, decToBin(atoi(num+1), WORD_SIZE-3));
		strcat(bin, ARE_100);
		addMemEntry(mem, bin, error);
		free(bin);
}

/* Read next word, add word length to counter, if exceeds 81, report error */
void insertData(char *line, int *i, char type, char* fileName, int lineNum, int *error, Mem *dataMem) 
{
	char data[MAX_WORD_LEN+2];
	char dataWord[WORD_SIZE];
	int j, m;
	int maxMemSize = 0;
	int comaSeen = 1, trailingComa = 0, inValid =0;
		

	/* Insert ints to data mem */
	if(type == 'd')
	{
		while(readNextWordC(line, data, i, ','))
		{
			trailingComa = 0;

			if(!isInt(data))
			{
				printError(fileName, lineNum, ERR_CODE_12);
				*error = 1;
			}
			
			/* Insert data int to data mem */
			memcpy(dataWord, decToBin(atoi(data), WORD_SIZE), WORD_SIZE);
			dataWord[WORD_SIZE] = '\0';
			addMemEntry(dataMem, dataWord, error);
			
			if(!comaSeen)
				inValid = 1;
			if(!readComma(line, i))
				comaSeen = 0;
			else
				trailingComa = 1;
		}

		if(inValid)
		{
			printError(fileName, lineNum, ERR_CODE_14);
			*error = 1;
		}
		
		if(trailingComa)
		{
			printError(fileName, lineNum, ERR_CODE_15);
			*error = 1;
		}

	}
	
	/* Insert a string tp data mem */
	if(!maxMemSize && type == 's')
	{
		j = *i;
		if(!validString(line, i, &j))
		{
			printError(fileName, lineNum, ERR_CODE_16);
			*error = 1;
			inValid = 1;
		}
		
		/* If valid, insert string chars to mem*/
		for(m = *i + 1; !inValid && m <= j; m++)
		{
			if(m != j)
				memcpy(dataWord, decToBin((int)line[m], WORD_SIZE), WORD_SIZE);
			else
				memcpy(dataWord, decToBin(0, WORD_SIZE), WORD_SIZE); /* insert NULL terminator */
				
			dataWord[WORD_SIZE] = '\0';
			addMemEntry(dataMem, dataWord, error);	
		}
	}

}

void insertSymbolToMem(Mem *mem, int addrMthd, char *word, Table *symbolTable, FILE * extFile, char* fName, int lineNum, int *error)
{
	char *bin;
	int symbolAddr;
	int symbType;
	
	bin = malloc(WORD_SIZE+1);
	 
	if(!(symbolAddr = get(symbolTable,((addrMthd == 1) ? word : word+1), 'a')))
	{	
		*error = 1;
		printError(fName, lineNum, ERR_CODE_25);
	}
	
	if(addrMthd == 2) /* insert symbol of type & */
	{
		strcpy(bin, decToBin( symbolAddr - mem->memCounter + 1, WORD_SIZE-3));
		strcat(bin, ARE_100);
		addMemEntry(mem, bin, error);
		free(bin);
		return;
	}
	
	symbType = get(symbolTable, word, 't');
	
	/* if Label defined externally */
	if(symbType == 4 || symbType == 6 || symbType == 8) 
	{
		/* Insert symbol to .ext file */
		fprintf(extFile, "%s %06d\n", word, mem->memCounter);
		strcpy(bin, decToBin(symbolAddr-1, WORD_SIZE-3));
		strcat(bin, ARE_001);
	}
	else
	{
		strcpy(bin, decToBin(symbolAddr, WORD_SIZE-3));
		strcat(bin, ARE_010);
	}
		
	addMemEntry(mem, bin, error);	
	free(bin);
}

void freeAllMemory(Mem * instMem, Mem * dataMem, Table * symbolTable)
{
	if(instMem)
		free(instMem);
	if(dataMem)
		free(dataMem);
	if(symbolTable)
		freeTable(symbolTable);
}
