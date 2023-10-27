/********************************************************************
 * COPYRIGHT -- Automation Resources Group
 ********************************************************************
 * Library: CSVFileLib
 * File: csvParseLine.c
 * Author: David Blackburn
 * Created: January 06, 2012
 ********************************************************************
 * Implementation of library CSVFileLib
 ********************************************************************/

#include <bur/plctypes.h>
#ifdef __cplusplus
extern "C"
{
#endif

	#include "CSVFileLib.h"

#ifdef __cplusplus
};
#endif


#include <string.h>


/********************************************************************/
/* Parse one line of the file 										*/
/********************************************************************/

/* Check for null pointer		DONE and TESTED DFB 20120313 */

/* Check for invalid inputs		DONE and TESTED DFB 20120313 */

/* Check for comment			DONE and TESTED DFB 20120313 */

/* Get variable name			DONE and TESTED DFB 20120313 */
	/* Not found				DONE and TESTED DFB 20120313 */
	/* Found					DONE and TESTED DFB 20120313 */
	
/* Get value					DONE and TESTED DFB 20120313 */
	/* Not found				DONE and TESTED DFB 20120313 */
	/* Found					DONE and TESTED DFB 20120313 */
	
/* Get description				DONE and TESTED DFB 20120313 */
	/* Not found				DONE and TESTED DFB 20120313 */
	/* Found					DONE and TESTED DFB 20120313 */
	

unsigned short csvParseLine(unsigned long pLine, unsigned long LineNumber, struct CSVFileVariable_typ* pVariable, struct CSVFileMgr_typ* t)
{


/************************************************/
/* Check for invalid inputs						*/
/************************************************/

if( t == 0 ){
		
	return (UINT)CSV_ERR_INVALIDINPUT;
		
} // Check for null pointer //


if( 	(pLine == 0) 
	||	(pVariable == 0) 
	){
	
	csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, LineNumber, (UDINT)"csvParseLine()", t);
		
	return (UINT)CSV_ERR_INVALIDINPUT;
		
} // Check for null pointer //


/************************************************/
/* Parse line 									*/
/************************************************/

char	*pToken, *pReentry;


/* Check for commented line */

if( strncmp( (char*)pLine, "//", 2 ) == 0 ){
	
	if(t->IN.CFG.VerboseLog) csvAddLogInfo( (UINT)CSV_INFO_VERBOSE_COMMENT, LineNumber, pLine, t);
	
	return (UINT)CSV_ERR_COMMENT;
	
}
	

/* Get variable name */

pToken=	strtok_r( (char*)pLine, ",", &pReentry );


if( pToken == 0 ){

	
	/* Empty line */
	
	csvAddLogInfo( (UINT)CSV_INFO_NOVARNAMEFOUND, LineNumber, 0, t);
	
	return (UINT)CSV_ERR_NOVARNAMEFOUND;


} // No variable name found //

else{

	
	/* Remove padding and copy variable name */
	
	lstrip( (UDINT)pToken, (UDINT)CSV_PADDINGCHARS );
	rstrip( (UDINT)pToken, (UDINT)CSV_PADDINGCHARS );
	strncpy( pVariable->Name, pToken, CSV_STRLEN_VARNAME );
	if(t->IN.CFG.VerboseLog) csvAddLogInfo( (UINT)CSV_INFO_VERBOSE_VARNAME, LineNumber, (UDINT)pVariable->Name, t);
	
		
	/* Get variable value */	

	/* Start by checking if the value is inside quotes */
	char *pValueString = pReentry;	// Store the current pointer
	char *pStringStart = 0;			// Store the starting point within quotes
	char quote = 0;					// Store the type of quote we found
		
	// Go through the string until we find a null, newline, or line feed; 
	// or until we find a full, quote wrapped string
	while (*pValueString != 0 && *pValueString != '\n' && *pValueString != '\r') {
			
		// Comma found before quotes, no quotes, move on
		if (!quote && *pValueString == ',') {
			break;
		}		

		// Quotes found, start or finish?
		if (*pValueString == '\'' || *pValueString == '"') {
			
			if (!quote) {
				// First quote found
				quote = *pValueString;
				pStringStart = pValueString + 1;
			}
			else if (quote == *pValueString) {
				// End of the string
				break;
			}	
		}
			
		// Check next character
		pValueString++;	
			
	}

	/* We found a string in quotes */
	if (quote) {
		
		// Set pToken, the start of the value, to the start of the string we found
		pToken = pStringStart;
		
		// Set pReentry, the next search start point, to after the next comma
		if (*pValueString != 0) {
			
			// pValueString is not the end of the line
			// It is most likely a quote
			// Move pReentry forward one character
			pReentry = pValueString + 1;
				
			if (*pReentry == ',') {
				// If the next character is a comma, move past it since strtok won't
				pReentry++;
			}
			else {
				// If the current character is not a comma, find a comma to move past
				// If there is no comma there is no comment which is fine too
				strtok_r(0, ",\r\n", &pReentry);
			}
				
		}
		else {
			// pValueString is already at the end of the line
			// Set pReentry to the null character, as well
			pReentry = pValueString;
		}
		
		// Set the last character of the value string to null so that string 
		// copy will work
		*pValueString = 0;
		
	}
	else {
		// No quotes, handle in the historical way
		pToken = strtok_r(0, ",\r\n", &pReentry);
	}
		
	if( pToken == 0 ){
	
		
		/* No value found, corrupt line */
		
		if( t->Internal.CurrOperation == CSV_OP_OPEN ){
			csvAddLogInfo( (UINT)CSV_INFO_NOVALUEFOUND, LineNumber, (UDINT)pVariable->Name, t);
		}
		
		return (UINT)CSV_ERR_NOVALUEFOUND;
		
		
	} // No value found //
	
	else{
		
		
		/* If we have quotes, don't clean up the string	*/
		if (!quote) {

			// Remove padding
			lstrip( (UDINT)pToken, (UDINT)CSV_PADDINGCHARS );
			rstrip( (UDINT)pToken, (UDINT)CSV_PADDINGCHARS );

		}
			
		/* Copy variable value */
		strncpy( pVariable->Value, pToken, CSV_STRLEN_VARVALUE );
		if(t->IN.CFG.VerboseLog) csvAddLogInfo( (UINT)CSV_INFO_VERBOSE_VALUE, LineNumber, (UDINT)pVariable->Value, t);
		
		/* Get variable description */
		
		pToken=	strtok_r( 0, "\r\n", &pReentry);
	
		
		if( pToken == 0 ){
		
		
			/* No description found - acceptable */
			
			return 0;
		
		
		} // No description found //
		
		else{

			
			/* Remove padding and copy variable description */
			
			lstrip( (UDINT)pToken, (UDINT)CSV_PADDINGCHARS );
			rstrip( (UDINT)pToken, (UDINT)CSV_PADDINGCHARS );
			strncpy( pVariable->Description, pToken, CSV_STRLEN_VARDESCRIPTION );
			if(t->IN.CFG.VerboseLog) csvAddLogInfo( (UINT)CSV_INFO_VERBOSE_DESCRIPTION, LineNumber, (UDINT)pVariable->Description, t);
			
			return 0;
		
			
		} // Description found //
	} // Value found //
} // Name found //
		
		
} // End Fn //
