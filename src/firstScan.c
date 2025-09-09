/*
This file is responsible for the first scan of the assembler on the input file
It inserts symbols to the symbol table, and inserts the first word of every operation
 in the instruction memory. It inserts data to the data memory, and saves apce for symbols
 on the instruction memory for the second scan. 
It also detects syntax error in the source file (after the macros have been spread)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dataStructs.h"
#include "firstScan.h"
#include "errors.h"
#include "fileOperations.h"
#include "utils.h"
#include "secondScan.h"
#include "preAssembler.h"
#include "validators.h"
#include "memoryHandle.h"


int addressingMethod(char *operand) /* Addressing method */
{		
	int i;
	char *registerNames[] = {"r1","r2","r3","r4","r5","r6","r7","r8"};

	if(operand[0] == '\0')
		return EMPTY_OPERAND;
		
	if(operand[0] == '#')
	{
		if(operand[1] == '+' || operand[1] == '-' || isdigit(operand[1]))
		{
			i = 2;
			while(operand[i] != '\0' && isdigit(operand[i]))
				i++;
			
			/* operand is a number */
			if(operand[i] == '\0')
				return ADDR_MTHD_0;

			return INVALID_OPERAND;
		}
	}
	 
	for(i = 0; i < NUM_REGS; i++)
	{
		if(!strcmp(registerNames[i], operand))
		{
			/* operand is a register name */
			return ADDR_MTHD_3;
		}
	}
	
	i = 1;
	while(operand[i] != '\0' && (isdigit(operand[i]) || isalpha(operand[i])))
		i++;

	if(operand[i] == '\0')
	{
		/* operand for branch or jmp */
		if(operand[0] == '&')
			return ADDR_MTHD_2;

		/* A name of a symbol */
		if(isalpha(operand[0]) && validMacroName(operand))
			return ADDR_MTHD_1;

		return EMPTY_OPERAND;

	} else if(operand[i] == ':' && operand[i+1] == '\0' 
			&& isalpha(operand[0]))
		return LABEL_DEF; /* label definition (valid label name followed by ':' */
	
	return INVALID_OPERAND;
}

void buildFirstWordOfInst(char *word1, int op, int funct, int addrMthdOp1, int addrMthdOp2,
			char *operand1, char *operand2)
{
	/* Initiate first word for current instruction  */
	
	/* Init opcode field */
	memcpy(word1, decToBin(op, 6), 6);
		
	/* Init funct field */
	if(op == 2 || op == 5 || op == 9) 
		memcpy(&word1[BIT_16], decToBin(funct,5), 5);
	else 
		memcpy(&word1[BIT_16], decToBin(0,5), 5);
	
	/* Init addressing method of operand 1 field */
	if(op >= 0 && op <= 4)
		memcpy(&word1[BIT_6], decToBin(addrMthdOp1,2), 2);
	else
		memcpy(&word1[BIT_6], decToBin(0,2), 2);
			
	/* Init addressing method of operand 2 field */	
	if(op >= 0 && op <= 4)
		memcpy(&word1[BIT_11], decToBin(addrMthdOp2,2), 2);
	else if(op > 4 && op <= 13)
		memcpy(&word1[BIT_11], decToBin(addrMthdOp1,2), 2); /* This set of insts have one operand */
	else
		memcpy(&word1[BIT_11], decToBin(0,2), 2);
		
		
	/* Init ARE field */
	memcpy(&word1[BIT_21], decToBin(4, 3), 3);
		
	/* Init reg field. remember, regs from 0 to 7*/	
	if(addrMthdOp1 == 3)
		if(op >= 0 && op <= 4)
			memcpy(&word1[BIT_8], decToBin(operand1[1]-'0',3),3); /* -'0' to get int num of reg */
		else
			memcpy(&word1[BIT_13], decToBin(operand1[1]-'0',3),3);
	else
		memcpy(&word1[BIT_8], decToBin(0,3),3);
			
	if(addrMthdOp2 == 3)
		memcpy(&word1[BIT_13], decToBin((operand2[1]-'0'),3),3); /* -'0' to get int num of reg */
	else
		if(op >= 0 && op <= 4)
			memcpy(&word1[BIT_13], decToBin(0,3),3);
}


