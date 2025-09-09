#ifndef FIRSTSCAN_h
#define FIRSTSCAN_h

#include "dataStructs.h"
#include "memoryHandle.h"

#define INIT_TABLE_SIZE 1000
#define INIT_MEM_SIZE 1000
#define WORD_SIZE 24

/* Addressing methods */
#define ADDR_MTHD_0 0
#define ADDR_MTHD_1 1
#define ADDR_MTHD_2 2
#define ADDR_MTHD_3 3
#define LABEL_DEF 4

#define MAX_LABEL_LEN 31
#define MAX_WORD_LEN 31
#define MAX_LINE_LEN 81

#define EMPTY_OPERAND -1
#define INVALID_OPERAND -2
#define TOO_LONG -1

/* For initiallization of the first word of an instruction */
#define BIT_16 16
#define BIT_6 6
#define BIT_11 11
#define BIT_21 21
#define BIT_13 13
#define BIT_8 8

/**
* Commands is a table holding the following data about every operation :
* name; the name of the operation
* opcode; the opcode of the operation
* funct; the function number of the operation (if applicable)
* op1AddrMethod: an array containing the valid addressing methods of operand 1 of the operation
* op2AddrMethod: an array containing the valid addressing methods of operand 2 of the operation
*/
typedef struct {
	char *name;
	int opcode;
	int funct;
	int op1AddrMethod[3];
	int op2AddrMethod[3];
} Commands; 


/**
* This function returns the addressing method of the operand passed in as a paramter. These range from -1 to 4
* where -1 is undefines, 0 - 3 is as described in the manual of this assembly language, and 4 is for label
* definition
*
* @param operand the operand who's addressing method is being checked
*
* @return the addressing method of the operand
*/
int addressingMethod(char *operand);


/**
* This function builds the first binary word of a binary encoded instruction
*
* @param word1 the string that will contain the first word of current instruction
* @param op the opcode of current intruction
* @param funct the funct of current instruction
* @param addrMthOp1 the addressing method of the first operand (if applicable)
* @param addrMthOp2 the addressing method of the second operand (if applicable)
* @param operand1 first operand supplied to instruction (if applicable)
* @param operand2 second operand supplied to instruction (if applicable)
*/
void buildFirstWordOfInst(char *word1, int op, int funct, int addrMthdOp1, int addrMthdOp2,
			char *operand1, char *operand2);


/**
* This function is resposible for saving space in memory for symbols (which will be filled in the second pass if no
* erorrs), and inserting the first encoded word and operands (that are not symbols) of an instrucion into memory
*
* @param line the line containing the current instruction
* @param i the indec from which to start reading from line
* @param op the opcode of current intruction
* @param funct the funct of current instruction
* @param fileName the name of the file currently being parsed
* @param lineNum the number of the line in the file currently being parsed
* @param error a flag used to detect erros in the assmebly of the file
* @param cmds the table containing data about every operation
* @param instMem the instruction memory
*/
void addOperands(char *line, int i, int op, int funct, char* fileName, int lineNum, int *error, Commands cmds[], Mem *instMem);


/**
* This function performs the first scan of the assmebler on the input file. It inserts data into memory and saves 
* space for symbols, and inserts symbols to the symbolTable
*
* @param inputFile the name of the file to operate on (with the .am extension)
* @param origFile the name of the input file without an extension 
*/
void firstScan(char* inputFile, char* origFile);

#endif
