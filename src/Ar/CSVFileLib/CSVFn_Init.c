/*
 * File: CSVFn_Init.c
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
/* CSVFileMgr initialization function 								*/
/********************************************************************/

/* Check for null pointer		DONE and TESTED DFB 20130627 */

/* Clear Internal and OUT		DONE and TESTED DFB 20130627 */

/* Check CFG values				DONE and TESTED DFB 20131220 */

/* Allocate memory				DONE and TESTED DFB 20130627 */
	/* Error					DONE and TESTED DFB 20130627 */
	/* No error					DONE and TESTED DFB 20130627 */
	

unsigned short CSVFn_Init(struct CSVFileMgr_typ* t)
{


/************************************************/
/* Check for invalid inputs						*/
/************************************************/

if( t == 0 ){
		
	return (UINT)CSV_ERR_INVALIDINPUT;
		
} // Check for null pointer //


/************************************************/
/* Clear CSVFileMgr internals and outputs		*/
/************************************************/

memset( &(t->Internal),	0,	sizeof(CSVFileMgr_Internal_typ) );
memset( &(t->OUT),		0,	sizeof(CSVFileMgr_OUT_typ) );


/************************************************/
/* Check for invalid CFG data					*/
/************************************************/

if( t->IN.CFG.MaxFileSize == 0 )		t->IN.CFG.MaxFileSize=		CSV_DEFAULT_MAXFILESIZE;
if( t->IN.CFG.MaxLineLength == 0 )		t->IN.CFG.MaxLineLength=	CSV_DEFAULT_MAXLINELENGTH;
if( t->IN.CFG.MaxLinesPerScan == 0 )	t->IN.CFG.MaxLinesPerScan=	CSV_DEFAULT_MAXLINESPERSCAN;

if( !t->IN.CFG.DisableLogging )
{
	if( strcmp(t->IN.CFG.LoggerName, "") == 0 )	strcpy( t->IN.CFG.LoggerName, CSV_DEFAULT_LOGGERNAME);
}
	
if( t->IN.CFG.DoubleSigFigs == 0 ) {
	t->IN.CFG.DoubleSigFigs = CSV_DEFAULT_SIGFIGS;
}
	

/************************************************/
/* Initialize buffers							*/
/************************************************/

if( 	(csvInitBuffer(	t->IN.CFG.MaxFileSize,		&(t->Internal.ReadBuffer) 		) != 0)
	||	(csvInitBuffer(	t->IN.CFG.MaxFileSize,		&(t->Internal.WriteBuffer) 		) != 0)
	||	(csvInitBuffer(	t->IN.CFG.MaxFileSize,		&(t->Internal.LogBuffer) 		) != 0)
	||	(TMP_alloc( 	t->IN.CFG.MaxLineLength+1,	(void **)&(t->Internal.pLine)	) != 0)
	){

	csvSetError( (UINT)CSV_ERR_MEMALLOC, t );
	
	t->Internal.Initialized=	0;

	return CSV_ERR_MEMALLOC;

}

/* Clear line buffer */

memset( (void*)t->Internal.pLine, 0, t->IN.CFG.MaxLineLength+1 );


// DONE //

t->Internal.Initialized=		1;
		
return 0;


} // End Fn //
