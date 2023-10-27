/********************************************************************
 * COPYRIGHT -- Automation Resources Group
 ********************************************************************
 * Library: CSVFileLib
 * File: csvProcessData.c
 * Author: David Blackburn
 * Created: January 04, 2012
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
/* Process data from file 											*/
/********************************************************************/

/* Check for null pointer				DONE and TESTED DFB 20120314 */
/* Check for initialized				DONE and TESTED DFB 20120406 */
/* Check for invalid operation			DONE and TESTED DFB 20120314 */

/* Parse file							DONE and TESTED DFB 20120406 */
	
	/* Get line							DONE and TESTED DFB 20120410 */
	
	/* Empty file (done)				DONE and TESTED DFB 20120410 */
		/* No line failures				DONE and TESTED DFB 20120410 */
		/* With line failures			DONE and TESTED DFB 20120410 */
	
	/* Copy line						DONE and TESTED DFB 20120410 */
		/* Check MaxLineLength			DONE and TESTED DFB 20120410 */
	
	/* Parse line						see cfmParseLine.st */
		/* No error						DONE and TESTED DFB 20120314 */
		/* Fatal error					DONE not possible */
		/* Comment						DONE and TESTED DFB 20120314 */
			/* Open						DONE and TESTED DFB 20120314 */
			/* Save						DONE and TESTED DFB 20150124 */
				/* Room in file			DONE and TESTED DFB 20120406 */
				/* Full file			DONE and TESTED DFB 20120410 */
		/* No value found				DONE and TESTED DFB 20120314 */
			/* Open						DONE and TESTED DFB 20120314 */
			/* Save						DONE and TESTED DFB 20120314 */
	
	/* Get Variable info				see cfmGetVarInfo.st */
		/* No error						DONE and TESTED DFB 20120314 */
		/* Error						DONE and TESTED DFB 20120314 */
			/* Open						DONE and TESTED DFB 20120314 */
			/* Save						DONE and TESTED DFB 20120406 */
				/* Room in file			DONE and TESTED DFB 20120406 */
				/* Full file			DONE and TESTED DFB 20120410 */
			
	
	/* Process variable					DONE and TESTED DFB 20120314 */
		
		/* Open							see cfmOpenVar.st */
			/* No error					DONE and TESTED DFB 20120314 */
			/* Error					DONE and TESTED DFB 20120314 */
			
		/* Save							see cfmSaveVar.st */
			/* No error					DONE and TESTED DFB 20120314 */
			/* File full				DONE and TESTED DFB 20120406 */
			/* Other error				DONE and TESTED DFB 20120406 */
				/* Room in file			DONE and TESTED DFB 20120406 */
				/* Full file			DONE and TESTED DFB 20120410 */
			
		/* Invalid operation			DONE not possible */
		
/* Busy									DONE and TESTED DFB 20120410 */


