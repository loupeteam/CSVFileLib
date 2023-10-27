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
*	@param - numSigFigs - number of significant figures to retain while printing to string
*	
*	@return - pString
*
*	resulting string has 32 sigfig precision
*	in case of sprintf error, write error message to string
*/
unsigned long csvDTOA(double value, unsigned long pString, unsigned char numSigFigs){
	
	int len = 0;
	char formatString[8] = "%."; // printf format string should look like: "%.XXg"
	char numsigFigsString[3];
	
	if(!pString){return pString;}
	
	// Through trial and error I found max value is about 51 sig figs. 
	// More than 16 appears to have accuracy issues
	// Not sure why that is the limit, the value is not accurate that high.
	// Limit to less than 100 so we know how long the string will be 
	if(numSigFigs > 99) numSigFigs = 99;
  
	brsitoa(numSigFigs, (UDINT)&numsigFigsString);
  
	strcat(formatString, numsigFigsString);
	strcat(formatString, "g");
	
	len = sprintf((char*)pString, formatString, value);
	
	if(len < 0){
		strcpy((char*)pString, "error: dtoa conversion failed");	
	}
	
	return pString;
	
}

#ifdef __cplusplus
}
#endif