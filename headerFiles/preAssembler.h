#ifndef PREASSEMBLER_h
#define PREASSEMBLER_h

#define mcro "mcro"
#define mcroend "mcroend"
#define MAX_WORD_LEN 31
#define MAX_MCRO_LEN 31
#define MAX_LINE_LEN 81


/**
* This file checks if a macro was previously deifned, and if macro deifnition was found, the parameter mcroAddr[0] 
* gets the address of deifiniton of the macro, and mcroAddr[1] gets the address of the end of the defition of
* the macro
* 
* @param word the name of the macro to be searched for
* @param listHead the linked list containing all macro deifnitions
* @param mcroAddr an int array  deifned outside of this function, gets address of start and end of current macro
* 	 defintion
*
* @return return 1 if macro was found in the list (macro was predefined), and 0 otherwise
*/
int isMacroDefined(char *word, ptr listHead, int *mcroAddr);


/**
* Given a name of a macro, this function checks wether this macro has already been defined elswhere in the 
* same file, and returns 1 (as an error indicator) if so
*
* @param name the name of the macro which is being searched for
* @param listHead the head of the linked list containing all macro definitions in current file
* 
* @return return 1 if macro with the same name was defined twice, and 0 otherwise
*/
int doubleDefinition(char *name, ptr listHead);


/**
* This file reads all bytes between start and end (in terms of offset in bytes from the beginign of the source   
* file) from a destination file to a source file
*
* @param src a pointer to the source file from which data will be read from
* @param dest a pointer to the file to which data will be written to
* @param start the offest in bytes from the begining of src file, from which to start reading data from
* @param end the offest in bytes from the begining of src file, from which to end reading data when reached
*/
void spreadMacro(FILE *src, FILE *dest, int start, int end);


/**
* This function is responsible for the pre-assembler process. It opens a new file with a ".am" extension, and
* copies the contents of its input file into the new file, while spreadin macros
* 
* @param fp a pointer to an input file
* @param the name of the input file without the .as extension
*
* @return the name if the new file with the .am extension, which contains the spread macros (if defined) 
*/
char* preAssembler(FILE *fp, char* fileName);

#endif
