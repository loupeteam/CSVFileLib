/********************************************************************
 * COPYRIGHT -- Automation Resources Group
 ********************************************************************
 * Library: CSVFileLib
 * File: csvInitBuffer.c
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
/* Initialize a buffer												*/
/********************************************************************/

/* Check for invalid inputs		DONE and TESTED DFB 20120405 */
/* Allocate buffer				DONE and TESTED DFB 20120405 */
/* Set Buffer.MaxLength			DONE and TESTED DFB 20120405 */
/* Clear buffer					DONE and TESTED DFB 20120405 */


unsigned short csvInitBuffer(unsigned long BufferLength, struct CSVFileMgr_Int_Buffer_typ* pBuffer)
{

	
/************************************************/
/* Check for invalid inputs						*/
/************************************************/

if( 	(BufferLength == 0)
	||	(pBuffer == 0)
	){
		
	return (UINT)CSV_ERR_INVALIDINPUT;
		
} // Check for null pointer //


/************************************************/
/* Initialize buffer							*/
/************************************************/

memset( pBuffer, 0, sizeof(CSVFileMgr_Int_Buffer_typ) );

if( TMP_alloc( BufferLength, (void **)&(pBuffer->pData) ) != 0 ) return CSV_ERR_MEMALLOC;
	
pBuffer->MaxLength=	BufferLength;

csvClearBuffer(pBuffer);

return 0;


} // End Fn //
