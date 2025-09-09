/*
This file manages the second pass of the assembler. It inserts the memory addresses of symbols from the symbol address to memory. It then (if no error was detected) creates the .obj, .ent, and .ext files.
*/

#include <string.h>
#include <stdlib.h>

#include "dataStructs.h"
#include "errors.h"
#include "fileOperations.h"
#include "utils.h"
#include "validators.h"
#include "firstScan.h"
#include "memoryHandle.h"

void createObj(Mem *instMem, Mem *dataMem, int ICF, int DCF, char *inputFName)
{
	FILE *objFile;
	int i = 100;
	char* hex;
	char *newFName;
	int mallocErr = 0;
	
	/* create a new file with a .obj extension */
	newFName = addFileExtension(inputFName, ".obj", &mallocErr);
	objFile = fopen(newFName,"w");

	/* Write number of items in inst mem and data mem int obj file */
	fprintf(objFile,"    %d %d\n", ICF-100, DCF);
	
	/* Insert all data from inst mem to .obj file in hex base */
	for( ; i < ICF; i++)
	{
		hex = binToHex(instMem->memory[i].data);
		fprintf(objFile, "%06d %s\n", i, hex);
		free(hex);
	}
	
	/* Insert all data from data mem to .obj file in hex base */
	for(i = 0 ; i < DCF; i++)
	{
		hex = binToHex(dataMem->memory[i].data);
		fprintf(objFile, "%06d %s\n", ICF + i, hex);
		free(hex);
	}
	
	fclose(objFile);
}

void createEnt(Table *symbolTable, char* inputFName)
{
	FILE *entFile;
	int j;
	
	/* Siginifies if not entries were found */
	int empty = 1;
	
	char *newFName;
	int mallocErr = 0;
	
	/* create a new file with a .obj extension */
	newFName = addFileExtension(inputFName, ".ent", &mallocErr);
	entFile = fopen(newFName,"w");
	
	for(j = 0; j < symbolTable->size; j++)
	{
		/* If label defined as .entry */
		if(symbolTable->entries[j].symbolType[0] == 'e' || 
		symbolTable->entries[j].symbolType[1] == 'e')
		{
			empty = 0;
			fprintf(entFile, "%s %06d\n", symbolTable->entries[j].symbolName, 
			symbolTable->entries[j].addr);
		}
	}
	
	fclose(entFile);
	
	/* If no entries were found */
	if(empty)
		remove(newFName);
}

void secondScan(char *fName, char *inputFName, Mem *instMem, Mem *dataMem,
		int ICF, int DCF, Table *symbolTable, int error)
{	
	FILE *fp;
	FILE *extFile;
	char *extFileName;
	
	char line[MAX_LINE_LEN+1];
	char word[MAX_WORD_LEN+1];
	
	int i = 0;
	
	int skipLine = 0; 
	int lineNum = 1;
	
	/* Indicates wether or not a .ext file was created */
	int createdExt = 0;
	
	int mallocErr = 0;
	
	fp = fopen(fName, "r+");
	instMem->memCounter = 100;
	
	/* Check if no symbol was defines as only .entry in file */	
	validateSymbolTable(symbolTable, fName, &error);
	
	if(!error)
	{
		/* In order to know if need to close file in case of error */
		createdExt = 1; 
		
		/* Create .ext file */
		extFileName = addFileExtension(inputFName, ".ext", &mallocErr);
		extFile = fopen(extFileName,"w");
	}
	
	/* While not last line of file, read line to inputBuffer */
	while(readNextLine(fp, line)) 
	{
		readNextWord(line,word,&i);
		
		/* If comment */
		if(word[0] == ';')
			skipLine = 1;

		if(!strcmp(word, ".extern") || !strcmp(word, ".entry"))
			skipLine = 1;
		
		/* If label deifinition */
		if(addressingMethod(word) == 4)
			readNextWord(line,word,&i); 
		
		/* To check for label before and after e/x */
		if(!strcmp(word, ".extern") || !strcmp(word, ".entry")) 
			skipLine = 1;
			
		if(!strcmp(word, ".data") || !strcmp(word, ".string"))
			skipLine = 1;
		
		/* If this point is reached, then word contains an operation name */
		if(!error && !skipLine) 
		{
			/* Move ic to an address where a symbol needs to be inserted */
			/* For first operand (if applicable): */
			instMem->memCounter++;
			readNextWordC(line,word,&i,',');
			
			/* Avoid confusing stop and rts as symbol names (zero operand insts) */
			if(addressingMethod(word) == 1 && strcmp(word, "stop") && strcmp(word, "rts")) 
			{
				insertSymbolToMem(instMem, 1, word, symbolTable, 
				extFile, fName, lineNum, &error);
				
			} else if(addressingMethod(word) == 2)
			{
				insertSymbolToMem(instMem, 2, word, symbolTable,
				extFile, fName, lineNum, &error);
				
			} else if(addressingMethod(word) == 0)
				instMem->memCounter++;
					
			if(!readComma(line, &i))
				skipLine = 1;
			readNextWord(line,word,&i);
			
			/* For second operand (if applicable): */
			if(addressingMethod(word) == 1 && strcmp(word, "stop") &&  strcmp(word, "rts") && !skipLine)
			{
				insertSymbolToMem(instMem, 1, word, symbolTable,
				extFile, fName, lineNum, &error);
				
			} else if(!skipLine&&addressingMethod(word) == 2)
			{
				insertSymbolToMem(instMem, 2, word, symbolTable,
				extFile, fName, lineNum, &error);
				
			} else if(!skipLine&&addressingMethod(word) == 0)
				instMem->memCounter++;
	
		}
		
		i = 0;
		skipLine = 0;
		lineNum++;
	}


	
	fclose(fp);
	if(createdExt)
		fclose(extFile);
	
	/* Create .obj and .ent files */	
	if(!error)
	{
		createObj(instMem,dataMem,ICF,DCF,inputFName);
		createEnt(symbolTable,inputFName);
		printf("Succesfully completed assembly for file %s\n\n",fName);
	} else
	{
		if(createdExt)
			remove(extFileName);
		printf("Error(s) encountered while assembling %s. Assembly terminated\n\n", fName);
	}

	freeAllMemory(instMem, dataMem, symbolTable);
}
