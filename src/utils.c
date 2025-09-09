/*
This file includes general purpose helper funnctions, include number base convertes, isInt, and readNextWord
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

int isInt(char *num)
{
	if(num =='\0')
		return 0;

	/* Can start with a plus or minus */
	if(num[0] == '+' || num[0] == '-')
		num++;

	if(*num == '\0')
		return 0;
		
	while(*num != '\0')
	{
		
		if(!isdigit(*num))
			return 0;
		num++;
	}
	
	/* 1 for valid, 0 for invalid */
	return 1;
}

char* decToBin(int n, int end)
{
	/* create a string of length end, containing binary n (2's complement) */
	unsigned int mask = (1U << end) - 1;
	unsigned int val = n & mask;
	int i;

	char *bin = (char *)malloc(end + 1);
	if(!bin);
	
	for(i = end - 1; i >= 0; i--)
	{
		bin[i] = (val & 1) ? '1' : '0';
		val >>= 1;
	}
	
	bin[end] = '\0';
	return bin;
}

char *binToHex(char *bin)
{
	/* Returns a 6 digit hex */
	char* hex = malloc(7*sizeof(char));
	unsigned int dec = 0;
	int i = 0;
	for( ; i < 24; i++)
		 dec = (dec << 1) | (bin[i] - '0');
	sprintf(hex, "%06x", dec);
	return hex;
}

int readNextWord(char* line, char* word, int* idx)
{
	int i = *idx, j = 0, temp;
	word[MAX_WORD_LEN] = '\0'; 

	if(line[i] == '\0')
		return 0; /* Empty string*/
		
	while( line[i] != '\0' && isspace(line[i]) )
		i++; /* inc i until a non-whitespace char is reached */
		
	temp = i;	
	/* Copy contents of next word in line into word */
	while( !isspace(line[i]) && line[i] != '\0' && j <= MAX_WORD_LEN)
		word[j++] = line[i++];	
	
	if(temp == i)
		return 0; /* Empty string */
		
	word[j] = '\0';
	*idx = i;
	
	return 1; /* 0 for End of Line */
}

int readNextWordC(char* line, char* word, int* idx, char stopC) 
{
	int i = *idx, j = 0;
	int stop = 0;
	word[MAX_WORD_LEN] = '\0';

	if(line[i] == '\0')
		return 0;
	
	/* Bring counter i to index of the next word in line*/	
	while(!stop && ((line[i] != '\0' && isspace(line[i]) && line[i]) || (line[i] == stopC)))
	{
		if(line[i] == stopC)
			stop = 1;
		i++;
	}


	/* reads from line untill encounters char stopC */
	while( line[i] != stopC && !isspace(line[i]) && line[i] != '\0')
	{
		if(line[i] == stopC)
			stop = 1;
		/* Copy contents of next word in line into word */
		word[j++] = line[i++];	
	}
	
	if(stop)
		--i;	
	word[j] = '\0';
	*idx = i;
	

	if(isspace(word[0]) || word[0] == '\0')
		return 0; /* 0 for End of Line */
		
	return 1; 
}

int readComma(char *line, int *i) 
{
	/* If coma encountered, returns 1 */
	char comma = ' ';
	while(line[*i] != '\0' && isspace(comma))
	{
		comma = line[(*i)++];
	}
		
	if(comma == ',')
		return 1;
		
	return 0;	
}
