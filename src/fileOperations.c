/*
This file handles basic file operations - opening a new file, adding an extension to an existing file name, opening an existing file, and reading the next line from a file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileOperations.h"
#include "errors.h"

FILE *openFile(int argc, char* argv)
{

	FILE *fd;
	char *newFName;
	int mallocErr = 0;
	
	/* Open the file with a .as extension (file name supplied without) */	
	newFName = addFileExtension(argv, ".as", &mallocErr);
	if(!(fd = fopen(newFName,"r")))
	{
		printInternalError(ERR_CODE_32);
		free(newFName);
		return NULL;
	}

	free(newFName);
	if(mallocErr)
	{
		printInternalError(ERR_CODE_7);
		exit(1);
	}
	return fd;
}

char* addFileExtension(char *fileName, char *extension, int *mallocErr)
{
	/* Create new file name with new extension*/
	char *newFName;
	newFName = (char *)malloc(sizeof(char)*(strlen(fileName)+strlen(extension)+1));
	
	if(!newFName)
	{
		*mallocErr = 1;
		return NULL;
	}
	
	strcpy(newFName,fileName);
	strcat(newFName,extension);
	return newFName;
}

FILE *openNewFileExtension(char *fileName, char *extension, char permission)
{
	FILE *file;
	char *newFName;
	
	/* Open a new file with a new extension */
	newFName = (char *)malloc(sizeof(char)*(strlen(fileName)+4));
	strcpy(newFName,fileName);
	strcat(newFName,extension);
	
	/* param permission indicates which permission to give */
	if(permission == 'w')
		file = fopen(newFName,"w");
	else
		file = fopen(newFName,"r+");
	
	free(newFName);
	
		
	if(!file)
	{
		printInternalError(ERR_CODE_33);
		return NULL;
	}

	return file;
}

int readNextLine(FILE *fp, char *line)
{
	/* set with '\0' */
	memset(line, '\0', strlen(line)); 
	
	if(fgets(line, MAX_LINE_LEN+2, fp))
	{
		line[strcspn(line, "\n")] = '\0';
		
		/* -1 indicates that line exceeds maximum of 81 chars*/
		if(strlen(line) > MAX_LINE_LEN)
			return -1;
			
		return 1;
	}
	
	/* End of line reched */
	return 0;
}
