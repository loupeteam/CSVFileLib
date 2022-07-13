/********************************************************************
 * COPYRIGHT -- Automation Resources Group
 ********************************************************************
 * Library: CSVFileLib
 * File: csvSaveVar.c
 * Author: David Blackburn
 * Created: January 06, 2012
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
/* Process one variable of a Save operation 						*/
/********************************************************************/

/* Check for null pointer		DONE and TESTED DFB 20120314 */

/* Check for invalid inputs		DONE and TESTED DFB 20120406 */

/* Write Value to Variable		DONE and TESTED DFB 20120319 */
	
	/* BOOL						DONE and TESTED DFB 20120314 */
			
	/* SINT						DONE and TESTED DFB 20120314 */
	/* INT						DONE and TESTED DFB 20120314 */
	/* DINT						DONE and TESTED DFB 20120406 */
	/* TIME						DONE and TESTED DFB 20120406 */
	/* USINT					DONE and TESTED DFB 20120314 */
	/* UINT						DONE and TESTED DFB 20120314 */
	/* UDINT					DONE and TESTED DFB 20120406 */
	/* DATE_AND_TIME			DONE and TESTED DFB 20120406 */

	/* REAL						DONE and TESTED DFB 20120314 */

	/* STRING					DONE and TESTED DFB 20120319 */
	
	/* Unsupported type			DONE and TESTED DFB 20120406 */
	/* Invalid type				DONE and TESTED DFB 20120314 */

/* Format line for file			DONE and TESTED DFB 20120314 */
	/* Remove prefix			DONE and TESTED DFB 20120420 */
		/* No error				DONE and TESTED DFB 20120420 */
		/* Error				DONE and TESTED DFB 20120420 */

/* Add line to write buffer		DONE and TESTED DFB 20120420 */
	/* No error					DONE and TESTED DFB 20120420 */
	/* Error					DONE and TESTED DFB 20120420 */


