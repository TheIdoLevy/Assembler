#include <stdio.h>
#include <stdlib.h>
#include "firstScan.h"
#include "preAssembler.h"
#include "fileOperations.h"


/**
* This function is the main function, which recieves file names from stdin, opens them, and sends them 
* to pre-processing, firstScan, and secondScan
*
* @param argc the number of command line arguments
* @param argv the command line arguments
*
* @return returns 0 if no internal error occured during the assembly of any of the files passed in as input
*/
int main(int argc, char* argv[])
{
	if(argc <= 1)
		return 0;
	while(--argc > 0)
	{
		FILE *fp; 
		char *processedFile;
		fp = openFile(argc, argv[argc]);
		if(fp)
		{
			processedFile = preAssembler(fp, argv[argc]);
			if(processedFile)	
			firstScan(processedFile, argv[argc]);
		}
	}
	return 0;
}