void addOperands(char *line, int i, int op, int funct, char* fileName, int lineNum, int *error, Commands cmds[], Mem *instMem)
{
	/* operands supplied to instruction */
	char operand1[MAX_WORD_LEN+2];
	char operand2[MAX_WORD_LEN+2]; 
	
	/* used to find trailing text on inst line*/
	char errorCatcher[MAX_WORD_LEN+2];
	
	/* First word (binary) of inst*/
	char word1[WORD_SIZE+1];
	
	/* Number of operands supplied to inst*/
	int numOperands = 0;
	
	int j;
	int addrMthdOp1, addrMthdOp2;
	int comaErr = 0;
	
	
	operand1[0] = '\0';
	operand2[0] = '\0';
	errorCatcher[0] = '\0';
	
	readNextWordC(line,operand1,&i,','); 
	
	/* Inst with opcode greater than 4 get no more than one operand*/
	if(readComma(line, &i) && op > 4)
	{
		*error = 1;
		comaErr = 1;
		printError(fileName, lineNum, ERR_CODE_29);
	}
	
	j = i;
	
	/* Too many commas in a row */
	if(readComma(line, &j))
	{
		*error = 1;
		comaErr = 1;
		printError(fileName, lineNum, ERR_CODE_24);
	}
		
	readNextWordC(line,operand2,&i,',');
	j = i;
	
	if(!comaErr && readComma(line, &j))
	{
		printError(fileName, lineNum, ERR_CODE_21);
		comaErr = 1;
		*error = 1;		
	}
	
	i++;
	readNextWord(line,errorCatcher,&i);
	
	/* Count the number of operands supplies to instruction */
	if(operand1[0] != '\0')
		numOperands++;
	if(operand2[0] != '\0')
		numOperands++;
	if(errorCatcher[0] != '\0')
		numOperands++;
		
	for(j = 0; j < WORD_SIZE; j++)
		word1[j] = 0+'0';
	word1[WORD_SIZE] = '\0';
	
	if(comaErr)
		return;
	
	if(validNumOperands(op, numOperands, fileName, lineNum, error))
		validAddressing(op, operand1, operand2, fileName, lineNum, error, cmds);
	addrMthdOp1 = addressingMethod(operand1);
	addrMthdOp2 = addressingMethod(operand2);


	buildFirstWordOfInst(word1, op, funct, addrMthdOp1, addrMthdOp2, operand1, operand2);	
	addMemEntry(instMem, word1, error);

	/* Insert first intruction word and operands to memory */
	/* If an operand is a symbol name, increase ic (save space on mem) */	
	if(addrMthdOp1 == ADDR_MTHD_1 || addrMthdOp1 == ADDR_MTHD_2)
		instMem->memCounter++;
	else if(addrMthdOp1 == ADDR_MTHD_0)
		insertNumToMem(instMem, operand1, error);
			
	if(addrMthdOp2 == ADDR_MTHD_1 || addrMthdOp2 == ADDR_MTHD_2)
		instMem->memCounter++;
	else if(addrMthdOp2 == ADDR_MTHD_0)
		insertNumToMem(instMem, operand2, error);
}


