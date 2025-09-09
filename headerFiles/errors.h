#ifndef ERRORS_H
#define ERRORS_H

/**
Represents the various errors that can occur during assembly
*/
typedef enum ERROR_CODES {
	ERR_CODE_0 = 0,
	ERR_CODE_1,
	ERR_CODE_2,
	ERR_CODE_3,
	ERR_CODE_4,
	ERR_CODE_5,
	ERR_CODE_6,
	ERR_CODE_7,
	ERR_CODE_8,
	ERR_CODE_9,
	ERR_CODE_10,
	ERR_CODE_11,
	ERR_CODE_12,
	ERR_CODE_13,
	ERR_CODE_14,
	ERR_CODE_15,
	ERR_CODE_16,
	ERR_CODE_17,
	ERR_CODE_18,
	ERR_CODE_19,
	ERR_CODE_20,
	ERR_CODE_21,
	ERR_CODE_22,
	ERR_CODE_23,
	ERR_CODE_24,
	ERR_CODE_25,
	ERR_CODE_26,
	ERR_CODE_27,
	ERR_CODE_28,
	ERR_CODE_29,
	ERR_CODE_30,
	ERR_CODE_31,
	ERR_CODE_32,
	ERR_CODE_33
} ERROR_CODES;


/**
Represents an error, where:
errNum: The number of the error
errMsg: The message of the error
*/
typedef struct Error {
	int errNum;
	char *errMsg;
} Error;


/**
This functions prints an error (syntax and label based error) based on its three parameters;
@param fileName The name of the file in which the error has occured
@param lineNum The number of the line in which the error has occured
@param errCode the error message is printed based on error number [errCode] in ERR_CODES
*/
void printError(char *fileName, int lineNum, int errCode);


/**
This function outputs an error when an internal memory has occured (e.g failed mem allocation, failed to open file, etc
@param errCode the err to print with code errCode
*/
void printInternalError(int errCode);


/**
This function outputs errors related to symbol being defined as .entry but not actually defined in the given file;
@param fileName The name of the file in which the error has occured
@param symbolName The name of the symbol which caused the error
@param errCode the error message is printed based on error number [errCode] in ERR_CODES
*/
void printSymbolError(char *fileName, char * symbolName, int errCode);


#endif
