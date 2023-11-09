/*
 * File: csvExpandVar.c
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
/* Expand a complex variable into simple variables 					*/
/********************************************************************/

/* Check for null pointer					DONE and TESTED DFB 20120319 */

/* Check for invalid inputs					DONE and TESTED DFB 20120405 */
	
/* Copy VarName at top level				DONE and TESTED DFB 20120319 */

/* Traverse Structure						DONE will not be tested further */

	/* Get Info if necessary				DONE and TESTED DFB 20120319 */
		/* Check InfoStatus					DONE and TESTED DFB 20120319 */
		/* Reset iItem						DONE and TESTED DFB 20120319 */
		/* Check for enum					DONE and TESTED DFB 20121022 */
	
	/* Switch(DataType)						DONE will not be tested further */
	
		/* Structure						DONE will not be tested further */
			/* Check current level			DONE and TESTED DFB 20120319 */
			/* Get Item						DONE and TESTED DFB 20120319 */
			/* Check ItemStatus error		DONE will not be tested */
			/* Set next level name			DONE and TESTED DFB 20120319 */
			/* Increment level				DONE and TESTED DFB 20120319 */
	
		/* Array of structures				DONE and TESTED DFB 20120319 */
			/* Check current level			DONE and TESTED DFB 20120319 */
			/* Set next level name			DONE and TESTED DFB 20120319 */
			/* Increment level				DONE and TESTED DFB 20120319 */
		
		/* Basic data type					DONE and TESTED DFB 20120420 */
			/* Add names to buffer			DONE and TESTED DFB 20120420 */
				/* Single variable			DONE and TESTED DFB 20120420 */
					/* Remove prefix		DONE and TESTED DFB 20120420 */
						/* Error			DONE and TESTED DFB 20120420 */
						/* No error			DONE and TESTED DFB 20120420 */
					/* Full					DONE and TESTED DFB 20120405 */
					/* Not full				DONE and TESTED DFB 20120405 */
				/* Array variable			DONE and TESTED DFB 20120420 */
					/* Remove prefix		DONE and TESTED DFB 20120420 */
						/* Error			DONE and TESTED DFB 20120420 */
						/* No error			DONE and TESTED DFB 20120420 */
					/* Full					DONE and TESTED DFB 20120405 */
					/* Not full				DONE and TESTED DFB 20120405 */
			/* Clear current level			DONE and TESTED DFB 20120319 */
			/* Find next item				DONE and TESTED DFB 20120319 */
				/* All done					DONE and TESTED DFB 20120319 */
				/* Not done with a level	DONE and TESTED DFB 20120319 */
				/* Done with a level		DONE and TESTED DFB 20120319 */
				
/* Return if busy 							DONE and TESTED DFB 20120321 */


unsigned short csvExpandVar(unsigned long pVarName, struct CSVFileMgr_Int_Buffer_typ* pBuffer, struct CSVFileMgr_StructData_typ* pStructData, struct CSVFileMgr_typ* t)
{
	
	
/************************************************/
/* Check for invalid inputs						*/
/************************************************/

if( t == 0 ){
		
	return (UINT)CSV_ERR_INVALIDINPUT;
		
} // Check for null pointer //


if(		(pVarName == 0)
	||	(pBuffer == 0)
	||	(pStructData == 0)
	){
	
	csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, 0, (UDINT)"csvExpandVar()", t);
		
	return (UINT)CSV_ERR_INVALIDINPUT;
		
} // Check for null pointer //


/************************************************/
/* Expand variable								*/
/************************************************/

UDINT	i, iArray;
UINT	InfoStatus, ItemStatus, AddStatus, RemovePrefixStatus;
STRING	ElementName[CSV_STRLEN_VARNAME + 1], 
		ItemAddInfo[CSV_STRLEN_INFOSTRING + 1], 
		ItemNumberString[10 + 1],
		ArrayIndexString[10 + 1],
		Line[CSV_STRLEN_VARNAME + 3 + 1]; // Add characters for comma (1), and newline (2) //


/* If you are at the top level,
	copy VarName to StructData */

