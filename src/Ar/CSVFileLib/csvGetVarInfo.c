/*
 * File: csvGetVarInfo.c
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
/* Get variable address and data type based on variable name 		*/
/********************************************************************/

/* Check for null pointers		DONE and TESTED DFB 20120313 */

/* Add prefix					DONE and TESTED DFB 20120419 */

/* Get Address					DONE and TESTED DFB 20120313 */
	/* No error					DONE and TESTED DFB 20120313 */
	/* Error					DONE and TESTED DFB 20120313 */

/* Get Info						DONE will not be tested further */
	/* Check for enums			DONE and TESTED DFB 20121022 */
	/* No error					DONE and TESTED DFB 20120313 */
	/* Error					DONE will not be tested */


unsigned short csvGetVarInfo(unsigned long LineNumber, struct CSVFileVariable_typ* pVariable, struct CSVFileMgr_typ* t)
{


/************************************************/
/* Check for invalid inputs						*/
/************************************************/

if( t == 0 ){
		
	return (UINT)CSV_ERR_INVALIDINPUT;
		
} // Check for null pointer //


if( pVariable == 0 ){
	
	csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, LineNumber, (UDINT)"csvGetVarInfo()", t);
		
	return (UINT)CSV_ERR_INVALIDINPUT;
		
} // Check for null pointer //


/************************************************/
/* Get variable info							*/
/************************************************/

UINT	Status;

UDINT	DataLength;

STRING	VarAddress[10+1],
		VarType[10+1],
		VarLength[10+1],
		VarDimension[5+1],
		ElementName[CSV_STRLEN_VARNAME+1];


/* Add prefix */

csvAddPrefix( sizeof(pVariable->Name) - 1, pVariable->Name, t->IN.PAR.Prefix );


/* Get address */

Status=	PV_xgetadr(	pVariable->Name, 
					&(pVariable->Address), 
					&DataLength );


if( Status == 0 ){

	
	/* Have address. Log and get other info */
	
	if( t->IN.CFG.VerboseLog ){
	
		brsitoa( pVariable->Address, (UDINT)VarAddress );
		csvAddLogInfo( (UINT)CSV_INFO_VERBOSE_VARADDRESS, LineNumber, (UDINT)VarAddress, t );
	
	}
	
	
	Status=	PV_ninfo(	pVariable->Name,
						&(pVariable->DataType),
						&(pVariable->Length),
						&(pVariable->Dimension) );
						
	
	if( Status == 0 ){
		
	
		/* Check for enums */
		
		if( pVariable->DataType == CSV_TYPE_STRUCT ){
			
			Status=	PV_item( pVariable->Name, 0, ElementName );
			
			
			/* Handle enums as if they were DINTs */
									
			if( Status == CSV_ERR_PVITEM_ENUM )	pVariable->DataType=	CSV_TYPE_DINT;
			
		} // if struct //
		
		
		/* Have all variable info. Log and return */
		
		if( t->IN.CFG.VerboseLog ){
			
			brsitoa( pVariable->DataType, (UDINT)VarType );
			csvAddLogInfo( (UINT)CSV_INFO_VERBOSE_VARTYPE, LineNumber, (UDINT)VarType, t );
		
			brsitoa( pVariable->Length, (UDINT)VarLength );
			csvAddLogInfo( (UINT)CSV_INFO_VERBOSE_VARLENGTH, LineNumber, (UDINT)VarLength, t );
		
			brsitoa( pVariable->Dimension, (UDINT)VarDimension );
			csvAddLogInfo( (UINT)CSV_INFO_VERBOSE_VARDIMENSION, LineNumber, (UDINT)VarDimension, t );
		
		}
		
		
		return 0;
	
		
	} // Found var info //
	
	else{
	
		
		/* Error getting variable information. Log and return error */
		
		csvAddLogInfo( (UINT)CSV_INFO_DATATYPENOTFOUND, LineNumber, (UDINT)pVariable->Name, t );
		
		return Status;
	
		
	} // Error getting var info //
	
} // Found address //

else{


	/* Error getting address. Log and return error */

	csvAddLogInfo( (UINT)CSV_INFO_ADDRESSNOTFOUND, LineNumber, (UDINT)pVariable->Name, t );
	
	return Status;


} // Error getting address //


} // End Fn //