unsigned short csvProcessData(struct CSVFileMgr_typ* t)
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
	/* Check for invalid operation					*/
	/************************************************/

	if(		(t->Internal.CurrOperation != CSV_OP_OPEN)
		&&	(t->Internal.CurrOperation != CSV_OP_SAVE)
	){
	
		return CSV_ERR_INVALIDOPERATION;
	
	} /* Check for invalid operation */


	/************************************************/
	/* Process data									*/
	/************************************************/

	CSVFileVariable_typ	Variable;

	BOOL	GetInfo, ProcessVariable;

	UDINT	i;

	UINT	Status, AddStatus;

	STRING	Line[t->IN.CFG.MaxLineLength+1];


	/* Loop through lines of file, up to MaxLines */

	for(i=0; i<t->IN.CFG.MaxLinesPerScan; i++){
	
	
		/* Reset GetInfo, ProcessVariable, and Variable each iteration */
	
		GetInfo=			0;
		ProcessVariable=	0;
		memset( &(Variable), 0, sizeof(Variable) );
	
	
		/****************************************************************************/
		/* Parse file																*/
		/****************************************************************************/

		/* Get current line */
	
		t->Internal.pToken=	(UDINT)strtok_r( (char*)t->Internal.pString, "\r\n", (char**)&(t->Internal.pReentry) );
		t->Internal.LineNumber++;
	
		/* Set pString to 0 to get subsequent lines */
	
		t->Internal.pString=	0;
	
	
		if( t->Internal.pToken == 0 ){
	
	
			/* Empty string, check for any parsing errors and return */
	
			csvAddLogInfo( (UINT)CSV_INFO_DONE, t->Internal.LineNumber, 0, t);

			if( t->Internal.FailedLineCount != 0 ){
				return CSV_ERR_LINEFAILURE;
			}
			else{
				return 0;
			}

	
		} // No more lines //

		else{


			/* Copy line and put newline on end */
		
			memset( (char*)t->Internal.pLine, 0, t->IN.CFG.MaxLineLength+1 );
			strncpy( (char*)t->Internal.pLine, (char*)t->Internal.pToken, t->IN.CFG.MaxLineLength-2 );
			strcat( (char*)t->Internal.pLine, "\r\n" );


			/* If saving and there is a parsing error, want to write the original line back to the file */
			/* So, get a copy of the clean line now, since pLine will be modified while parsing */
		
			strcpy( Line, (char*)t->Internal.pLine );
	
		
			/* Parse line to get Variable Name, Value, and Description */
		
			Status=	csvParseLine( t->Internal.pLine, t->Internal.LineNumber, &Variable, t );
		
		
			/* Check parse status */
		
			switch( Status ){
			
			
				/* No error, GetInfo */
			
				case 0:	GetInfo=	1;	break;
			
			
				/* Fatal (also impossible) errors, increment FailedLineCount; if save, write line back to file */
			
				case CSV_ERR_INVALIDINPUT:
				case CSV_ERR_NOVARNAMEFOUND:
				
				
					t->Internal.FailedLineCount++;
			
			
					if( t->Internal.CurrOperation == CSV_OP_SAVE ){
					
					
					/* Add Line to write buffer and check for errors */
				
					AddStatus=	csvAddToBuffer( (UDINT)Line, strlen(Line), &(t->Internal.WriteBuffer) );
				
				
					if( AddStatus == CSV_ERR_INVALIDINPUT ){
					
					csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, t->Internal.LineNumber, (UDINT)"csvAddToBuffer()", t);
					return AddStatus;
					
					}
					else if( AddStatus == CSV_ERR_BUFFERFULL ){
					
					csvAddLogInfo( (UINT)CSV_INFO_FILEFULL, t->Internal.LineNumber, 0, t);
					return AddStatus;
					
					}

				
					} // if saving //
			
			
					break;
				
			
				/* Comment - if open, move on to next line; if save, write back to file */
			
				case CSV_ERR_COMMENT:
			
			
				/* If a comment was found, then do not increment Successful or FailedLineCount, but
					do write line back to file if saving (do not write old header comment) */
				
					if(		(t->Internal.CurrOperation == CSV_OP_SAVE)
						&&	(strncmp( Line, "// File generated ", strlen("// File generated ") ) != 0)
						&&	(strncmp( Line, "// Application Revision: ", strlen("// Application Revision: ") ) != 0)
						&&	(strncmp( Line, "// Configuration: ", strlen("// Configuration: ") ) != 0)
					){
					
					
						/* Add Line to write buffer and check for errors */
				
						AddStatus=	csvAddToBuffer( (UDINT)Line, strlen(Line), &(t->Internal.WriteBuffer) );
				
				
						if( AddStatus == CSV_ERR_INVALIDINPUT ){
					
							csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, t->Internal.LineNumber, (UDINT)"csvAddToBuffer()", t);
							return AddStatus;
					
						}
						else if( AddStatus == CSV_ERR_BUFFERFULL ){
					
							csvAddLogInfo( (UINT)CSV_INFO_FILEFULL, t->Internal.LineNumber, 0, t);
							return AddStatus;
					
						}
					
					
					} // if saving //

					
					break;
				
			
				/* No value found - if Open, increment FailedLineCount and move on to next line; if Save, GetInfo */
			
				case CSV_ERR_NOVALUEFOUND:
				
				
					if( t->Internal.CurrOperation == CSV_OP_OPEN ){
				
						t->Internal.FailedLineCount++;
				
					} // Open //
				
					else if( t->Internal.CurrOperation == CSV_OP_SAVE ){
				
						GetInfo=	1;
				
					} // Save //
				
					else{
	
						return CSV_ERR_INVALIDOPERATION;
	
					} // Invalid operation //
				
			
					break;	
		
		
			} // switch(ParseStatus) //
		

		} // Line found //	
	

		/****************************************************************************/
		/* Get variable address and info based on variable name						*/
		/****************************************************************************/
	
		if( GetInfo ){
		
			Status=	csvGetVarInfo( t->Internal.LineNumber, &Variable, t );
		
		
			/* Check status */
		
			if( Status == 0 ){
		
				ProcessVariable=	1;
		
			} // InfoFound //
		
			else{
			
			
			/* If there is an error getting the info, increment FailedLineCount 
				and write original line back to file if saving */
			
				t->Internal.FailedLineCount++;
			
			
				if( t->Internal.CurrOperation == CSV_OP_SAVE ){
			
			
					/* Add Line to write buffer and check for errors */
				
					AddStatus=	csvAddToBuffer( (UDINT)Line, strlen(Line), &(t->Internal.WriteBuffer) );
			
			
					if( AddStatus == CSV_ERR_INVALIDINPUT ){
				
						csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, t->Internal.LineNumber, (UDINT)"csvAddToBuffer()", t);
						return AddStatus;
				
					}
					else if( AddStatus == CSV_ERR_BUFFERFULL ){
				
						csvAddLogInfo( (UINT)CSV_INFO_FILEFULL, t->Internal.LineNumber, 0, t);
						return AddStatus;
				
					}
				
				
				} // if saving //
			
			
			} // error getting info //
		

		} // If GetInfo //
	
	
		/****************************************************************************/
		/* Process variable based on current operation								*/
		/****************************************************************************/
	
		if( ProcessVariable ){
		
		
			switch( t->Internal.CurrOperation ){
			
			
				case CSV_OP_OPEN:
			
				
					Status=	csvOpenVar( t->Internal.LineNumber, &Variable, t );
				
				
					/* Check status */
				
					if( Status == 0 ){
				
						t->Internal.SuccessfulLineCount++;
				
					}
					else{
				
						t->Internal.FailedLineCount++;
				
					}
		
		
					break;
			
			
				case CSV_OP_SAVE:
					
				
					Status=	csvSaveVar( t->Internal.LineNumber, &Variable, t );
				
				
					/* Check status */
				
					if( Status == 0 ){
				
						t->Internal.SuccessfulLineCount++;
				
					} // no error //
				
					else if( Status == CSV_ERR_BUFFERFULL ){
				
						return CSV_ERR_BUFFERFULL;
				
					} // File full //
				
					else{
				
					
					/* For other line save errors, increment FailedLineCount
						and write original line back to file */
						
						t->Internal.FailedLineCount++;
					
					
						/* Add Line to write buffer and check for errors */
				
						AddStatus=	csvAddToBuffer( (UDINT)Line, strlen(Line), &(t->Internal.WriteBuffer) );
				
				
						if( AddStatus == CSV_ERR_INVALIDINPUT ){
					
							csvAddLogInfo( (UINT)CSV_INFO_INVALIDINPUT, t->Internal.LineNumber, (UDINT)"csvAddToBuffer()", t);
							return AddStatus;
					
						}
						else if( AddStatus == CSV_ERR_BUFFERFULL ){
					
							csvAddLogInfo( (UINT)CSV_INFO_FILEFULL, t->Internal.LineNumber, 0, t);
							return AddStatus;
					
						}
					
				
					} // Other line error //
		
		
					break;
			
			
				default: return CSV_ERR_INVALIDOPERATION; break;
			
			
			} // Switch( CurrOperation ) //
		
		
		} // If ProcessVariable //
	
	
	} // Loop through lines of file //


	return CSV_ERR_BUSY;


} // End Fn //
