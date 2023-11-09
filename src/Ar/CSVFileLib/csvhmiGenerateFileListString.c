/*
 * File: csvhmiGenerateFileListString.c
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
/* Generate the file list string for a given file					*/
/********************************************************************/

/* Check for null pointers				DONE and TESTED DFB 20120330 */

/* Generate string						DONE and TESTED DFB 20120330 */
	/* Filename							DONE and TESTED DFB 20120330 */
		/* Truncated					DONE and TESTED DFB 20120330 */
		/* Not truncated				DONE and TESTED DFB 20120330 */
	/* Padding							DONE and TESTED DFB 20120330 */
		/* StartingPoint				DONE and TESTED DFB 20120330 */
		/* NumSpaces					DONE and TESTED DFB 20120330 */
	/* Timestamp						DONE and TESTED DFB 20120330 */
	

unsigned short csvhmiGenerateFileListString(unsigned long pFileListString, fiDIR_READ_DATA* pDirReadData)
{


/************************************************/
/* Check for invalid inputs						*/
/************************************************/

if( 	(pFileListString == 0)
	||	(pDirReadData == 0)
	){
	
	return (UINT)CSV_ERR_INVALIDINPUT;
		
} // Check for invalid inputs //


/****************************************************/
/* WARNING: THIS FUNCTION ASSUMES A CERTAIN			*/
/* STRING LENGTH. MAKE SURE THAT THE STRING			*/
/* PASSED TO THE FUNCTION IS THE CORRECT LENGTH.	*/
/****************************************************/

/* Clear file list string */

memset( (void*)pFileListString, 0, CSV_STRLEN_FILELIST+1);


/* Check to see if file name needs to be truncated and copy file name */

UDINT	FileNameLength=	strlen( (char*)&(pDirReadData->Filename) );


if( FileNameLength > CSV_STRLEN_FILENAMEDISPLAY ){

	strncpy( (char*)pFileListString, (char*)&(pDirReadData->Filename), CSV_STRLEN_FILENAMEDISPLAY - 3 );
	strcat( (char*)pFileListString, "..." );
	
}
else{
	
	strcpy( (char*)pFileListString, (char*)&(pDirReadData->Filename) );
	
}


/* Add padding */

UDINT	StartingPoint=	pFileListString + strlen( (char*)pFileListString );
UDINT	NumSpaces=		CSV_STRLEN_FILENAMEDISPLAY - strlen( (char*)pFileListString ) + CSV_STRLEN_MINSPACING;

memset( (char*)(StartingPoint), ' ', NumSpaces );


/* Add Timestamp */

STRING	Timestamp[CSV_STRLEN_TIMESTAMP+1];

GenerateTimestamp( pDirReadData->Date, (UDINT)Timestamp, CSV_STRLEN_TIMESTAMP );

strcat( (char*)pFileListString, Timestamp );


return 0;


} // End Fn //
