/*
 * File: csvSetError.c
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
/* Set error status on CSVFileMgr	 								*/
/********************************************************************/

/* Check for null pointer		DONE and TESTED DFB 20120313 */

/* Set Error status				DONE and TESTED DFB 20120802 */
	/* Busy, Done, Error		DONE and TESTED DFB 20120313 */
	/* ErrorID					DONE and TESTED DFB 20120313 */
	/* ErrorLevel				DONE and TESTED DFB 20120313 */
	/* ErrorState				DONE and TESTED DFB 20120313 */
	/* ErrorString				DONE and TESTED DFB 20120802 */
	
/* Set State					DONE and TESTED DFB 20120313 */


unsigned short csvSetError(unsigned short ErrorID, struct CSVFileMgr_typ* t)
{


/************************************************/
/* Check for invalid inputs						*/
/************************************************/

if( t == 0 ){
		
	return (UINT)CSV_ERR_INVALIDINPUT;
		
} // Check for null pointer //


/************************************************/
/* Set error status								*/
/************************************************/

t->OUT.STAT.Busy=		0;
t->OUT.STAT.Done=		0;

t->OUT.STAT.Error=		1;
t->OUT.STAT.ErrorID=	ErrorID;
t->OUT.STAT.ErrorLevel=	2;
t->OUT.STAT.ErrorState=	t->OUT.STAT.State;

t->OUT.STAT.State=		CSV_ST_ERROR;
	

/* Set ErrorString */

memset( t->OUT.STAT.ErrorString, 0, sizeof(t->OUT.STAT.ErrorString) );

switch( ErrorID ){
	

	/* No error */	

	case 0: strncpy( t->OUT.STAT.ErrorString, "", CSV_STRLEN_ERROR ); break;


	/* CSVFileLib Errors */
	
	case CSV_ERR_INVALIDINPUT:		strncpy( t->OUT.STAT.ErrorString, "Invalid function input.", CSV_STRLEN_ERROR ); break;
	case CSV_ERR_MEMCFG:			strncpy( t->OUT.STAT.ErrorString, "Invalid memory configuration. Check IN.CFG.MaxFileSize and MaxLineLength.", CSV_STRLEN_ERROR ); break;
	case CSV_ERR_MEMALLOC:			strncpy( t->OUT.STAT.ErrorString, "Error allocating memory. Check IN.CFG.MaxFileSize and MaxLineLength.", CSV_STRLEN_ERROR ); break;
	case CSV_ERR_NOTINITIALIZED:	strncpy( t->OUT.STAT.ErrorString, "CSVFileMgr not initialized. Call CSVFn_Init() in the initialization routine of your task.", CSV_STRLEN_ERROR ); break;
	case CSV_ERR_LINEFAILURE:		strncpy( t->OUT.STAT.ErrorString, "Error occurred while parsing file. Check log for details.", CSV_STRLEN_ERROR ); break;
	case CSV_ERR_BUFFERFULL:		strncpy( t->OUT.STAT.ErrorString, "Maximum file size reached. Check IN.CFG.MaxFileSize.", CSV_STRLEN_ERROR ); break;
	
	case CSV_ERR_LOGNOTAVAILABLE:	strncpy( t->OUT.STAT.ErrorString, "Log not available. Check IN.CFG.MaxFileSize.", CSV_STRLEN_ERROR ); break;
	case CSV_ERR_LOGFULL:			strncpy( t->OUT.STAT.ErrorString, "Log full. Check IN.CFG.MaxFileSize.", CSV_STRLEN_ERROR ); break;
	
	case CSV_ERR_INVALIDOPERATION:	strncpy( t->OUT.STAT.ErrorString, "Internal error: Invalid operation.", CSV_STRLEN_ERROR ); break;
	case CSV_ERR_COMMENT:			strncpy( t->OUT.STAT.ErrorString, "Internal error: Comment found.", CSV_STRLEN_ERROR ); break;
	case CSV_ERR_NOVARNAMEFOUND:	strncpy( t->OUT.STAT.ErrorString, "Internal error: No variable name found.", CSV_STRLEN_ERROR ); break;
	case CSV_ERR_NOVALUEFOUND:		strncpy( t->OUT.STAT.ErrorString, "Internal error: No value found.", CSV_STRLEN_ERROR ); break;
	case CSV_ERR_INVALIDVALUE:		strncpy( t->OUT.STAT.ErrorString, "Internal error: Invalid value.", CSV_STRLEN_ERROR ); break;
	case CSV_ERR_UNSUPPORTEDTYPE:	strncpy( t->OUT.STAT.ErrorString, "Internal error: Unsupported data type.", CSV_STRLEN_ERROR ); break;
	case CSV_ERR_INVALIDTYPE:		strncpy( t->OUT.STAT.ErrorString, "Internal error: Invalid data type.", CSV_STRLEN_ERROR ); break;

	case CSV_ERR_EMPTYFILENAME:		strncpy( t->OUT.STAT.ErrorString, "Empty file name. Check IN.PAR.FileName.", CSV_STRLEN_ERROR ); break;
	
	
//	/* sys_lib Errors */
//	
//	case 3300:
//	case 3310:
//	case 3311:
//	case 3312:
//	case 3313:
//	case 3314:
//	case 3324:
//	case 3332:
//	case 3333:
//	case 3336:
//	
//	case 9133:
//
//	case 14700:
//	case 14702:
//	case 14703:
//	case 14704:
//	case 14710:
//	case 14714:	strncpy( t->OUT.STAT.ErrorString, "Error with sys_lib library. Check VarName: ", CSV_STRLEN_ERROR ); break;
	
	default:	strncpy( t->OUT.STAT.ErrorString, "Unknown error.", CSV_STRLEN_ERROR ); break;
	
} // switch( ErrorID ) //


return 0;


} // End Fn //
