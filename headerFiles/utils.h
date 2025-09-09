#ifndef UTILS_h
#define UTILS_h

#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 31

/**
* Given a line and an index in that line, this functions reads the next word after index idx into a word array. A 
* word ends when a whitespace or '\n' or '\0' is reached
*
* @param line the line from which to read a word from
* @param word the string into which to copy the next word into
* @param idx the index in the line from which to start reading from
*
* @return 0 if the end of the line was reached, and 1 otherwise
*/
int readNextWord(char* line, char* word, int* idx);


/**
* Given a line, an index in that line and a char stopC, this functions reads characters into word until stopC is
* encountered or until a whitespace or end of line is encountered
*
* @param line the line from which to read a word from
* @param word the string into which to copy the next word into
* @param idx the index in the line from which to start reading from
* @param stopC the character which, when encountered, the function stops reading chars into word
*
* @return 0 if the end of the line was reached, and 1 otherwise
*/
int readNextWordC(char* line, char* word, int* idx, char stopC);


/**
* Given a line and an index i, this function searches for an appearence of a comma ',' starting from index i 
* in that line
*
* @param line the line in which a ',' is searched for
* @param i the index in the line from which to start searching for a ','
*
* @return returns 1 if a coma was encountered in the line after index i, and 0 otherwise
*/
int readComma(char *line, int *i);


/**
* Given an int n and int end, this function turns n into a string of length end, which conatins the 2's complement
* binary representation of n
*
* @param n the number which will be "transformed" to binary
* @param end the length of the string which will represent the binary n
*
* @return the string containing the binary representation of n, with [end] bits 
*/
char* decToBin(int n, int end);


/**
* Given a string representing an int on a .data definition, this function validates the syntax of int
*
* @param num the int (string representing an int) to be validated
*
* @return returns 1 if the syntax of num is valid, and 0 otherwise
*/
int isInt(char *num);


/**
* Given a string representing a binary number, this function returns a string that is the hex representation of
* the binary number
*
* @param bin the binary number to be converted to base hexadecimal
*
* @return a string containing the hex representation of the int represented in int
*/
char *binToHex(char *bin);

#endif
