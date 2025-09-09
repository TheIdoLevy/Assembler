/*
This file handles validating various paramaters across the assembly process.
It includudes validating macro names, validating an addressing method or number of operands,
validating the symbol table anf validating the syntax off a string 
*/

#include <ctype.h>
#include <string.h>

#include "dataStructs.h"
#include "firstScan.h"
#include "validators.h"
#include "errors.h"



int validMacroName(char *mcroName)
{
	/* A valid macro name is composed only of alphabetical chars and / or '_',
	and cannot be a register/operation name */
	int i;
	char firstChar = mcroName[0];
	char *regNames[] = {"r1","r2","r3","r4","r5","r6","r7","r8"};
	char *opNames[] ={"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","stop", ".string", ".data"};
	
	/* mcro name exceeds max macro length */
	if(strlen(mcroName) > MAX_MCRO_LEN)
		return 0;
	
	/* mcro name equals to a register name */	
	for(i = 0; i < NUM_REGS; i++)
		if(!strcmp(mcroName,regNames[i]))
			return 0;
	
	/* mcro name equals to an operation name */		
	for(i = 0; i < NUM_OPS; i++)
		if(!strcmp(mcroName,opNames[i]))
			return 0;
			
	/* Invalid name */
	for(i = 1; i < strlen(mcroName); i++)
	{
		if(mcroName[i] != '_')
			if(!isalpha(mcroName[i]) && !isdigit(mcroName[i]))
				return 0;
	}
	
	if(!isalpha(firstChar) && firstChar != '_')
		return 0;
				
	return 1;
}

int validNumOperands(int opcode, int numOps, char* fileName, int lineNum, int *error)
{
	/* Based on table in manual on this assembly language */
	if( (opcode >= 0 && opcode <= 4 && numOps != 2)
		|| (opcode > 4 && opcode <= 13 && numOps != 1)
		|| (opcode >= 14 && numOps != 0) )
	{
		printError(fileName, lineNum, ERR_CODE_20);
		*error = 1;
		return 0;		
	}
	return 1;
}

void validAddressing(int opcode, char* op1, char* op2, char* fileName, int lineNum, int *error, Commands cmds[])
{
	int j,i;
	int op1Valid = 0;
	int op2Valid = 0;
	
	for(j = 0; cmds[j].name != NULL; j++)
		if(cmds[j].opcode == opcode)
			break;
	
	/* Go over the cmds table and check if addressing methos of
	 operands to current instruction are valid */
	for(i = 0; i <= 2; i++)
	{
		if(cmds[j].op1AddrMethod[i] == addressingMethod(op1))
			op1Valid = 1;
		if(cmds[j].op2AddrMethod[i] == addressingMethod(op2))
			op2Valid = 1;
		
	}

	if(!op1Valid || !op2Valid)
	{
		printError(fileName, lineNum, ERR_CODE_19);
		*error = 1;	
	}
}

void validateSymbolTable(Table *symbolTable, char *fName, int *error)
{
	int j;
	/* Check if a symbol in symbol table exists that has only been defined as .entry */
	for(j = 0; j < symbolTable->size; j++)
	{
		if(symbolTable->entries[j].symbolType[0] == 'e' || 
		symbolTable->entries[j].symbolType[1] == 'e')
		{
			if(symbolTable->entries[j].symbolType[0] != 'c' &&
			symbolTable->entries[j].symbolType[0] != 'd' &&
			symbolTable->entries[j].symbolType[1] != 'c' &&
			symbolTable->entries[j].symbolType[1] != 'd')
			{
				*error = 1;
				printSymbolError(fName, symbolTable->entries[j].symbolName, ERR_CODE_30);
			}
		}
	}	
}

int validString(char *line, int *i, int *k)
{
	/* A valid string must have an opening and closing " */
	
	int j;
	/* search for first " */
	while(isspace(line[*i]))
		(*i)++;
		
	j = *i;	
		
	while(line[j] != '\0')
		j++;
	
	/* search for last " */	
	while(isspace(line[j--]))
		;
	
	*k = j;	
	if(j <= *i || line[*i] != '"' || line[j] != '"')
		return 0; /* Invalid */

	return 1; /* Valid */
}