unsigned short csvSaveVar(unsigned long LineNumber, struct CSVFileVariable_typ* pVariable, struct CSVFileMgr_typ* t)
{


	/************************************************/
	/* Check for invalid inputs						*/
	/************************************************/

	if( t == 0 ){
		
		return (UINT)CSV_ERR_INVALIDINPUT;
		
	} // Check for null pointer //


	if( pVariable == 0 ){
	
		csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, LineNumber, (UDINT)"csvSaveVar()", t);
		
		return (UINT)CSV_ERR_INVALIDINPUT;
		
	} // Check for null pointer //


	if( pVariable->Address == 0 ){
	
		csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, LineNumber, (UDINT)"csvSaveVar()", t);
		
		return (UINT)CSV_ERR_INVALIDINPUT;
		
	} // Check for null pointer //


	if( t->Internal.WriteBuffer.pData == 0 ){

		csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, LineNumber, (UDINT)"csvSaveVar()", t);
		
		return (UINT)CSV_ERR_INVALIDINPUT;
	
	} // Check for null pointer //

	if( t->Internal.pUserVariable != 0 && t->Internal.TemplateVariable.Address == 0 ){
		csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, LineNumber, (UDINT)"csvOpenVar()", t);
		
		return (UINT)CSV_ERR_INVALIDINPUT;
		
	} // Check for null pointer //

	UDINT pReadAddress;

	/************************************************/
	/* Calculate address of variable to write to	*/
	/************************************************/	
	
	if( t->Internal.pUserVariable != 0 ){

		pReadAddress = t->Internal.pUserVariable + (pVariable->Address - t->Internal.TemplateVariable.Address);

	}
	else{

		pReadAddress = pVariable->Address;
	
	}
	
	/********************************************************/
	/* Get value of variable and copy to Variable.Value		*/
	/********************************************************/

	DINT	ValueDint;
	UDINT	ValueUdint;
	REAL	ValueReal;
	LREAL	ValueLReal;


	/* Clear the Value string. It will be overwritten below */

	memset( pVariable->Value, 0, sizeof(pVariable->Value) );


	switch( pVariable->DataType ){
	
	
		/********************************************************************************/
		/* BOOL																			*/
		/********************************************************************************/
	
		case CSV_TYPE_BOOL:
		
			if( *(BOOL*)(pReadAddress) == 0 ){
			
				strcpy( pVariable->Value, "FALSE" );
			
			}
			else if( *(BOOL*)(pReadAddress) == 1 ){
			
				strcpy( pVariable->Value, "TRUE" );
			
			}
			else{
			
				csvAddLogInfo( (UINT)CSV_INFO_INVALIDVALUE, LineNumber, (UDINT)pVariable->Name, t);
				return CSV_ERR_INVALIDVALUE;
			
			}
		
			break;
	
	
		/********************************************************************************/
		/* Integer types																*/
		/********************************************************************************/
	
		case CSV_TYPE_SINT:
		
			ValueDint=	*(SINT*)(pReadAddress);
		
			brsitoa( ValueDint, (UDINT)pVariable->Value );
		
			break;
	
	
		case CSV_TYPE_INT:
		
			ValueDint=	*(INT*)(pReadAddress);
		
			brsitoa( ValueDint, (UDINT)pVariable->Value );
		
			break;
	
	
		case CSV_TYPE_DINT:
		case CSV_TYPE_TIME:
		
			ValueDint=	*(DINT*)(pReadAddress);
		
			brsitoa( ValueDint, (UDINT)pVariable->Value );
		
			break;
	
	
		case CSV_TYPE_USINT:
		
			ValueDint=	*(USINT*)(pReadAddress);
		
			brsitoa( ValueDint, (UDINT)pVariable->Value );
		
			break;
	
	
		case CSV_TYPE_UINT:
		
			ValueDint=	*(UINT*)(pReadAddress);
		
			brsitoa( ValueDint, (UDINT)pVariable->Value );
		
			break;
	
	
		case CSV_TYPE_UDINT:
		case CSV_TYPE_DATE_AND_TIME:
		
			ValueUdint=	*(UDINT*)(pReadAddress);
		
			uitoa( ValueUdint, (UDINT)pVariable->Value );
		
			break;
	
	
		/********************************************************************************/
		/* REAL																			*/
		/********************************************************************************/
	
		case CSV_TYPE_REAL:
		
			ValueReal=	*(REAL*)(pReadAddress);
		
			brsftoa( ValueReal, (UDINT)pVariable->Value );
		
			break;
	
		case CSV_TYPE_LREAL:

			ValueLReal=	*(LREAL*)(pReadAddress);
	
			csvDTOA( ValueLReal, (UDINT)pVariable->Value, t->IN.CFG.DoubleSigFigs );
	
			break;	
		/********************************************************************************/
		/* STRING																		*/
		/********************************************************************************/
	
		case CSV_TYPE_STRING:

			strcpy(pVariable->Value, "'");
			strncat(pVariable->Value, (char*)pReadAddress, sizeof(pVariable->Value)-3);
			strcat(pVariable->Value, "'");
		
			break;
		
	
		/********************************************************************************/
		/* Unsupported and Invalid types												*/
		/********************************************************************************/
	
		case CSV_TYPE_STRUCT:
		case CSV_TYPE_ULINT:
		case CSV_TYPE_DATE:
		case CSV_TYPE_ARRAY_OF_STRUCT:
		case CSV_TYPE_TIME_OF_DAY:
		case CSV_TYPE_BYTE:
		case CSV_TYPE_WORD:
		case CSV_TYPE_DWORD:
		case CSV_TYPE_LWORD:
		case CSV_TYPE_WSTRING:
		case CSV_TYPE_LINT:
	
			csvAddLogInfo( (UINT)CSV_INFO_UNSUPPORTEDTYPE, LineNumber, (UDINT)pVariable->Name, t);
			return CSV_ERR_UNSUPPORTEDTYPE;
	
			break;


		default:
	
			csvAddLogInfo( (UINT)CSV_INFO_INVALIDTYPE, LineNumber, (UDINT)pVariable->Name, t);
			return CSV_ERR_INVALIDTYPE;

			break;	
	
	
	} // switch(DataType) //


	/************************************************/
	/* Format line and copy to pWriteData			*/
	/************************************************/


	/* Format Line */

	UINT	AddStatus, RemovePrefixStatus;
			
	STRING	Line[t->IN.CFG.MaxLineLength+1];
	memset( Line, 0, sizeof(Line) );


	/* Variable Name */

	strncat( Line, pVariable->Name, t->IN.CFG.MaxLineLength - strlen(Line) - 2 );


	/* Remove prefix */

	RemovePrefixStatus=	csvRemovePrefix( sizeof(Line) - 1, Line, t->IN.PAR.Prefix );


	if( RemovePrefixStatus == CSV_ERR_INVALIDINPUT ){

		csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, LineNumber, (UDINT)"csvRemovePrefix()", t);
		return RemovePrefixStatus;

	}
	else if( RemovePrefixStatus == CSV_ERR_PREFIXNOTSTRUCTURE ){

		csvAddLogInfo( (UINT)CSV_INFO_PREFIXNOTSTRUCTURE, LineNumber, (UDINT)&(pVariable->Name), t);
		return RemovePrefixStatus;

	}

	
	/* Value */

	strncat( Line, "," , t->IN.CFG.MaxLineLength - strlen(Line) - 2 );
	strncat( Line, pVariable->Value , t->IN.CFG.MaxLineLength - strlen(Line) - 2 );


	/* Description */

	if( strcmp( pVariable->Description, "" ) != 0 ){

		strncat( Line, ",", t->IN.CFG.MaxLineLength - strlen(Line) - 2 );
		strncat( Line, pVariable->Description, t->IN.CFG.MaxLineLength - strlen(Line) - 2 );
	
	}


	/* Newline */

	strcat( Line, "\r\n" );


	/* Add Line to write buffer and check for errors */

	AddStatus=	csvAddToBuffer( (UDINT)Line, strlen(Line), &(t->Internal.WriteBuffer) );


	if( AddStatus == CSV_ERR_INVALIDINPUT ){

		csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, LineNumber, (UDINT)"csvAddToBuffer()", t);
		return AddStatus;
	
	}
	else if( AddStatus == CSV_ERR_BUFFERFULL ){

		csvAddLogInfo( (UINT)CSV_INFO_FILEFULL, LineNumber, 0, t);
		return AddStatus;

	}


	return 0;


} // End Fn //
