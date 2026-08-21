/*
 * File: csvOpenVar.c
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
#include <ctype.h>


/********************************************************************/
/* Process one variable of an Open operation 						*/
/********************************************************************/

/* Check for null pointers		DONE and TESTED DFB 20120313 */

/* Set Variable					DONE and TESTED DFB 20120406 */
	
	/* BOOL						DONE and TESTED DFB 20120313 */
		/* 0					DONE and TESTED DFB 20120313 */
		/* 1					DONE and TESTED DFB 20120313 */
		/* TRUE					DONE and TESTED DFB 20120313 */
		/* FALSE				DONE and TESTED DFB 20120313 */
		/* Invalid value		DONE and TESTED DFB 20120313 */
		
	/* SINT						DONE and TESTED DFB 20120313 */
		/* Valid value			DONE and TESTED DFB 20120313 */
		/* Invalid value		DONE and TESTED DFB 20120313 */
	/* INT						DONE and TESTED DFB 20120313 */
		/* Valid value			DONE and TESTED DFB 20120313 */
		/* Invalid value		DONE and TESTED DFB 20120313 */
	/* DINT						DONE and TESTED DFB 20120406 */
		/* Valid value			DONE and TESTED DFB 20120406 */
		/* Invalid value		DONE and TESTED DFB 20120406 */
	/* TIME						DONE and TESTED DFB 20120406 */
		/* Valid value			DONE and TESTED DFB 20120406 */
		/* Invalid value		DONE and TESTED DFB 20120406 */
	/* USINT					DONE and TESTED DFB 20120313 */
		/* Valid value			DONE and TESTED DFB 20120313 */
		/* Invalid value		DONE and TESTED DFB 20120313 */
	/* UINT						DONE and TESTED DFB 20120313 */
		/* Valid value			DONE and TESTED DFB 20120313 */
		/* Invalid value		DONE and TESTED DFB 20120313 */
	/* UDINT					DONE and TESTED DFB 20120406 */
		/* Valid value			DONE and TESTED DFB 20120406 */
		/* Invalid value		DONE and TESTED DFB 20120406 */
	/* DATE_AND_TIME			DONE and TESTED DFB 20120406 */
		/* Valid value			DONE and TESTED DFB 20120406 */
		/* Invalid value		DONE and TESTED DFB 20120406 */

	/* REAL						DONE and TESTED DFB 20120314 */
		/* Valid value			DONE and TESTED DFB 20120314 */
		/* Invalid value		DONE and TESTED DFB 20120314 */

	/* STRING					DONE and TESTED DFB 20120314 */
	
	/* Unsupported type			DONE and TESTED DFB 20120406 */
	/* Invalid type				DONE and TESTED DFB 20120314 */



/********************************************************************/
/* Parse a BYTE/WORD/DWORD value from the file						*/
/********************************************************************/

/* Reading is deliberately permissive, so that a file written with either
	BitStringFormat setting reads back, as do hand written files.
	
	A prefixed literal (0xFF, 16#FF, $FF) is hexadecimal, bare digits are
	decimal. Requiring the prefix for hex is what keeps this unambiguous -
	otherwise "10" could reasonably mean either ten or sixteen. */


static unsigned short csvParseBitString(const char* pValue, unsigned long* pResult)
{

	const char*	p;

	UDINT	Value, Digit;


	if( 	(pValue == 0)
		||	(pResult == 0)
		){
		
		return CSV_ERR_INVALIDINPUT;
		
	}


	p=	pValue;

	p+=	strspn( p, " \t\r\n\v\f" );


	/* Hexadecimal, if there is a prefix */

	if( 	( (p[0] == '0') && ((p[1] == 'x') || (p[1] == 'X')) )
		||	( (p[0] == '1') && (p[1] == '6') && (p[2] == '#') )
		||	(  p[0] == '$' )
		){

		if( HexStringToUDINT( (UDINT)p, (UDINT)pResult ) != 0 ) return CSV_ERR_INVALIDVALUE;

		return 0;

	}


	/* Decimal. Accumulated by hand rather than with atoui(), which
		saturates on overflow instead of reporting it. */

	if( 	(*p < '0')
		||	(*p > '9')
		){
		
		return CSV_ERR_INVALIDVALUE;
		
	}

	Value=	0;

	while( 	(*p >= '0')
		&&	(*p <= '9')
		){

		Digit=	*p - '0';

		/* Checked before multiplying so that an overflow is caught rather than wrapping */

		if( Value > ((CSV_MAX_UDINT - Digit) / 10) ) return CSV_ERR_INVALIDVALUE;

		Value=	Value * 10 + Digit;

		p++;

	}


	/* Anything after the digits other than white space is an error */

	p+=	strspn( p, " \t\r\n\v\f" );

	if( *p != '\0' ) return CSV_ERR_INVALIDVALUE;


	*pResult=	Value;

	return 0;


} // End Fn //


