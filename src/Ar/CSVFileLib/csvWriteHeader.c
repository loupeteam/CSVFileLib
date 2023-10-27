/*
 * File: csvWriteHeader.c
 * Copyright (c) 2023 Loupe
 * https://loupe.team
 * 
 * This file is part of CSVFileLib, licensed under the MIT License.
 * 
 */

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
/* Write the header to the file 									*/
/********************************************************************/

/* Check for null pointer		DONE and TESTED DFB 20120313 */

/* Check for initialized		DONE and TESTED DFB 20120406 */

/* Generate header				DONE and TESTED DFB 20150624 */

/* Copy to write buffer			DONE and TESTED DFB 20120406 */
	/* Error					DONE and TESTED DFB 20120406 */
	/* No error					DONE and TESTED DFB 20120406 */
	

unsigned short csvWriteHeader(struct CSVFileMgr_typ* t)
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
	
		return (UINT)CSV_ERR_NOTINITIALIZED;
	
	} /* Check for initialized */


	/************************************************/
	/* Write Header									*/
	/************************************************/

	STRING			Header[CSV_STRLEN_HEADER+1],
	DTString[24+1];

	DTGetTime_typ	DTGetTime_0;


	memset( Header, 0, sizeof(Header) );
	memset( DTString, 0, sizeof(DTString) );
	memset( &DTGetTime_0, 0, sizeof(DTGetTime_0) );


	/* Get current date and time into DTString */

	DTGetTime_0.enable=	1;

	DTGetTime(&DTGetTime_0);

	ascDT( DTGetTime_0.DT1, (UDINT)DTString, sizeof(DTString) );


	/* Generate header */

	strcpy( Header, "// File generated " );
	strcat( Header, DTString );
	strcat( Header, "\r\n" );
	
	UDINT pRevision, pConfiguration, data_len;
	
	strcat(Header, "// Application Revision: ");
	if( PV_xgetadr("revision", &pRevision, &data_len) == 0 ){ 
		strcat(Header, (char*)pRevision);
	}
	else if( PV_xgetadr("buildRevision", &pRevision, &data_len) == 0 ){ 
		strcat(Header, (char*)pRevision);
	}
	else{
		strcat(Header, "unknown");
	}
	strcat(Header, "\r\n");

	
	strcat(Header, "// Configuration: ");
	if( PV_xgetadr("buildConfiguration", &pConfiguration, &data_len) == 0 ){
		strcat(Header, (char*)pConfiguration);
	}
	else{
		strcat(Header, "unknown");
	}
	strcat(Header, "\r\n");
		
	
	/* Add to write buffer and check for errors */

	UINT	AddStatus;
				
	AddStatus=	csvAddToBuffer( (UDINT)Header, strlen(Header), &(t->Internal.WriteBuffer) );


	if( AddStatus == CSV_ERR_INVALIDINPUT ){

		csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, 0, (UDINT)"csvAddToBuffer()", t);
	
	}
	else if( AddStatus == CSV_ERR_BUFFERFULL ){

		csvAddLogInfo( (UINT)CSV_INFO_FILEFULL, 0, 0, t);
	
	}


	return AddStatus;


	///* Copy to pWriteData if there is room */
	//
	//if( ( strlen((char*)t->Internal.pWriteData) + strlen(Header) ) <= t->IN.CFG.MaxFileSize ){
	//	
	//	strcat( (char*)t->Internal.pWriteData, Header );
	//	
	//	return 0;
	//
	//}
	//else{
	//
	//	csvAddLogInfo( (UINT)CSV_INFO_FILEFULL, 0, 0, t);
	//	return CSV_ERR_BUFFERFULL;
	//
	//}


} // End Fn //
