/*
This file handles errors in all the preAssembler phases, by identifying them (based in supplied error code) and printing them to the trminal for the user to see errors in source code or internal errors
*/

#include <stdio.h>
#include "errors.h"

Error errors[] = {
	{ERR_CODE_0, "No Error"},
	{ERR_CODE_1, "Invalid macro name"},
	{ERR_CODE_2, "Extra text after macro definition"},
	{ERR_CODE_3, "Extra text after macro end"},
	{ERR_CODE_4, "Missing macro name on macro definition line"},
	{ERR_CODE_5, "Cannot define two macros with the same name"},
	{ERR_CODE_6, "Could not create new file"},
	{ERR_CODE_7, "Failed to dynamically allocate memory. Exiting program execution"},
	{ERR_CODE_8, "Symbol Name exceeds maximum of 31 characters"},
	{ERR_CODE_9, "Cannot define a label on .entry or .extern"},
	{ERR_CODE_10, "Expected text after label definition"},
	{ERR_CODE_11, "Maximum Data memory size exceeded"},
	{ERR_CODE_12, "Expected type INT in .data definition"},
	{ERR_CODE_13, "Max line len of 81 chars exceeded"},
	{ERR_CODE_14, "Missing comas or too many between ints on .data definition"},
	{ERR_CODE_15, "Trailing , on .data definition"},
	{ERR_CODE_16, "Missing quotes "" on .string definition"},
	{ERR_CODE_17, "Label name on .entry/.extern definition must be valid"},
	{ERR_CODE_18, "Invalid instruction; undefined operation"},
	{ERR_CODE_19, "Invalid addressing method used for insruction"},
	{ERR_CODE_20, "Invalid number of operands supplied to instruction"},
	{ERR_CODE_21, "Trailing , on instruction line"},
	{ERR_CODE_22, "Extra text on instruction line"},
	{ERR_CODE_23, "Cannot define .e/.x label: .e/.x"},
	{ERR_CODE_24, "Too many sequential ','"},
	{ERR_CODE_25, "Undefined symbol"},
	{ERR_CODE_26, "This label has already beeen defined elsewhere"},
	{ERR_CODE_27, "Label already defined as .entry cannot be defined as .extern"},
	{ERR_CODE_28, "Label already defined as .extern cannot be defined as .entry"},
	{ERR_CODE_29, "Unexpected ','"},
	{ERR_CODE_30, "This label has been defined as .entry but hasn't been defined in this file"},
	{ERR_CODE_31, "You must give a file name as an input to the assembler"},
	{ERR_CODE_32, "File not found - cannot open file"},
	{ERR_CODE_33, "Could not open a new file"},
};

void printError(char *fileName, int lineNum, int errCode)
{
	printf("ERROR in file %s, line %d: %s\n", fileName, lineNum, errors[errCode].errMsg);
}

void printInternalError(int errCode)
{
	printf("ERROR: %s\n", errors[errCode].errMsg);
}

void printSymbolError(char *fileName, char * symbolName, int errCode)
{
	printf("ERROR in file %s, label: %s; %s\n", fileName, symbolName, errors[errCode].errMsg);
}