unsigned short csvOpenVar(unsigned long LineNumber, struct CSVFileVariable_typ* pVariable, struct CSVFileMgr_typ* t)
{


	/************************************************/
	/* Check for invalid inputs						*/
	/************************************************/

	if( t == 0 ){
		
		return (UINT)CSV_ERR_INVALIDINPUT;
		
	} // Check for null pointer //


	if( pVariable == 0 ){
	
		csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, LineNumber, (UDINT)"csvOpenVar()", t);
		
		return (UINT)CSV_ERR_INVALIDINPUT;
		
	} // Check for null pointer //


	if( pVariable->Address == 0 ){
	
		csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, LineNumber, (UDINT)"csvOpenVar()", t);
		
		return (UINT)CSV_ERR_INVALIDINPUT;
		
	} // Check for null pointer //


	if( t->Internal.pUserVariable != 0 && t->Internal.TemplateVariable.Address == 0 ){

		csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, LineNumber, (UDINT)"csvOpenVar()", t);
		
		return (UINT)CSV_ERR_INVALIDINPUT;
		
	} // Check for null pointer //

	/************************************************/
	/* Calculate address of variable to write to	*/
	/************************************************/	
	
	UDINT pWriteAddress;

	if( t->Internal.pUserVariable != 0 ){

		pWriteAddress = t->Internal.pUserVariable + (pVariable->Address - t->Internal.TemplateVariable.Address);

	}
	else{

		pWriteAddress = pVariable->Address;
	
	}

	/************************************************/
	/* Set variable to value from file				*/
	/************************************************/

	UDINT	i, ValueStringLength, ValueUdint;
	UDINT	MaxValue;
	UINT	ParseStatus;
	DINT	ValueDint;
	REAL	ValueReal;
	LREAL	ValueLReal;

	
	switch( pVariable->DataType ){
	
	
		/********************************************************************************/
		/* BOOL																			*/
		/********************************************************************************/
	
		case CSV_TYPE_BOOL:
		
			if(	strcmp( pVariable->Value, "0" ) == 0 ){
		
				*(BOOL*)(pWriteAddress)=	0;
		
			} // "0" //
		
			else if( strcmp( pVariable->Value, "1" ) == 0 ){
		
				*(BOOL*)(pWriteAddress)=	1;
		
			} // "1" //
		
			else{
			
			
				/* Convert String to upper case */
			
				ValueStringLength=	strlen( pVariable->Value );
			
				for( i=0; i<ValueStringLength; i++ ){
			
					pVariable->Value[i]=	toupper( pVariable->Value[i] );
			
				} // convert to upper case //
			
			
				/* Check string */
			
				if( strcmp( pVariable->Value, "FALSE" ) == 0 ){
		
					*(BOOL*)(pWriteAddress)=	0;
		
				}
				else if( strcmp( pVariable->Value, "TRUE" ) == 0 ){
		
					*(BOOL*)(pWriteAddress)=	1;
		
				}
				else{
			
					csvAddLogInfo( (UINT)CSV_INFO_INVALIDVALUE, LineNumber, (UDINT)pVariable->Name, t);
					return CSV_ERR_INVALIDVALUE;
			
				}
			
			
			} // Other string //

			break;
	
	
		/********************************************************************************/
		/* Integer types																*/
		/********************************************************************************/
	
		case CSV_TYPE_SINT:
		
			ValueDint=	brsatoi( (UDINT)pVariable->Value );
		
			if( 	(ValueDint > CSV_MAX_SINT)
				||	(ValueDint < CSV_MIN_SINT)
			){
			
				csvAddLogInfo( (UINT)CSV_INFO_INVALIDVALUE, LineNumber, (UDINT)pVariable->Name, t);
				return CSV_ERR_INVALIDVALUE;
		
			}
			else{
			
				*(SINT*)(pWriteAddress)=	(SINT)ValueDint;
				
			}
			
			break;
	
	
		case CSV_TYPE_INT:
		
			ValueDint=	brsatoi( (UDINT)pVariable->Value );
		
			if( 	(ValueDint > CSV_MAX_INT)
				||	(ValueDint < CSV_MIN_INT)
			){
			
				csvAddLogInfo( (UINT)CSV_INFO_INVALIDVALUE, LineNumber, (UDINT)pVariable->Name, t);
				return CSV_ERR_INVALIDVALUE;
		
			}
			else{
			
				*(INT*)(pWriteAddress)=	(INT)ValueDint;
				
			}
			
			break;
	
	
		case CSV_TYPE_DINT:
		case CSV_TYPE_TIME:
		
			ValueDint=	brsatoi( (UDINT)pVariable->Value );
		
			if( 	(ValueDint > CSV_MAX_DINT)
			||	(ValueDint < CSV_MIN_DINT)
			){
			
			csvAddLogInfo( (UINT)CSV_INFO_INVALIDVALUE, LineNumber, (UDINT)pVariable->Name, t);
			return CSV_ERR_INVALIDVALUE;
		
			}
			else{
			
			*(DINT*)(pWriteAddress)=	(DINT)ValueDint;
				
			}
			
			break;
	
	
		case CSV_TYPE_USINT:
		
			ValueDint=	brsatoi( (UDINT)pVariable->Value );
		
			if( 	(ValueDint > CSV_MAX_USINT)
				||	(ValueDint < CSV_MIN_USINT)
			){
			
				csvAddLogInfo( (UINT)CSV_INFO_INVALIDVALUE, LineNumber, (UDINT)pVariable->Name, t);
				return CSV_ERR_INVALIDVALUE;
		
			}
			else{
			
				*(USINT*)(pWriteAddress)=	(USINT)ValueDint;
				
			}
			
			break;
	
	
		case CSV_TYPE_UINT:
		
			ValueDint=	brsatoi( (UDINT)pVariable->Value );
		
			if( 	(ValueDint > CSV_MAX_UINT)
				||	(ValueDint < CSV_MIN_UINT)
			){
			
				csvAddLogInfo( (UINT)CSV_INFO_INVALIDVALUE, LineNumber, (UDINT)pVariable->Name, t);
				return CSV_ERR_INVALIDVALUE;
		
			}
			else{
			
				*(UINT*)(pWriteAddress)=	(UINT)ValueDint;
				
			}
			
			break;
	
	
		case CSV_TYPE_UDINT:
		case CSV_TYPE_DATE_AND_TIME:
		
			ValueUdint=	atoui( (UDINT)pVariable->Value );
		
			if( 	(ValueUdint > CSV_MAX_UDINT)
			||	(ValueUdint < CSV_MIN_UDINT)
			){
			
			csvAddLogInfo( (UINT)CSV_INFO_INVALIDVALUE, LineNumber, (UDINT)pVariable->Name, t);
			return CSV_ERR_INVALIDVALUE;
		
			}
			else{
			
			*(UDINT*)(pWriteAddress)=	(UDINT)ValueUdint;
				
			}
			
			break;
	
	
		/********************************************************************************/
		/* REAL																			*/
		/********************************************************************************/
	
		case CSV_TYPE_REAL:
		
			ValueReal=	brsatof( (UDINT)pVariable->Value );
		
			if( 	(ValueReal > CSV_MAX_REAL)
				||	(ValueReal < CSV_MIN_REAL)
			){
			
				csvAddLogInfo( (UINT)CSV_INFO_INVALIDVALUE, LineNumber, (UDINT)pVariable->Name, t);
				return CSV_ERR_INVALIDVALUE;
		
			}
			else{
			
				*(REAL*)(pWriteAddress)=	ValueReal;
				
			}
			
			break;

		case CSV_TYPE_LREAL:

			ValueLReal=	brsatod( (UDINT)pVariable->Value );
	
			if( 	(ValueReal > CSV_MAX_LREAL)
				||	(ValueReal < CSV_MIN_LREAL)
			){
		
				csvAddLogInfo( (UINT)CSV_INFO_INVALIDVALUE, LineNumber, (UDINT)pVariable->Name, t);
				return CSV_ERR_INVALIDVALUE;
	
			}
			else{
		
				*(LREAL*)(pWriteAddress)=	ValueLReal;
			
			}
		
			break;
	
		/********************************************************************************/
		/* STRING																		*/
		/********************************************************************************/
	
		case CSV_TYPE_STRING:
		
			memset( (char*)pWriteAddress, 0, pVariable->Length );
		
			strncpy( (char*)pWriteAddress, pVariable->Value, pVariable->Length-1 );
	
			break;
		
	
		/********************************************************************************/
		/* Bit string types																*/
		/********************************************************************************/
	
		case CSV_TYPE_BYTE:
		case CSV_TYPE_WORD:
		case CSV_TYPE_DWORD:
		
			/* Initialised because csvParseBitString() leaves it alone on failure,
				and the range check below must not read an indeterminate value */

			ValueUdint=		0;

			ParseStatus=	csvParseBitString( pVariable->Value, &ValueUdint );
		
			if( pVariable->DataType == CSV_TYPE_BYTE )		MaxValue=	CSV_MAX_BYTE;
			else if( pVariable->DataType == CSV_TYPE_WORD )	MaxValue=	CSV_MAX_WORD;
			else											MaxValue=	CSV_MAX_DWORD;
		
			if( 	(ParseStatus != 0)
				||	(ValueUdint > MaxValue)
				){
			
				csvAddLogInfo( (UINT)CSV_INFO_INVALIDVALUE, LineNumber, (UDINT)pVariable->Name, t);
				return CSV_ERR_INVALIDVALUE;
		
			}
			else{
			
				if( pVariable->DataType == CSV_TYPE_BYTE )		*(USINT*)(pWriteAddress)=	(USINT)ValueUdint;
				else if( pVariable->DataType == CSV_TYPE_WORD )	*(UINT*)(pWriteAddress)=	(UINT)ValueUdint;
				else											*(UDINT*)(pWriteAddress)=	(UDINT)ValueUdint;
				
			}
			
			break;
	
	
		/********************************************************************************/
		/* Unsupported and Invalid types												*/
		/********************************************************************************/
	
		case CSV_TYPE_STRUCT:
		case CSV_TYPE_ULINT:
		case CSV_TYPE_DATE:
		case CSV_TYPE_ARRAY_OF_STRUCT:
		case CSV_TYPE_TIME_OF_DAY:
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


	return 0;


} // End Fn //
