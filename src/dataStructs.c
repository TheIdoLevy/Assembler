/*
This file includes the definitions of the data structures that will be used during the program:
A linked list: for the macro definitions
A hash table: to hold the symbol table
*/


#include <stdlib.h>
#include <string.h>
#include "dataStructs.h"
#include "errors.h"

/* ---------------- LINKED LIST ---------------- */
void addToList(ptr *headPtr, char *mcroName, int addr1, int addr2, int *mallocErr)
{
	ptr t;
	t = (ptr) malloc(sizeof(item));
	if(!t)
	{
		*mallocErr = 1;
		return;
	}
	strcpy(t->name,mcroName);
	t->addr1 = addr1;
	t->addr2 = addr2;
	t->next = *headPtr;
	*headPtr = t;
}

void freeList(ptr *headPtr)
{
	ptr p;
	while(*headPtr != NULL)
	{
		p = *headPtr;
		*headPtr = (*headPtr)->next;
		free(p);
	}
}

/* ---------------- HASH TABLE ---------------- */

unsigned int hash(char* key, int size)
{
	/* Uses basic string hashing */
	unsigned int hash = 0;
	while(*key)
		hash = (hash * 31) + *key++;
	return (size > 0) ? hash % size : 0;
}

Table *createTable(int size, int *mallocError)
{
	Table *table = (Table *)malloc(sizeof(Table));
	if(!table)
	{
		*mallocError = 1;
		return NULL;
	}

	/* Initiallize table fields */
	table->size = size;
	table->count = 0;
	table->entries = (Entry *)calloc(size, sizeof(Entry));
	return table;
}

void resize(Table *table){

	/* new size of table */
	int newSize = table->size*2;
	
	int i;
	unsigned int idx;
	
	Entry *newEntries = (Entry *)calloc(newSize, sizeof(Entry));
	
	for(i = 0; i < table->size; i++)
	{
		/* Re insert existing entries */
		idx = hash(table->entries[i].symbolName, newSize);
		while(newEntries[idx].full)
			idx = (idx + 1) % newSize;
		newEntries[idx] = table->entries[i];
	}
	
	free(table->entries);
	table->entries = newEntries;
	table->size = newSize;
}

void insert(Table *table, char *name, int address, char type, char *fName, int lineNum, int *error) 
{
	/* Uses linear probing */
	unsigned int index;

	/* Load factor exceeded, resize table */
	if((double)table->count / table->size > LOAD_FACTOR)
		resize(table);
		
	index = hash(name, table->size);
	
	while(table->entries[index].full)
	{
		if(!strcmp(table->entries[index].symbolName, name))
		{
			/* Update type of symbols that have been defined as .entry */
			if(type == 'e')
			{
			/* Will enter only of this symbol was already defined.
			 In addition to previous definition, adds type .entry */
			 
			 	/* label can't be extern and entry */
			 	if(table->entries[index].symbolType[0] == 'x')
			 	{
			 		*error = 1;
			 		printError(fName, lineNum, ERR_CODE_27);
			 	}
			 	

			 	if(table->entries[index].symbolType[0] != 'e')
					table->entries[index].symbolType[1] = 'e'; 
					
				return;		
			}
			else if(table->entries[index].symbolType[0] == 'e')
			{
			/* Will enter if the symbol was defined as .entry before the actual definiton
			of the symbol. Adds the other typr of the data (code/data) */
			
			 	if(type == 'x')
			 	{
			 		/* Label can't be extern and entry */
			 		*error = 1;
			 		printError(fName, lineNum, ERR_CODE_28); 
			 	}
			 	
			 	if(type != 'e')
			 	{
			 		if(table->entries[index].symbolType[1] == 'd' ||
			 		   table->entries[index].symbolType[1] == 'c')
			 		{
						*error = 1;
			 			printError(fName, lineNum, ERR_CODE_26);
			 		} else
			 		{
						table->entries[index].symbolType[1] =  type;
						table->entries[index].addr = address;
					}
				}
				return;
			}
			else 
			{
				
				*error = 1;
			 	printError(fName, lineNum, ERR_CODE_26);
			}
		}
		
		index = (index+1)% table->size;
	}
	
	/* Will get to this part only if symbol hasn't been defined yet */
	strncpy(table->entries[index].symbolName, name, 30);
	
	table->entries[index].symbolName[29] = '\0';
	table->entries[index].symbolType[0] =  type;
	
	if(type == 'e' || type == 'x')
		table->entries[index].addr = 0;
	else
		table->entries[index].addr = address;
	
	/* Mark as occupied */	
	table->entries[index].full = 1;
	
	/* Extra element added to table, inc size*/
	table->count = table->count + 1;
}


int get(Table *table, char *name, char field)
{
	/* field indicates which field to retrieve: type or address */
	int idx = hash(name, table->size);
	char type[3];
	while(table->entries[idx].full)
	{			
		if(!strcmp(table->entries[idx].symbolName, name))
		{
			if(field == 't')/*Type*/
			{	
				/* Return the symbol type */
				strcpy(type, table->entries[idx].symbolType);
				if(!strcmp(type,"c"))
					return CODE;
				if(!strcmp(type,"d"))
					return DATA;
				if(!strcmp(type,"e"))
					return ENTRY;
				if(!strcmp(type,"x"))
					return EXTERN;
				if(!strcmp(type,"ec") || !strcmp(type,"ce"))
					return ENT_CODE;
				if(!strcmp(type,"xc") || !strcmp(type,"cx"))
					return EXT_CODE;
				if(!strcmp(type,"ed") || !strcmp(type,"de"))
					return ENT_DATA;
				if(!strcmp(type,"xd") || !strcmp(type,"dx"))
					return EXT_DATA;					
			}
			
			if(table->entries[idx].symbolType[0] == 'x' ||
			table->entries[idx].symbolType[1] == 'x')
				return 1; /* Because extern returns 0, and need to check if label exists. */
			return table->entries[idx].addr; /* data labels get ic+100 so won't get 0 */
		}
		
		idx = (idx+1) % table->size;
	}

	return 0;
}

void freeTable(Table* table)
{
	free(table->entries);
	free(table);
}
