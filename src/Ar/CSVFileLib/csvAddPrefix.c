/********************************************************************
 * COPYRIGHT -- Automation Resources Group
 ********************************************************************
 * Library: CSVFileLib
 * File: csvAddPrefix.c
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
/* Add a prefix to a string, unconditionally 						*/
/********************************************************************/

/* Check for null pointers				DONE and TESTED DFB 20120418 */
/* Check for empty prefix				DONE and TESTED DFB 20120418 */
/* Add prefix							DONE and TESTED DFB 20120420 */


unsigned short csvAddPrefix(unsigned long MaxLength, plcstring* String, plcstring* Prefix)
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
	/* Add prefix									*/
	/************************************************/

	STRING	TempString[MaxLength + 1];
	memset( TempString, 0, sizeof(TempString) );

	strncpy( TempString, Prefix, MaxLength );
	if( *String != 0 ) {
		strncat( TempString, ".", MaxLength - strlen(TempString) );
		strncat( TempString, String, MaxLength - strlen(TempString) );
	}
	strcpy( String, TempString );


	return 0 ;


} // End Fn //
