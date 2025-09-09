#ifndef DATASTRUCTS_h
#define DATASTRUCTS_h

#include <stdio.h>
#include <stdlib.h>

#define LOAD_FACTOR 0.75

#define CODE 1
#define DATA 2
#define ENTRY 3
#define EXTERN 4
#define ENT_CODE 5
#define EXT_CODE 6
#define ENT_DATA 7
#define EXT_DATA 8

/* This is a ptr to the head of the linked list, which will contain macro data for pre assmebler*/
typedef struct node * ptr;


/**
 A node in the linke list used to represent macro data in the pre assmembler phase
name: name of the macro being defined
addr1: The offset from the start of the, in bytes, to the macro definition
addr2: The offset from the start of the, in bytes, to the mcroend statemrnt
next: pointer to the next node int linked list
*/
typedef struct node {
	char name[81];
	int addr1;
	int addr2;
	ptr next;
} item; /* Node in list */


/**
 A entry in a hash table. These are the entries if the symbol table 
SymbolName: The name of the symbol defined
SymbolType: This holds the type of the symbol. It can represent code, data, extern, entry,
or a combination. 
Addr: The address of the label deifinition (0 if .extern)
full: signifies wether this entry is currently occupied
*/
typedef struct Entry
{
	char symbolName[32]; 
	char symbolType[3];
	/*unsigned*/int addr;
	int full;
} Entry;


/**
 This is ta hash table, which holds the symbol table
entries: Entries, as define above, to the table
count: Number of entries currently in the table
size: Current size of the table ******
*/
typedef struct Table
{
	Entry *entries;
	int count;
	int size;
} Table;


/**
This function inserts a new macro as the head of the linked list containiting all macro definitions.
It initializes the files of node (defined above) with the supllied parameters
@param headPtr pointer to head of the linke list containg all macro definitions
@param mcroName the name of the macro that is currently been defined
@param addr1 the offset in bytes from the begining of current file to the macro definition line
@param addr2 the offset in bytes from the begining of current file to the macro end line
@param mallocErro passed in as an argument from other functions in order to detect memory allocation errors
*/
void addToList(ptr *headPtr, char *mcroName, int addr1, int addr2, int *mallocErr);


/**
Frees the linked list, who's head is pointed to by ptr
@param the head of the linked list which whill be deleted
*/
void freeList(ptr*);


/**
Given a key (a symbol name) and the current size of the symbol table, this function hashes the key and returns an index in the symbol table in which to insert the symbol
@param key the name of the symbol which will be hashed
@param size the current size of the symbol table
@return an index in the symbol table in which to insert the symbol data (based on the hashing of the symbol name)
*/
unsigned int hash(char* key, int size);

/**
Creates a new hash table, which represents the symbol table
@param size the initial size of the hash table
@param mallocError used by other functions to detect memory allocation errors
@return an initialized hash table
*/
Table *createTable(int size, int *mallocError);


/**
This function inserts a symbol into the symbol table, and checks that no other label with the same name has been defined (or impoorted as .extern) withtin the same file
@param table the symbol table into which the label is inserted
@param name the name if the symbol which is inserted into the table
@param address the address of the definition of the symbol (if defines as .extern, then address=0)
@param type the type of the symbol: data, code, entry, extern, or a cetain combination
@param fName the name of the source file
@param lineNum the line number in which this symbol is deinfed. Used in order to report errors
@param error used by other functions to detect errors in this phase of the assembly
*/
void insert(Table *table, char *name, int address, char type, char *fName, int lineNum, int *error);


int get(Table *table, char *name, char field);

/**
This functions frees the memory allocated by the symbol table
@param table the table which to free
*/
void freeTable(Table*);

#endif