if( pStructData->iLevel == 0 ){
	
	strcpy( pStructData->Level[0].Name, "" );
	strncat( pStructData->Level[0].Name, (char*)pVarName, CSV_STRLEN_VARNAME );
	
}


/* Traverse structure looking for simple variables */

for( i = 0; i < t->IN.CFG.MaxLinesPerScan; i++ ){
	
	
	/* If necessary, get info */
	
	if( pStructData->Level[pStructData->iLevel].Length  == 0 ){
	
	
		/* Get Var Info for current level */
	
		InfoStatus=	PV_ninfo(	pStructData->Level[pStructData->iLevel].Name, 
								&(pStructData->Level[pStructData->iLevel].DataType), 
								&(pStructData->Level[pStructData->iLevel].Length), 
								&(pStructData->Level[pStructData->iLevel].Dimension) );
		
		if( InfoStatus != 0 ){
			
			csvAddLogInfo( (UINT)CSV_INFO_INFONOTFOUND, 0, (UDINT)pStructData->Level[pStructData->iLevel].Name, t);
		
			return InfoStatus;
	
		}
		
		
		/* Reset current item index */
		
		pStructData->Level[pStructData->iLevel].iItem=	0;
		
		
		/* Check for enums */
		
		if( pStructData->Level[pStructData->iLevel].DataType == CSV_TYPE_STRUCT ){
			
			ItemStatus=	PV_item( 	pStructData->Level[pStructData->iLevel].Name,
									0,
									ElementName );
			
			
			/* Handle enums as if they were DINTs */
									
			if( ItemStatus == CSV_ERR_PVITEM_ENUM )	pStructData->Level[pStructData->iLevel].DataType=	CSV_TYPE_DINT;
			
		} // if struct //
								
				
	} // If current level is empty //
			
	
	/* switch off datatype - 
		If it is a structure then you need to get current structure element and move down one level
		If it is an array of structures, then you need to get current array element and move down one level
		If it is a basic data point, then you need to write the name(s) to the write data and move up to the proper level and item */
	
	switch( pStructData->Level[pStructData->iLevel].DataType ){
		
		
		/****************************************************/
		/* Single Structure		 							*/
		/****************************************************/
		
		case CSV_TYPE_STRUCT:	
			
			
			/* Check current level */
			
			if( pStructData->iLevel == CSV_MAI_LEVEL){
				
				csvAddLogInfo( (UINT)CSV_INFO_MAXLEVELREACHED, 0, 0, t);
		
				return (UINT)CSV_ERR_MAXLEVELREACHED;
				
			}
			
			
			/* Get Current Structure Element name */
			
			ItemStatus=	PV_item( 	pStructData->Level[pStructData->iLevel].Name,
									pStructData->Level[pStructData->iLevel].iItem,
									ElementName );
			
			if( ItemStatus != 0 ){
				
				brsitoa( pStructData->Level[pStructData->iLevel].iItem, (UDINT)ItemNumberString );
				
				strcpy( ItemAddInfo, "" );
				strncat( ItemAddInfo, pStructData->Level[pStructData->iLevel].Name, CSV_STRLEN_INFOSTRING - strlen(ItemAddInfo) );
				strncat( ItemAddInfo, ", item ", CSV_STRLEN_INFOSTRING - strlen(ItemAddInfo) );
				strncat( ItemAddInfo, ItemNumberString, CSV_STRLEN_INFOSTRING - strlen(ItemAddInfo) );
				
				csvAddLogInfo( (UINT)CSV_INFO_ITEMNOTFOUND, 0, (UDINT)ItemAddInfo, t);
		
				return ItemStatus;
				
				/* DFB 20130627 - This is where having a dummy structure as element breaks the expansion.
					If you want to recover from this, you will have to skip this item appropriately.
					Not sure how to do that right now. */
		
			} // Error with PV_Item //
			
			
			/* Set next level name to current structure element,
				go down one level */
			
			strcpy( pStructData->Level[pStructData->iLevel + 1].Name, "" );
			strncat( pStructData->Level[pStructData->iLevel + 1].Name, pStructData->Level[pStructData->iLevel].Name,	CSV_STRLEN_VARNAME - strlen(pStructData->Level[pStructData->iLevel + 1].Name) );
			strncat( pStructData->Level[pStructData->iLevel + 1].Name, ".", 											CSV_STRLEN_VARNAME - strlen(pStructData->Level[pStructData->iLevel + 1].Name) );
			strncat( pStructData->Level[pStructData->iLevel + 1].Name, ElementName, 									CSV_STRLEN_VARNAME - strlen(pStructData->Level[pStructData->iLevel + 1].Name) );
			
			pStructData->iLevel++;
			
			break;
			
		
		/****************************************************/
		/* Array of structures 								*/
		/****************************************************/
		
		case CSV_TYPE_ARRAY_OF_STRUCT:


			/* Check current level */
			
			if( pStructData->iLevel == CSV_MAI_LEVEL){
				
				csvAddLogInfo( (UINT)CSV_INFO_MAXLEVELREACHED, 0, 0, t);
		
				return (UINT)CSV_ERR_MAXLEVELREACHED;
				
			}
			
			
			/* Set next level name to current array element,
				go down one level */
			
			brsitoa( pStructData->Level[pStructData->iLevel].iItem, (UDINT)ArrayIndexString );

			strcpy( pStructData->Level[pStructData->iLevel + 1].Name, "" );
			strncat( pStructData->Level[pStructData->iLevel + 1].Name, pStructData->Level[pStructData->iLevel].Name,	CSV_STRLEN_VARNAME - strlen(pStructData->Level[pStructData->iLevel + 1].Name) );
			strncat( pStructData->Level[pStructData->iLevel + 1].Name, "[", 											CSV_STRLEN_VARNAME - strlen(pStructData->Level[pStructData->iLevel + 1].Name) );
			strncat( pStructData->Level[pStructData->iLevel + 1].Name, ArrayIndexString, 								CSV_STRLEN_VARNAME - strlen(pStructData->Level[pStructData->iLevel + 1].Name) );
			strncat( pStructData->Level[pStructData->iLevel + 1].Name, "]", 											CSV_STRLEN_VARNAME - strlen(pStructData->Level[pStructData->iLevel + 1].Name) );
			
			pStructData->iLevel++;
			
			break;
			
			
		/****************************************************/
		/* Basic data types									*/
		/****************************************************/
	
		default:
			
			
			/****************************************/
			/* Write variable name(s) to pData		*/
			/****************************************/
			
			
			/* Single basic variable */
			
			if( pStructData->Level[pStructData->iLevel].Dimension == 1 ){
				
				
				/* Populate Line */
				
				strcpy( Line, "" );
				strncat( Line, pStructData->Level[pStructData->iLevel].Name, CSV_STRLEN_VARNAME );
				strcat( Line, ",\r\n" );
				
				
				/* Remove prefix and check for errors */
					
				RemovePrefixStatus=	csvRemovePrefix( sizeof(Line) - 1, Line, t->IN.PAR.Prefix );
				
				
				if( RemovePrefixStatus == CSV_ERR_INVALIDINPUT ){
					
					csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, 0, (UDINT)"csvRemovePrefix()", t);
					t->Internal.ExpandError=	1;
					
				}
				else if( RemovePrefixStatus == CSV_ERR_PREFIXNOTSTRUCTURE ){
					
					csvAddLogInfo( (UINT)CSV_INFO_PREFIXNOTSTRUCTURE, 0, (UDINT)&(pStructData->Level[pStructData->iLevel].Name), t);
					t->Internal.ExpandError=	1;
					
				}
				
					
				/* Add Line to buffer and check for errors */
				
				AddStatus=	csvAddToBuffer( (UDINT)Line, strlen(Line), pBuffer );
				
				
				if( AddStatus == CSV_ERR_INVALIDINPUT ){
					
					csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, 0, (UDINT)"csvAddToBuffer()", t);
					return AddStatus;
					
				}
				else if( AddStatus == CSV_ERR_BUFFERFULL ){
					
					csvAddLogInfo( (UINT)CSV_INFO_FILEFULL, 0, 0, t);
					return AddStatus;
					
				}
				
				
			} // Single variable //
			
			
			/* Array of basic variables */
			
			else{
				
			
				/* Loop through array */
				
				for( iArray=0; iArray < pStructData->Level[pStructData->iLevel].Dimension; iArray++ ){
					
					
					/* Populate Line */
				
					brsitoa( iArray, (UDINT)ArrayIndexString );
					
					strcpy( Line, "" );
					
					strncat( Line, pStructData->Level[pStructData->iLevel].Name,	CSV_STRLEN_VARNAME );
					
					strncat( Line, "[", 				CSV_STRLEN_VARNAME - strlen(pStructData->Level[pStructData->iLevel].Name) );
					strncat( Line, ArrayIndexString,	CSV_STRLEN_VARNAME - strlen(pStructData->Level[pStructData->iLevel].Name) );
					strncat( Line, "]", 				CSV_STRLEN_VARNAME - strlen(pStructData->Level[pStructData->iLevel].Name) );
					
					strcat( Line, ",\r\n" );
				
					
					/* Remove prefix and check for errors */
					
					RemovePrefixStatus=	csvRemovePrefix( sizeof(Line) - 1, Line, t->IN.PAR.Prefix );
				
				
					if( RemovePrefixStatus == CSV_ERR_INVALIDINPUT ){
					
						csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, 0, (UDINT)"csvRemovePrefix()", t);
						t->Internal.ExpandError=	1;
					
					}
					else if( RemovePrefixStatus == CSV_ERR_PREFIXNOTSTRUCTURE ){
					
						csvAddLogInfo( (UINT)CSV_INFO_PREFIXNOTSTRUCTURE, 0, (UDINT)&(pStructData->Level[pStructData->iLevel].Name), t);
						t->Internal.ExpandError=	1;
					
					}
				
					
					/* Add Line to buffer and check for errors */
				
					AddStatus=	csvAddToBuffer( (UDINT)Line, strlen(Line), pBuffer );
				
				
					if( AddStatus == CSV_ERR_INVALIDINPUT ){
					
						csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, 0, (UDINT)"csvAddToBuffer()", t);
						return AddStatus;
					
					}
					else if( AddStatus == CSV_ERR_BUFFERFULL ){
					
						csvAddLogInfo( (UINT)CSV_INFO_FILEFULL, 0, 0, t);
						return AddStatus;
					
					}
					
					
				} // loop through array elements //
								
			} // Array of variables //
				
			
			/****************************************/
			/* Clear current level					*/
			/****************************************/
			
			memset( &(pStructData->Level[pStructData->iLevel]), 0, sizeof(pStructData->Level[pStructData->iLevel]) );
			
				
			/****************************************/
			/* Find next item (iLevel, iItem)		*/
			/****************************************/
			
			while(1){
				
				
				/* Move up one level */
				
				pStructData->iLevel--;
				
				
				/* If you are above level 0,  
					all done */
				
				if( pStructData->iLevel < 0 ){
				
					pStructData->iLevel=	0;						
					
					return 0;

				} // Done //
				
	
				/* Increment iItem and check against dimension */
				
				pStructData->Level[pStructData->iLevel].iItem++;
	
	
				if( pStructData->Level[pStructData->iLevel].iItem < pStructData->Level[pStructData->iLevel].Dimension ){
			
					/* Need to do more with this structure or array, 
						keep going */
			
					break;
			
				} // keep going with this structure //
		
		
				else{
			
					/* Done with this structure or array, 
						clear this level and move up (stay in loop) */ 
			
					memset( &(pStructData->Level[pStructData->iLevel]), 0, sizeof(pStructData->Level[pStructData->iLevel]) );
						
				} // Move up one more level //
				
				
			} // Loop to go up levels //
				

			break;
		
		
	} // switch(DataType) //
	
			
} // Traverse loop //


return	CSV_ERR_BUSY;


} // End Fn //
