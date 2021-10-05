/********************************************************************
 * COPYRIGHT -- Automation Resources Group
 ********************************************************************
 * Library: CSVFileLib
 * File: csvAddToBuffer.c
 * Author: David Blackburn
 * Created: April 05, 2012
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
/* Add data to the end of a buffer, checking for overflow			*/
/********************************************************************/

/* Check for invalid inputs		DONE and TESTED DFB 20120405 */
/* Check for overflow			DONE and TESTED DFB 20120405 */
/* Copy data					DONE and TESTED DFB 20120405 */
/* Update CurrentLength			DONE and TESTED DFB 20120405 */


unsigned short csvAddToBuffer(unsigned long pData, unsigned long DataLength, struct CSVFileMgr_Int_Buffer_typ* pBuffer)
{


/************************************************/
/* Check for invalid inputs						*/
/************************************************/

if( 	(pData == 0)
	||	(pBuffer == 0)
	){
		
	return (UINT)CSV_ERR_INVALIDINPUT;
		
} // Check for null pointer //


if( pBuffer->pData == 0 ){
	
	return (UINT)CSV_ERR_INVALIDINPUT;
		
} // Check for null pointer //


/************************************************/
/* Add data to buffer if there is room			*/
/************************************************/

if( (pBuffer->CurrentLength + DataLength) > pBuffer->MaxLength ) return CSV_ERR_BUFFERFULL;

memcpy( (void*)(pBuffer->pData + pBuffer->CurrentLength), (void*)pData, DataLength );

pBuffer->CurrentLength+=	DataLength;

return 0;


} // End Fn //
