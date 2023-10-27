/********************************************************************
 * COPYRIGHT -- Automation Resources Group
 ********************************************************************
 * Library: CSVFileLib
 * File: csvRemovePrefix.c
 * Author: David Blackburn
 * Created: April 18, 2012
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
/* Remove the prefix from the string if necessary 					*/
/********************************************************************/

/* Check for null pointers				DONE and TESTED DFB 20120418 */

/* Check for empty prefix				DONE and TESTED DFB 20120418 */

/* Non-matching prefix					DONE and TESTED DFB 20120420 */

/* Matching prefix						DONE and TESTED DFB 20120420 */
	/* Structure						DONE and TESTED DFB 20120420 */
	/* Not a structure					DONE and TESTED DFB 20120420 */



unsigned short csvRemovePrefix(unsigned long MaxLength, plcstring* String, plcstring* Prefix)
{


/************************************************/
/* Check for invalid inputs						*/
/************************************************/

if( 	(String == 0)
	||	(Prefix == 0)
	){
		
	return (UINT)CSV_ERR_INVALIDINPUT;
		
} // Check for null pointer //


/************************************************/
/* If prefix is empty, return immediately		*/
/************************************************/

if( *Prefix == 0 ) return 0;


/************************************************/
/* Remove prefix if necessary					*/
/************************************************/

UDINT	TempLength;


/* Check beginning against prefix */

TempLength=	strlen(Prefix);

if( strncmp( String, Prefix, TempLength ) == 0 ){
	
	
	/* Check for '.' after prefix */
	
	if( String[TempLength] == '.' ){

		STRING	TempString[MaxLength + 1];
		memset( TempString, 0, sizeof(TempString) );
	
		strncpy( TempString, String + TempLength + 1, MaxLength );
	
		strcpy( String, TempString );
		
	}
	else{
		
		/* Prefix matches, but it is not a structure */
		
		return CSV_ERR_PREFIXNOTSTRUCTURE;
		
	}
	
	
}


return 0;


} // End Fn //
