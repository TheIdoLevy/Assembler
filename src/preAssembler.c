/*
This file handles the pre-assembly process. It traverses a file until a macro definition is found. Macro data is inserted into a linked list, which is later traversed inorder to create a new file with the spread macros.
*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "dataStructs.h"
#include "errors.h"
#include "preAssembler.h"
#include "fileOperations.h"
#include "utils.h"
#include "validators.h"


int isMacroDefined(char *word, ptr listHead, int *mcroAddr)
{
	while(listHead && strcmp(listHead->name,word))
		listHead = (listHead)->next;
	if(!listHead)
		return 0; /* If macro not defined */
		
	mcroAddr[0] = listHead->addr1; /* Begining of macro defintion relative to src file begining (in bytes) */
	mcroAddr[1] = listHead->addr2; /* Begining of macro defintion relative to src file begining (in bytes) */

	return 1; /* Macro defintion was found */
}

int doubleDefinition(char *name, ptr listHead)
{
	while(listHead && strcmp(listHead->name,name))
		listHead = (listHead)->next;
	if(!listHead)
		return 0; 
	return 1; /* Mcro with the same name has been defined twice */	
}

void spreadMacro(FILE *src, FILE *dest, int start, int end)
{
	fpos_t p1; 
	/* i gets the number of bytes to read from src file into dest */
	int i = end - start + 1;
	char c;
	
	/* src gets the address from which to start reading from in src */
	fgetpos(src, &p1); 
	fseek(src,start,SEEK_SET);
	
	/* Write the contents of the macro in src file to dest file */
	while(i-- > 0)
	{
		c = fgetc(src);
		fputc(c,dest);
	}
	
	fsetpos(src, &p1);
}

char* preAssembler(FILE *fp, char* fileName)
{
	char inputBuffer[MAX_LINE_LEN+1];  
	char word[MAX_WORD_LEN+1]; 
	
	int i = 0, j = 0;
	
	/* Error flags - if error detected, cannot proceed to next assembling stage */
	int error = 0, mallocErr = 0; 
	
	int lineNum = 1;
	
	/* position of word in line */ 
	int wordPos = 1; 
	
	/* macro definition status */
	int mcroDef = 0, mcroDefLine = 0;
	 
	/* Currently closing a macro */
	int mcroEndLine = 0; 
	
	/* Pointer to head of linked list which keeps track of macro defintions */
	ptr head = NULL; 
	int mcroStartAddr, mcroEndAddr;
	
	char mcroName[MAX_MCRO_LEN+2]; 
	
	/* Used for second run over source file - if macro name detected, search name in list and insert start and end addrs to this aray */
	int mcroAddr[2]; 
	
	/* To add the .am extension */
	char *newFName; 
	FILE *newFilePtr; 

	for(i = 0; i < 82; i++)
		mcroName[i]= '\0';
	i=0;
		
	/* While not last line of file, read line to inputBuffer */
	while(readNextLine(fp, inputBuffer)) 
	{
		/* While not last word of current line */
		while(readNextWord(inputBuffer,word,&i)) 
		{
			/* Enters only if second or more word of line */
			if(mcroDefLine) 
			{
				/* Trailing text */
				if(wordPos == 3){ 
					printError(fileName, lineNum, ERR_CODE_2);
					error = 1;
				} else {
					/* Save macro name in the var mcroName */
					strcpy(mcroName,word); 
					if(!validMacroName(mcroName))
					{
						printError(fileName, lineNum, ERR_CODE_1);
						error = 1;	
					}
					mcroDef = 1; /* Currently definig a macro */
					mcroStartAddr = ftell(fp)/*+strlen(mcroName)*/;
				}		
			}
			
			/* If the line 'mcroend' was detected */
			if(mcroEndLine) 
			{
				/* Trailing text on mcroend line */
				if(wordPos == 2){
					printError(fileName, lineNum, ERR_CODE_3);
					error = 1;
				} 	
			}
			
			if(wordPos == 1)
			{
				/* If first word of the line is mcro */
				if(!strcmp(word, mcro)) 
					mcroDefLine = 1;
					
				if(!strcmp(word, mcroend)){
					if(mallocErr)
					{
						error = 1;
						printInternalError(ERR_CODE_7);
					} else if(!doubleDefinition(mcroName,head))
					{
						/* Calculate number of bytes between fp and
						start of line mcroend */
						j = i;
						while(inputBuffer[j] != '\0')
							j++;
						mcroDef = 0;
						mcroEndLine = 1;
						mcroEndAddr = ftell(fp)-(j-i)-9*sizeof(char);
						 
						/* Add to macro list the a node containing the
						 current macro's name, start address and end 
						 address in source file */
						addToList(&head, mcroName, mcroStartAddr,
						 mcroEndAddr, &mallocErr); 
					} else
					{
						/* Two or more macros defined with the same name */
						printError(fileName, lineNum, ERR_CODE_5);
						error = 1;
						
					}
				}
			}
			
			wordPos++;
		}
		
		/* No name declared on macro defintion line */
		if(mcroDefLine && !mcroDef) 
		{
			printError(fileName, lineNum, ERR_CODE_4);
			error = 1;
		}
		
		/* Reset flags for next line scan*/
		mcroDefLine = 0;
		mcroEndLine = 0;
		wordPos = 1;
		i = 0;
		
		lineNum++;
	}
	
	/* Create a new file with .am extension, if no error yet */
	newFName = addFileExtension(fileName, ".am", &mallocErr);
	if(mallocErr)
	{
		free(head);
		printInternalError(ERR_CODE_7);
		return NULL;
	}	
	if(error)
	{
		free(head);
		return NULL;
	}
		
	newFilePtr = fopen(newFName,"w");
		
	if(newFilePtr == NULL)
	{
		error = 1;
		printInternalError(ERR_CODE_6);
	}
	
	/* return file pointer to 0 for second pass on file */	
	fseek(fp, 0, SEEK_SET); 
	i = 0;
		
	mcroDef = 0;
	
	/* While not last line of file */
	while(!error && readNextLine(fp, inputBuffer)) 
	{
		/* While not last word of current line */
		while(readNextWord(inputBuffer,word,&i)) 
		{
			if(!strcmp(word, mcro))
				mcroDef = 1;
				
			
			/* spread macro if macro is called */		
			if(!mcroDef && isMacroDefined(word,head,mcroAddr)) 
				spreadMacro(fp, newFilePtr, mcroAddr[0], mcroAddr[1]);
			else if(!mcroDef && strcmp(word, mcroend))
			{
				/* Read into file any line that does not contain macro name or mcro/mcroend*/
				fputs(inputBuffer, newFilePtr);
				fputc('\n', newFilePtr);
				break;
			}
				
			if(!strcmp(word, mcroend))
				mcroDef = 0;
		}
		i = 0;
	}

	if(head!=NULL)
		free(head);
	if(error)
		return NULL;
	fclose(newFilePtr);
	return newFName;
}
