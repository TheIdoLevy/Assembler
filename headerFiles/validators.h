#ifndef VALIDATORS_h
#define VALIDATORS_h

#include "dataStructs.h"
#include "firstScan.h"

#define MAX_MCRO_LEN 31
#define NUM_REGS 8
#define NUM_OPS 16


/**
* Given a string as an input, this function checks wether the string is a valid macro definition; Can only contain * letters from the english alphabet and the char '_', cannot be name of defined operation or register
*
* @param mcroName the name of the macro that is being checked
*
* @return returns 0 if invalid, 1 if vald
*/
int validMacroName(char *mcroName);


/**
* Given an opcode of an instruction and the number of operands supplied to it, this function checks if the nmuber * of operands supplied is valid.
*
* @param opcode the opcode of the instruction
* @param numOps the number of operands supplied to the instruction in a line of code
* @param fileName the name of the file from which this instruction was read from
* @param lineNum the number of the line in the file from which this instruction was read
* @param error a flag indicating if an error was encountered during the first scan of the assembler
*
* @return 0 if the number of operands supplies is invlaid, and 1 otherwise
*/
int validNumOperands(int opcode, int numOps, char* fileName, int lineNum, int *error);


/**
* Given an opcode of an instruction and the number the operands supplied to it in a line in a file, this function * chekcs if the addresing methods of these operands are compatible with th opcode
*
* @param opcode the opcode of the instruction
* @param op1 the first operand (if supplied) that is given to the inst
* @param op2 the second operand (if supplied) that is given to the inst
* @param fileName the name of the file from which this instruction was read from
* @param lineNum the number of the line in the file from which this instruction was read
* @param error a flag indicating if an error was encountered during the first scan of the assembler
* @param cmds a table containtng data about the valid addressing methods of the source and detination operands of an instruction
*
*@return 1 if the addressing methods are correct, and 0 otherwise
*/
void validAddressing(int opcode, char* op1, char* op2, char* fileName, int lineNum, int *error, Commands cmds[]);


/**
* This function scans the symbolTable after the firstScan is complete, and checks for invalid symbols that have 
* been defined as .entry but not defined elsewhere in the file
*
* @param symbolTable the symbol table after the first scan of the assebler is complete
* @param fName the name of the file that the assembler is currently assembling
* @param error used to detect errors in the second scan of the assembler
*/
void validateSymbolTable(Table *symbolTable, char *fName, int *error);


/**
* This functions validates the syntax of a string on a .data defintion - a string must contain an opening and 
* closing '"'
*
* @param line the line currently being parsed
* @param i the index in the line from which to start reading from
* @param k and index helper to find the last occurence of '"' in the line
*
* @return returns 1 if the string is valid, and 0 otherwise
*/
int validString(char *line, int *i, int *k);

#endif
