/********************************************************************
 * COPYRIGHT -- Automation Resources Group
 ********************************************************************
 * Library: CSVFileLib
 * File: csvClearBuffer.c
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
/* Clear a buffer 													*/
/********************************************************************/

/* Check for null pointer		DONE and TESTED DFB 20120405 */
/* Clear buffer					DONE and TESTED DFB 20120405 */


unsigned short csvClearBuffer(struct CSVFileMgr_Int_Buffer_typ* pBuffer)
{


/************************************************/
/* Check for invalid inputs						*/
/************************************************/

if( pBuffer == 0 ){
		
	return (UINT)CSV_ERR_INVALIDINPUT;
		
} // Check for null pointer //


/************************************************/
/* Clear buffer									*/
/************************************************/

pBuffer->CurrentLength=	0;

memset( (void*)pBuffer->pData, 0, pBuffer->MaxLength );

return 0;


} // End Fn //
