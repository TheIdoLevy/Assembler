#ifndef FILEOPERATIONS_h
#define FILEOPERATIONS_h

#include <stdlib.h>
#define MAX_LINE_LEN 81

/**
This function attempts to open a file after concatenating ".as" to the name of the file it recieves from main
@param argv the name of the file (without the ".as" extension) that needs to be opened
@return if the file was found and opened succesfully, returns a pointer to the file. Otherwise, returns NULL
*/
FILE *openFile(int argc, char* );


/**
Given a file name and an extension, this function returns a new file name which is the concatenation of the two
@param fileName the name of the file to which concatenation will be done
@param extension the extension which will be concatenated
@param mallocErr allows other functions to detect memory allocation errors in this function
@retuen the new, extended file name or NULL of an error occured
*/
char* addFileExtension(char *fileName, char *extension, int *mallocErr);


/**
Given a pointer to a file (FILE *) and a char array, this function copies the next line from the file to the array. It will finish reading when encountered with EOF or '\n' or exceeded 81 chars in line
@param fp pointer to file from which to read from
@param line the string in which to read the line into
@return 0 id EOF, -1 if line length exceeded the maximum of 81 chars (including whitespcae) and 1 if all ok
*/
int readNextLine(FILE *fp, char *line);

#endif
