/********************************************************************
 * COPYRIGHT -- Automation Resources Group
 ********************************************************************
 * Library: CSVFileLib
 * File: CSVOpenFile_Init.c
 * Author: David Blackburn
 * Created: July 30, 2013
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
/* Open a file in an INIT routine 									*/
/********************************************************************/


/* Check for null pointer					DONE and TESTED DFB 20130730 */

/* Check for initialized					DONE and TESTED DFB 20130730 */

/* Open file								DONE and TESTED DFB 20130730 */
	/* Done									DONE and TESTED DFB 20130730 */
	/* Error								DONE and TESTED DFB 20130730 */


unsigned short CSVOpenFile_Init(struct CSVFileMgr_typ* t)
{


/************************************************/
/* Check for invalid inputs						*/
/************************************************/

if( t == 0 ){
		
	return (UINT)CSV_ERR_INVALIDINPUT;
		
} // Check for null pointer //


/************************************************/
/* Check for initialized						*/
/************************************************/

if(		(t->Internal.Initialized == 0)
	||	(t->Internal.ReadBuffer.pData == 0)
	||	(t->Internal.WriteBuffer.pData == 0)
	||	(t->Internal.LogBuffer.pData == 0)
	||	(t->Internal.pLine == 0)
	){
	
	t->Internal.Initialized=	0;
	
	csvSetError( (UINT)CSV_ERR_NOTINITIALIZED, t );
	
	return (UINT)CSV_ERR_NOTINITIALIZED;
	
} /* Check for initialized */


/************************************************/
/* Open a file									*/
/************************************************/

while(1){

	t->IN.CMD.OpenFile=	1;
	CSVFn_Cyclic(t);
	
	if( t->OUT.STAT.Error ){
		t->IN.CMD.OpenFile=	0;
		CSVFn_Cyclic(t);
		return t->OUT.STAT.ErrorID;
	}
	else if( t->OUT.STAT.Done ){
		t->IN.CMD.OpenFile=	0;
		CSVFn_Cyclic(t);
		return 0;
	}
	
} // Infinite loop to call CSVFn_Cyclic //


} // End Fn //
