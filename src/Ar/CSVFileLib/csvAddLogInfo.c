/*
 * File: csvAddLogInfo.c
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
/* Add information to the parse status log							*/
/********************************************************************/

/* Check for null pointer		DONE and TESTED DFB 20120313 */

/* Check for initialized		DONE and TESTED DFB 20120405 */

/* Create log entry				DONE and TESTED DFB 20120313 */
	/* Line number				DONE and TESTED DFB 20120313 */
	/* Info						DONE and TESTED DFB 20120313 */
		/* Error				DONE and TESTED DFB 20120420 */
		/* Verbose				DONE and TESTED DFB 20120313 */
		/* Done					DONE and TESTED DFB 20120313 */
		/* Default				DONE and TESTED DFB 20120313 */
	/* Additional info			DONE and TESTED DFB 20120313 */
	
/* Add entry to log				DONE and TESTED DFB 20120405 */
	/* No error					DONE and TESTED DFB 20120405 */
	/* Error					DONE and TESTED DFB 20120405 */


unsigned short csvAddLogInfo(unsigned short InfoID, unsigned long LineNumber, unsigned long pAddInfo, struct CSVFileMgr_typ* t)
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
/* Add information based on InfoID				*/
/************************************************/

STRING	InfoString[CSV_STRLEN_INFOSTRING+1],
		LineString[10+1],
		NumSuccess[10+1],
		NumFailure[10+1];

memset( InfoString, 0, sizeof(InfoString) );
memset( LineString, 0, sizeof(LineString) );
memset( NumSuccess, 0, sizeof(NumSuccess) );
memset( NumFailure, 0, sizeof(NumFailure) );

brsitoa( LineNumber, (UDINT)LineString );

strcpy( InfoString, "Line " );
strcat( InfoString, LineString );
strcat( InfoString, ": " );


switch( InfoID ){

	case CSV_INFO_NOVARNAMEFOUND:		strncat( InfoString, "ERROR: No variable name found", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_NOVALUEFOUND:			strncat( InfoString, "ERROR: No value found for variable", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_ADDRESSNOTFOUND:		strncat( InfoString, "ERROR: No address found for variable", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_DATATYPENOTFOUND:		strncat( InfoString, "ERROR: No data type found for variable", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_INVALIDVALUE:			strncat( InfoString, "ERROR: Invalid value based on variable data type. Variable Name:", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_UNSUPPORTEDTYPE:		strncat( InfoString, "ERROR: Unsupported data type. Variable Name:", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_INVALIDTYPE:			strncat( InfoString, "ERROR: Invalid data type. Variable Name:", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_INVALIDINPUT:			strncat( InfoString, "ERROR: Invalid input to helper function", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_FILEFULL:				strncat( InfoString, "ERROR: MaxFileSize exceeded.", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_INFONOTFOUND:			strncat( InfoString, "ERROR: Information not found for variable", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_MAXLEVELREACHED:		strncat( InfoString, "ERROR: Maximum structure level reached. Increase CSV_MAI_LEVEL constant and recompile project.", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_ITEMNOTFOUND:			strncat( InfoString, "ERROR: Item not found for variable", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_PREFIXNOTSTRUCTURE:	strncat( InfoString, "ERROR: Variable name starts with IN.PAR.Prefix, but the prefix is not a structure. Variable Name: ", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	
	case CSV_INFO_VERBOSE_VARNAME:		strncat( InfoString, "Variable Name:", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_VERBOSE_VALUE:		strncat( InfoString, "Value:", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_VERBOSE_DESCRIPTION:	strncat( InfoString, "Description:", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_VERBOSE_VARADDRESS:	strncat( InfoString, "Variable Address:", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_VERBOSE_VARTYPE:		strncat( InfoString, "Variable Data Type:", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_VERBOSE_VARLENGTH:	strncat( InfoString, "Variable Length:", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_VERBOSE_VARDIMENSION:	strncat( InfoString, "Variable Dimension:", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	case CSV_INFO_VERBOSE_COMMENT:		strncat( InfoString, "Comment:", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	
	case CSV_INFO_DONE:
	
		brsitoa( t->Internal.SuccessfulLineCount, (UDINT)NumSuccess );
		brsitoa( t->Internal.FailedLineCount, (UDINT)NumFailure );
		
		/* Using strcpy instead of strncat here removes the Line Number from the beginning of the log message */
		strcpy( InfoString, "Finished parsing file; " );
		
		strncat( InfoString, NumSuccess, CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 );
		strncat( InfoString, " lines processed successfully; ", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 );
		strncat( InfoString, NumFailure, CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 );
		strncat( InfoString, " lines contained errors", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 );
	
		break;
	
	default:	strncat( InfoString, "ERROR: Invalid InfoID. AddInfo:", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 ); break;
	
} // switch( InfoID ) //


/* Cat additional info if present */

if( pAddInfo != 0 ){
	strncat( InfoString, " ", CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 );
	strncat( InfoString, (char*)pAddInfo, CSV_STRLEN_INFOSTRING - strlen(InfoString) - 2 );
}


/* Remove any preexisting newlines and add one */

rstrip( (UDINT)InfoString, (UDINT)"\r\n" );
strcat( InfoString, "\r\n" );


/* Add to log buffer */

return csvAddToBuffer( (UDINT)InfoString, strlen(InfoString), &(t->Internal.LogBuffer) );


//if( ( strlen((char*)t->Internal.pLog) + strlen(InfoString) ) <= t->IN.CFG.MaxFileSize ){
//	
//	strcat( (char*)(t->Internal.pLog), InfoString );
//
//	return 0;
//
//}
//else{
//
//	return (UINT)CSV_ERR_LOGFULL;
//	
//}


} // End Fn //