void firstScan(char* inputFile, char* origFile)
{
	/* This table contains information about every operation */
	/* From left to right */
	/* Column 1: operation name */
	/* Column 2: operation opcode */
	/* Column 3: operation funct (-1 if not applicable) */
	/* Column 4: list of all addressing methods allowed for first operand of operation */
	/* Column 5: list of all addressing methods allowed for second operand of operation */
	/* Columns 4&5: -1 means expected no operand */
	Commands cmds[]={
		{"mov", 0, -1, {0,1,3},{1,3,3}},
		{"cmp", 1, -1, {0,1,3},{0,1,3}},
		{"add", 2, 1, {0,1,3},{1,3,3}},
		{"sub", 2, 2, {0,1,3},{1,3,3}},
		{"lea", 4, -1, {1,1,1},{1,3,3}},
		{"clr", 5, 1, {1,3,3}, {-1,-1,-1}},
		{"not", 5, 2, {1,3,3}, {-1,-1,-1}},
		{"inc", 5, 3, {1,3,3}, {-1,-1,-1}},
		{"dec", 5, 4, {1,3,3}, {-1,-1,-1}},
		{"jmp", 9, 1, {1,2,2}, {-1,-1,-1}},
		{"bne", 9, 2, {1,2,2}, {-1,-1,-1}},
		{"jsr", 9, 3, {1,2,2}, {-1,-1,-1}},
		{"red", 12, -1, {1,3,3}, {-1,-1,-1}},
		{"prn", 13, -1, {0,1,3},{-1,-1,-1}},
		{"rts", 14, -1, {-1,-1,-1},{-1,-1,-1}},
		{"stop", 15, -1, {-1,-1,-1},{-1,-1,-1}},
		{NULL, -1,-1,{-1,-1,-1},{-1,-1,-1}}
	};


	/* Next line from file gets written to line */
	char line[MAX_LINE_LEN+2]; 
	/* Next word from line gets written to word */
	char word[MAX_WORD_LEN+2]; 
	
	int i = 0, j;
	int lineNum = 1;

	int opcode;
	int funct;
	
	int ICF, DCF;
	
	int symbolDef = 0;
	char symbolName[MAX_WORD_LEN+2];
	
	int entryDef = 0;
	int externDef = 0;
	int dataDef = 0;
	
	/* Indicates if line exceeds max of 81 chars */
	int lineStatus;
	
	/* If comment has been deinfed */
	int comment = 0;
	
	int error = 0, memAllocError = 0;
	
	Table *symbolTable;

	Mem *instMem;
	Mem *dataMem;
	
	FILE *fp = fopen(inputFile, "r+");
	
	/* Initiallize symbol table, instruction mem, and data mem */
	symbolTable = createTable(INIT_TABLE_SIZE, &memAllocError);
	instMem = initMemory(1, &memAllocError);
	dataMem = initMemory(0, &memAllocError);
	
	if(memAllocError)
	{
		printInternalError(ERR_CODE_7);
		exit(1);
	}
	
	/* While not last line of file, read line to inputBuffer */
	while((lineStatus = readNextLine(fp, line))) 
	{
		j = 0;
		if(lineStatus == TOO_LONG)
		{
			printError(origFile, lineNum, ERR_CODE_13);
			error = 1;
		}
		
		readNextWord(line,word,&i);
		
		/* Comment has been defined, skip line*/
		if(word[0] == ';')
			comment = 1;

		/* Check is this is a .extern or .entry definition */
		if(!comment && !strcmp(word, ".extern"))
		{
			externDef = 1;
			readNextWord(line,word,&i);
		} else if(!comment && !strcmp(word, ".entry"))
		{
			entryDef = 1;
			readNextWord(line,word,&i);
		} 

		/* If a label is being defined */
		if(!comment && addressingMethod(word) == 4) 
		{
			/* if line is of type: .entry/.extern LABEL: ...*/
			if(entryDef || externDef)
			{
				printError(origFile, lineNum, ERR_CODE_9);
				error = 1;
			}
			
			symbolDef = 1;
			
			/* Copy name of symbol without the ':' */
			readNextWordC(word, symbolName, &j, ':');
			
			if(strlen(symbolName) > MAX_LABEL_LEN)
			{
				printError(origFile, lineNum, ERR_CODE_8);
				error = 1;
				symbolDef = 1;
			}; 
			
			
			/* If no text after label definition */
			if(!readNextWord(line,word,&i) && !externDef && !entryDef)
			{
				printError(origFile, lineNum, ERR_CODE_10);
				error = 1;
			}
			
		} 
		
		/* If Label: .extern/.entry ...*/
		if(!comment && symbolDef && !strcmp(word, ".extern"))
		{
			if(entryDef || externDef)
			{
				printError(origFile, lineNum, ERR_CODE_23);
				error = 1;
			}
			externDef = 1;
			readNextWord(line,word,&i);
			
		} else if(!comment && symbolDef && !strcmp(word, ".entry"))
		{
			if(entryDef || externDef)
			{
				printError(origFile, lineNum, ERR_CODE_23);
				error = 1;
			}
			entryDef = 1;
			readNextWord(line,word,&i);
		}
		
		/* Defining data / string */
		if(!comment && (!strcmp(word, ".data") || !strcmp(word, ".string")))
		{
			dataDef = 1;
			
			/* if symbol deinfed, insert symbol to symbol table */
			if(symbolDef)
				insert(symbolTable, symbolName, dataMem->memCounter, 'd',  
				origFile, lineNum, &error);
				
			/* insert data to data mem */
			if(!strcmp(word, ".data"))
				insertData(line, &i, 'd', origFile, lineNum, &error, dataMem);
			else if(!strcmp(word, ".string"))
				insertData(line, &i, 's', origFile, lineNum, &error, dataMem);
		
		}
		
		/* If .extern/.entry statment and no label defined before */
		if(!comment && (entryDef || externDef))
		{
			/* If valid label name after .entry/.extern */
			if(addressingMethod(word)==1)
			{
				/* Insert symbols to symbol table*/
				if(externDef)
					insert(symbolTable, word, dataMem->memCounter, 'x',  
					origFile, lineNum, &error);
				else if(entryDef)
					insert(symbolTable, word, dataMem->memCounter, 'e',
					origFile, lineNum, &error);
			} else
			{
				printError(origFile, lineNum, ERR_CODE_17);
				error = 1;
			}
			
		} else if(!comment && (!strcmp(word, ".extern") || !strcmp(word, ".entry"))) 
		{
			/* If label defined before .extern/.entry statement. It has no meaning */
			readNextWord(line,word,&i);
			
			/* If a valid symbol name (syntax-wise) */
			if(addressingMethod(word) == 1) 
				insert(symbolTable, word, dataMem->memCounter, 
				(!strcmp(word, ".extern")) ? 'x' : 'e', origFile, lineNum, &error);
			else
			{
				printError(origFile, lineNum, ERR_CODE_17);
				error = 1;
			}
		}
		
		/* If current line is not a comment or .entry/.extern/data def */
		if(!comment && !dataDef && !entryDef && !externDef)
			for(j = 0; cmds[j].name != NULL; j++)
				if(!strcmp(cmds[j].name, word))
					break;
				
		if(!comment && !dataDef && !entryDef && !externDef && cmds[j].name != NULL)
		{
			/* If symbol defined, insert it to symbol mem */
			if(symbolDef) 
				insert(symbolTable, symbolName, instMem->memCounter, 'c',
				 origFile, lineNum, &error);
	
			opcode = cmds[j].opcode;
			funct = cmds[j].funct;
		} 

		if(!comment && !dataDef && !entryDef && !externDef)
		{
			/* Build first inrtuction binary word of operation */
			if(cmds[j].name != NULL)
				addOperands(line, i, opcode, funct, origFile, lineNum, &error, cmds, instMem);
			else
			{
				/* Undefined operation */
				printError(origFile, lineNum, ERR_CODE_18);
				error = 1;
			}	
		}
		
		/* reset flags for next line scan */	
		i = 0;
		symbolDef = 0;
		entryDef = 0;
		externDef = 0;
		dataDef = 0;
		comment = 0;
		lineNum++;
	}

	/* Save final data and instruction counters */
	ICF = instMem->memCounter;
	DCF = dataMem->memCounter;
	
	fclose(fp);
	
	/* Update data symbols in symbol table to containt their address plus ICF */
	if(!error)
	{
		for(j = 0; j < symbolTable->size; j++)
			if(symbolTable->entries[j].symbolType[0] == 'd' || 
			symbolTable->entries[j].symbolType[1] == 'd')
				symbolTable->entries[j].addr = symbolTable->entries[j].addr + ICF;
	}
	
	/* Go to second scan of file. If error found here, secondScan won't create files */
	secondScan(inputFile, origFile, instMem, dataMem, ICF, DCF, symbolTable, error);
}
