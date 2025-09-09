#ifndef SECONDSCAN_h
#define SECONDSCAN_h

#include "dataStructs.h"
#include "memoryHandle.h"

/**
* This function handles the creation and filling of the .obj file afer the second scan of the assembler is done
*
* @param instMem the instruction memory
* @param dataMem the data memory
* @param ICF the Final Instruction Counter, defined after the firstScan
* @param DCF the Final Data Counter, defined after the firstScan
* @param inputFName the name of the file which is currently being parsed
*/
void createObj(Mem *instMem, Mem *dataMem, int ICF, int DCF, char *inputFName);


/**
* This function handles the creation and filling of the .ent file afer the second scan of the assembler is done
*
* @param symbolTable the table containing all symbol deifnitions in current file
* @param inputFName the name of the file which is currently being parsed
*/
void createEnt(Table *symbolTable, char* inputFName);


/**
* This function is the second scan of the assembler. It traverses the code, and inserts the addresses of symbols
* that are defined in the symbol table to instruction and data memory. It handles the creation of the .obj .ent
* and .ext files by calling correspondent functions
*
* @param fName the name of the input file (with the .am extension)
* @param inputFName the name of the file without an extension (as supplied in the terminal)
* @param instMem the instruction memory
* @param dataMem the data memory
* @param ICF the Final Instruction Counter, defined after the firstScan
* @param DCF the Final Data Counter, defined after the firstScan
* @param symbolTable the table containing all symbol deifnitions in current file
* @param error a flag used to detect errors, passed as argument in firstScan (so if there was an error there,
* 	secondScan will detect the error and act accordingly)
*/
void secondScan(char *fName, char *inputFName, Mem *instMem, Mem *dataMem,
		int ICF, int DCF, Table *symbolTable, int error);


#endif
