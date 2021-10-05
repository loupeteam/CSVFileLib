#include "CSVFileLib.h"
#include <cstdio>
#include <cstring>

unsigned long bur_heap_size = 0xFFFFF;

#ifdef __cplusplus
extern "C" {
#endif


/*
*	double to string conversion function
*	wraps sprintf for use in c
*
*	@param - value - double to be converted to a string
*	@param - pString - pointer to string that will hold conversion output
*			 assumed to be 50 characters long (CSVFileLib: CSV_STRLEN_VARVALUE)
*	
*	@return - pString
*
*	resulting string has 32 sigfig precision
*	in case of sprintf error, write error message to string
*/
unsigned long csvDTOA(double value, unsigned long pString){
	
	int len = 0;
	
	if(!pString){return pString;}
	
	len = sprintf((char*)pString, "%.32g", value);
	
	if(len < 0){
		strcpy((char*)pString, "error: dtoa conversion failed");	
	}
	
	return pString;
	
}

#ifdef __cplusplus
}
#endif