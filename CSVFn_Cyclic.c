/********************************************************************
 * COPYRIGHT -- Automation Resources Group
 ********************************************************************
 * Library: CSVFileLib
 * File: CSVFn_Cyclic.c
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
/* CSVFileMgr cyclic function	 									*/
/********************************************************************/

/* Check for null pointer					DONE and TESTED DFB 20120319 */

/* Check for initialized					DONE and TESTED DFB 20130628 */

/* Reset internal CMDs						DONE and TESTED DFB 20120322 */

/* Main state machine						DONE and TESTED DFB 20120803 */

	/* CSV_ST_IDLE							DONE and TESTED DFB 20120802 */
		/* Reset Done						DONE and TESTED DFB 20120322 */
		/* Check for new CMDs				DONE and TESTED DFB 20120802 */
			/* UpdateFile					DONE and TESTED DFB 20120802 */
				/* Set Internal CMD			DONE and TESTED DFB 20120319 */
				/* Log action				DONE and TESTED DFB 20131220 */
				/* Set Busy, Reset Done		DONE and TESTED DFB 20120319 */
				/* Clear buffers			DONE and TESTED DFB 20120410 */
				/* Write header				DONE and TESTED DFB 20120410 */
				/* Reset iVar, ScanCount	DONE and TESTED DFB 20120319 */
				/* Reset ExpandError		DONE and TESTED DFB 20120319 */
				/* Set CurrOperation		DONE and TESTED DFB 20120319 */
				/* Change State				DONE and TESTED DFB 20120319 */
				/* Check FileName			DONE and TESTED DFB 20120802 */
			/* SaveVariableListToFile		DONE and TESTED DFB 20120802 */
				/* Set Internal CMD			DONE and TESTED DFB 20120319 */
				/* Log action				DONE and TESTED DFB 20131220 */
				/* Set Busy, Reset Done		DONE and TESTED DFB 20120319 */
				/* Clear buffers			DONE and TESTED DFB 20120410 */
				/* Write header				DONE and TESTED DFB 20120410 */
				/* Reset iVar, ScanCount	DONE and TESTED DFB 20120319 */
				/* Clear StructureData		DONE and TESTED DFB 20120406 */
				/* Reset ExpandError		DONE and TESTED DFB 20120319 */
				/* Set CurrOperation		DONE and TESTED DFB 20120319 */
				/* Change State				DONE and TESTED DFB 20120319 */
				/* Check FileName			DONE and TESTED DFB 20120802 */
			/* OpenFile						DONE and TESTED DFB 20120802 */
				/* Set Internal CMD			DONE and TESTED DFB 20120319 */
				/* Log action				DONE and TESTED DFB 20131220 */
				/* Set Busy, Reset Done		DONE and TESTED DFB 20120319 */
				/* Clear ReadData, Log		DONE and TESTED DFB 20120406 */
				/* Reset iVar, ScanCount	DONE and TESTED DFB 20120319 */
				/* Reset ExpandError		DONE and TESTED DFB 20120319 */
				/* Set CurrOperation		DONE and TESTED DFB 20120319 */
				/* Change State				DONE and TESTED DFB 20120319 */				
				/* Check FileName			DONE and TESTED DFB 20120802 */
			/* DeleteFile					DONE and TESTED DFB 20120802 */
				/* Set Internal CMD			DONE and TESTED DFB 20120322 */
				/* Log action				DONE and TESTED DFB 20131220 */
				/* Set Busy, Reset Done		DONE and TESTED DFB 20120322 */
				/* Set CurrOperation		DONE and TESTED DFB 20120402 */
				/* Change State				DONE and TESTED DFB 20120322 */
				/* Check FileName			DONE and TESTED DFB 20120802 */
				
	/* CSV_ST_EXPANDVARS					see csvExpandVar.c */
		/* Not empty VarName				DONE and TESTED DFB 20120410 */
			/* Call csvExpandVar()			DONE and TESTED DFB 20120406 */
			/* Check ExpandStatus			DONE and TESTED DFB 20120320 */
				/* 0						DONE and TESTED DFB 20120410 */
				/* Busy						DONE and TESTED DFB 20120320 */
				/* Fatal error				DONE and TESTED DFB 20120406 */
				/* Non-fatal error			DONE and TESTED DFB 20120410 */
		/* Empty VarName					DONE and TESTED DFB 20120410 */
		
	/* CSV_ST_OPEN							DONE and TESTED DFB 20120410 */
		/* Set FIOWrap inputs				DONE and TESTED DFB 20120406 */
		/* Done								DONE and TESTED DFB 20120410 */
		/* Error							DONE and TESTED DFB 20120320 */

	/* CSV_ST_PROCESS						see csvProcessData.c */
		/* Bad errors						DONE and TESTED DFB 20120320 */
		/* Busy								DONE and TESTED DFB 20120410 */
		/* Other status						DONE and TESTED DFB 20120320 */
	
	/* CSV_ST_DELETELOG						DONE and TESTED DFB 20120802 */
		/* Set FIOWrap inputs				DONE and TESTED DFB 20120802 */
		/* Done								DONE and TESTED DFB 20120320 */
		/* Error							DONE and TESTED DFB 20120320 */
	
	/* CSV_ST_SAVELOG						DONE and TESTED DFB 20120802 */
		/* Set FIOWrap inputs				DONE and TESTED DFB 20120802 */
		/* Done								DONE and TESTED DFB 20120320 */
			/* Opening						DONE and TESTED DFB 20120320 */
			/* Saving						DONE and TESTED DFB 20120320 */
				/* Fatal Expand error		DONE and TESTED DFB 20120320 */
				/* Non-fatal Expand error	DONE and TESTED DFB 20120320 */
				/* Fatal Process error		DONE and TESTED DFB 20120320 */
				/* Non-fatal Process error	DONE and TESTED DFB 20120320 */
				/* No error					DONE and TESTED DFB 20120320 */
			/* Invalid Operation			DONE and TESTED DFB 20120320 */
		/* Error							DONE and TESTED DFB 20120320 */

	/* CSV_ST_DELETETEMP					DONE and TESTED DFB 20120803 */
		/* Set FIOWrap inputs				DONE and TESTED DFB 20120803 */
		/* Done								DONE and TESTED DFB 20120320 */
		/* Error							DONE and TESTED DFB 20120320 */
			
	/* CSV_ST_SAVETEMP						DONE and TESTED DFB 20120803 */
		/* Set FIOWrap inputs				DONE and TESTED DFB 20120803 */
		/* Done								DONE and TESTED DFB 20120320 */
		/* Error							DONE and TESTED DFB 20120320 */

	/* CSV_ST_DELETEORIGINAL				DONE and TESTED DFB 20120320 */
		/* Set FIOWrap inputs				DONE and TESTED DFB 20120320 */
		/* Done								DONE and TESTED DFB 20120320 */
		/* Error							DONE and TESTED DFB 20120320 */

	/* CSV_ST_RENAMETEMP					DONE and TESTED DFB 20120803 */
		/* Set FUB inputs					DONE and TESTED DFB 20120803 */
		/* Done								DONE and TESTED DFB 20120320 */
			/* No errors					DONE and TESTED DFB 20120320 */
			/* Expand error					DONE and TESTED DFB 20120320 */
			/* Other process errors			DONE and TESTED DFB 20120320 */
		/* Error							DONE and TESTED DFB 20120320 */
	
	/* CSV_ST_DELETEFILE					DONE and TESTED DFB 20120322 */
		/* Set FIOWrap inputs				DONE and TESTED DFB 20120322 */
		/* Done								DONE and TESTED DFB 20120322 */
		/* Error							DONE and TESTED DFB 20120322 */
			
	/* CSV_ST_ERROR							DONE and TESTED DFB 20120320 */
		/* Acknowledge behavior				DONE and TESTED DFB 20120320 */
		

/* Reset AcknowledgeError CMD				DONE and TESTED DFB 20120320 */

/* FIOWrap handling							DONE and TESTED DFB 20120320 */

/* Call FileRename FUB						DONE and TESTED DFB 20120320 */
	

unsigned short CSVFn_Cyclic(struct CSVFileMgr_typ* t)
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
	
	csvSetError( (UINT)CSV_ERR_NOTINITIALIZED, t );
	
	return (UINT)CSV_ERR_NOTINITIALIZED;
	
} /* Check for initialized */


/****************************************/
/* Reset internal CMDs					*/
/****************************************/

if( !(t->IN.CMD.OpenFile) ){
	
	t->Internal.CMD.OpenFile=	0;
	
}


if( !(t->IN.CMD.UpdateFile) ){
	
	t->Internal.CMD.UpdateFile=	0;
	
}


if( !(t->IN.CMD.SaveVariableListToFile) ){
	
	t->Internal.CMD.SaveVariableListToFile=	0;
	
}


if( !(t->IN.CMD.DeleteFile) ){
	
	t->Internal.CMD.DeleteFile=	0;
	
}


/****************************************/
/* Main State Machine					*/
/****************************************/

StrExtArgs_typ Args;


switch( t->OUT.STAT.State ){


	case CSV_ST_IDLE:
	
	
		/* If all CMDs are cleared, then clear Done status */
	
		if( 	!(t->IN.CMD.OpenFile)
  			&&	!(t->IN.CMD.UpdateFile)
			&&	!(t->IN.CMD.SaveVariableListToFile)
			&&	!(t->IN.CMD.DeleteFile)
			){
   
   			t->OUT.STAT.Done=	0;
			
		}
		
		
		/* Set CurrOperation to INVALID. It will be set below on leaving the state */
		
		t->Internal.CurrOperation=	CSV_OP_INVALID;
		
		
		/* Wait for new commands */
		
		if( t->IN.CMD.UpdateFile && !(t->Internal.CMD.UpdateFile) ){
		
			t->Internal.CMD.UpdateFile=	1;
			
			if( !t->IN.CFG.DisableLogging ){
				
				Args.s[0]=	(UDINT)&(t->IN.PAR.FileName);

				logInfo( t->IN.CFG.LoggerName, 0, "Update file '%s'", (UDINT)&(Args) );
				
			}
			
			t->OUT.STAT.Busy=	1;
			t->OUT.STAT.Done=	0;
			
			csvClearBuffer( &(t->Internal.ReadBuffer) );
			csvClearBuffer( &(t->Internal.LogBuffer) );
			csvClearBuffer( &(t->Internal.WriteBuffer) );
			
			csvWriteHeader( t );

			t->Internal.iVar=		0;
			t->Internal.ScanCount=	0;
						
			t->Internal.ExpandError=	0;
						
			t->Internal.pUserVariable=	t->IN.PAR.pUserVariable;
				
			//Get user variable if it is loaded

			if(t->Internal.pUserVariable != 0){

				t->OUT.STAT.State=			CSV_ST_GET_TEMPLATE;
				t->Internal.StartState=		CSV_ST_OPEN;

			}

			else{
				
				t->OUT.STAT.State=			CSV_ST_OPEN;
				
			}
			
			t->Internal.CurrOperation=	CSV_OP_SAVE;
		
			if( strcmp(t->IN.PAR.FileName, "") == 0 ) csvSetError( CSV_ERR_EMPTYFILENAME, t );
			
		}
		
		else if( t->IN.CMD.SaveVariableListToFile && !(t->Internal.CMD.SaveVariableListToFile) ){

			t->Internal.CMD.SaveVariableListToFile=	1;
			
			if( !t->IN.CFG.DisableLogging ){
				
				Args.s[0]=	(UDINT)&(t->IN.PAR.FileName);

				logInfo( t->IN.CFG.LoggerName, 0, "Save variable list to file '%s'", (UDINT)&(Args) );
				
			}
			
			t->OUT.STAT.Busy=	1;
			t->OUT.STAT.Done=	0;
						
			csvClearBuffer( &(t->Internal.ReadBuffer) );
			csvClearBuffer( &(t->Internal.LogBuffer) );
			csvClearBuffer( &(t->Internal.WriteBuffer) );
			
			csvWriteHeader( t );

			t->Internal.iVar=		0;
			t->Internal.ScanCount=	0;
			
			memset( &(t->Internal.StructureData), 0, sizeof(t->Internal.StructureData) );
			
			t->Internal.ExpandError=	0;
	
			t->Internal.pUserVariable=	t->IN.PAR.pUserVariable;

			//Get user variable if it is loaded

			if(t->Internal.pUserVariable != 0){

				t->OUT.STAT.State=			CSV_ST_GET_TEMPLATE;
				t->Internal.StartState=		CSV_ST_EXPANDVARS;

			}

			else{

				t->OUT.STAT.State=			CSV_ST_EXPANDVARS;
					
			}

			t->Internal.CurrOperation=	CSV_OP_SAVE;

			if( strcmp(t->IN.PAR.FileName, "") == 0 ) csvSetError( CSV_ERR_EMPTYFILENAME, t );
			
		}

		else if( t->IN.CMD.OpenFile && !(t->Internal.CMD.OpenFile) ){

			t->Internal.CMD.OpenFile=	1;
			
			if( !t->IN.CFG.DisableLogging ){
				
				Args.s[0]=	(UDINT)&(t->IN.PAR.FileName);

				logInfo( t->IN.CFG.LoggerName, 0, "Open file '%s'", (UDINT)&(Args) );
				
			}
			
			t->OUT.STAT.Busy=	1;
			t->OUT.STAT.Done=	0;
						
			csvClearBuffer( &(t->Internal.ReadBuffer) );
			csvClearBuffer( &(t->Internal.LogBuffer) );
						
			t->Internal.iVar=		0;
			t->Internal.ScanCount=	0;
						
			t->Internal.ExpandError=	0;
			
			t->Internal.pUserVariable=	t->IN.PAR.pUserVariable;
			
			//Get user variable if it is loaded

			if(t->Internal.pUserVariable != 0){

				t->OUT.STAT.State=			CSV_ST_GET_TEMPLATE;
				t->Internal.StartState=		CSV_ST_OPEN;

			}

			else{

				t->OUT.STAT.State=			CSV_ST_OPEN;

			}
			t->Internal.CurrOperation=	CSV_OP_OPEN;

			if( strcmp(t->IN.PAR.FileName, "") == 0 ) csvSetError( CSV_ERR_EMPTYFILENAME, t );
			
		}

		else if( t->IN.CMD.DeleteFile && !(t->Internal.CMD.DeleteFile) ){

			t->Internal.CMD.DeleteFile=	1;
			
			if( !t->IN.CFG.DisableLogging ){
				
				Args.s[0]=	(UDINT)&(t->IN.PAR.FileName);

				logInfo( t->IN.CFG.LoggerName, 0, "Delete file '%s'", (UDINT)&(Args) );
				
			}
			
			t->OUT.STAT.Busy=	1;
			t->OUT.STAT.Done=	0;
												
			t->OUT.STAT.State=			CSV_ST_DELETEFILE;
			t->Internal.CurrOperation=	CSV_OP_DELETE;

			if( strcmp(t->IN.PAR.FileName, "") == 0 ) csvSetError( CSV_ERR_EMPTYFILENAME, t );
			
		}


		break;
		
	case CSV_ST_GET_TEMPLATE:

			memset( t->Internal.TemplateVariable.Name, 0, sizeof(t->Internal.TemplateVariable.Name) );
						
			if( csvGetVarInfo(0,&t->Internal.TemplateVariable,t) == 0 ){

				t->OUT.STAT.State= t->Internal.StartState;
			
			}
				
			else{

				csvSetError( CSV_ERR_NOVARNAMEFOUND , t );//Maybe a better error number?
									
				strcpy( t->OUT.STAT.ErrorString, "Error reading template variable" );
		
			}
			
			break;
	
	case CSV_ST_EXPANDVARS:
		
		
		/* If Variable name is not empty, expand variable */
		
		if( strcmp( (char*)&(t->IN.PAR.VariableList[t->Internal.iVar]), "" ) != 0 ){
		
			t->Internal.ExpandStatus=	csvExpandVar(	(UDINT)&(t->IN.PAR.VariableList[t->Internal.iVar]),
														&(t->Internal.ReadBuffer),
														&(t->Internal.StructureData),
														t );
														
			
			/* Check ExpandStatus and act accordingly */
										
			switch( t->Internal.ExpandStatus ){
				
				
				case 0:
					
					
					/* Successful expansion. Move on to next variable */
					
					t->Internal.iVar++;
					
					if( t->Internal.iVar > CSV_MAI_VARLIST ){
						
						/* Done, move on */
						
						t->Internal.ScanCount=	0;
						
						t->Internal.LineNumber=				0;
			
						t->Internal.SuccessfulLineCount=	0;
						t->Internal.FailedLineCount=		0;
						
						t->Internal.pString=	t->Internal.ReadBuffer.pData;

						t->OUT.STAT.State=		CSV_ST_PROCESS;
						
					}
					
					break;
					
				
				case CSV_ERR_BUSY:
				
					/* Still expanding current variable. Go another scan */
					
					t->Internal.ScanCount++;
				
					break;
					
				
				case CSV_ERR_INVALIDINPUT:
				case CSV_ERR_BUFFERFULL:
					
					
					/* Fatal errors - write log and set error status later */
					
					t->Internal.ExpandError=	1;
					
					t->OUT.STAT.State=		CSV_ST_DELETELOG;	
						
					break;
				
				
				default:
				
					
					/* Non-fatal errors. Move on to next variable.
						Error information will be in the log. */
					
					t->Internal.ExpandError=	1;
					
					t->Internal.iVar++;
					
					if( t->Internal.iVar > CSV_MAI_VARLIST ){
						
						/* Done, move on */
						
						t->Internal.ScanCount=	0;
						
						t->Internal.LineNumber=				0;
			
						t->Internal.SuccessfulLineCount=	0;
						t->Internal.FailedLineCount=		0;
						
						t->Internal.pString=	t->Internal.ReadBuffer.pData;

						t->OUT.STAT.State=		CSV_ST_PROCESS;
						
					}
					
				
					break;
				
				
			} // switch(ExpandStatus) //
			
														
		} // Variable name not empty //
		
		else{
		
			/* Done, move on */
						
			t->Internal.ScanCount=	0;
			
			t->Internal.LineNumber=				0;
			
			t->Internal.SuccessfulLineCount=	0;
			t->Internal.FailedLineCount=		0;
						
			t->Internal.pString=	t->Internal.ReadBuffer.pData;

			t->OUT.STAT.State=		CSV_ST_PROCESS;
			
		} // Variable name empty //
		
		
		break;
	
	
	case CSV_ST_OPEN:
	
		
		/* Open file */
		
		t->Internal.FIOWrap.IN.CMD.Open=	1;
		
		strcpy( t->Internal.FIOWrap.IN.PAR.FileName, t->IN.PAR.FileName );
		
		t->Internal.FIOWrap.IN.PAR.pData=	t->Internal.ReadBuffer.pData;
		t->Internal.FIOWrap.IN.PAR.len=		t->Internal.ReadBuffer.MaxLength;
		t->Internal.FIOWrap.IN.PAR.offset=	0;
		
		
		if( t->Internal.FIOWrap.OUT.STAT.Done ){
			
			t->Internal.FIOWrap.IN.CMD.Open=	0;
			
			t->Internal.LineNumber=				0;
			
			t->Internal.SuccessfulLineCount=	0;
			t->Internal.FailedLineCount=		0;
						
			t->Internal.pString=	t->Internal.ReadBuffer.pData;

			t->OUT.STAT.State=	CSV_ST_PROCESS;
				
		}
		
		else if( t->Internal.FIOWrap.OUT.STAT.Error ){
			
			t->Internal.FIOWrap.IN.CMD.Open=	0;
			
			csvSetError( t->Internal.FIOWrap.OUT.STAT.ErrorID, t );
			
			
			/* If there is a FIOWrap error, copy ErrorString out */
			
			strncpy( t->OUT.STAT.ErrorString, t->Internal.FIOWrap.OUT.STAT.ErrorString, CSV_STRLEN_ERROR );
			
		}
		
	
		break;	
		
		
	case CSV_ST_PROCESS:
	
	
		/* Process file data and check status */
		
		t->Internal.ProcessStatus=	csvProcessData( t );
		
		
		switch( t->Internal.ProcessStatus ){
			
	
			/* Impossible errors - don't write log */
	
			case CSV_ERR_INVALIDINPUT:
			case CSV_ERR_NOTINITIALIZED:
			case CSV_ERR_INVALIDOPERATION: csvSetError( t->Internal.ProcessStatus, t ); break;
			
			/* Busy - keep going */
			
			case CSV_ERR_BUSY: t->Internal.ScanCount++; break;
			
			/* Other errors or all good - write log then check status again later */
					
			default: t->OUT.STAT.State=	CSV_ST_DELETELOG; break;
			
			
		} // switch(ProcessStatus) //
		
		
		break;
		

	case CSV_ST_DELETELOG:
	
		
		/* Delete the log file if it exists */
		
		t->Internal.FIOWrap.IN.CMD.Delete=	1;
		
		if( strcmp( t->IN.CFG.LogFileName, "" ) == 0 ){
			strcpy( t->Internal.FIOWrap.IN.PAR.FileName, ".csvlog" );
		}
		else{
			strcpy( t->Internal.FIOWrap.IN.PAR.FileName, t->IN.CFG.LogFileName );
		}

		if( t->Internal.FIOWrap.OUT.STAT.Done ){
			
			t->Internal.FIOWrap.IN.CMD.Delete=	0;
			
			t->OUT.STAT.State=	CSV_ST_SAVELOG;
				
		}
		
		else if( t->Internal.FIOWrap.OUT.STAT.Error ){
			
			t->Internal.FIOWrap.IN.CMD.Delete=	0;
			
			csvSetError( t->Internal.FIOWrap.OUT.STAT.ErrorID, t );
			
			
			/* If there is a FIOWrap error, copy ErrorString out */
			
			strncpy( t->OUT.STAT.ErrorString, t->Internal.FIOWrap.OUT.STAT.ErrorString, CSV_STRLEN_ERROR );
			
		}
		
		break;


	case CSV_ST_SAVELOG:
	
		
		/* Save the log file */
		
		t->Internal.FIOWrap.IN.CMD.SaveAs=	1;
		
		if( strcmp( t->IN.CFG.LogFileName, "" ) == 0 ){
			strcpy( t->Internal.FIOWrap.IN.PAR.FileName, ".csvlog" );
		}
		else{
			strcpy( t->Internal.FIOWrap.IN.PAR.FileName, t->IN.CFG.LogFileName );
		}

		t->Internal.FIOWrap.IN.PAR.pData=	t->Internal.LogBuffer.pData;
		t->Internal.FIOWrap.IN.PAR.len=		t->Internal.LogBuffer.CurrentLength;
		t->Internal.FIOWrap.IN.PAR.offset=	0;
		
		
		if( t->Internal.FIOWrap.OUT.STAT.Done ){
			
			t->Internal.FIOWrap.IN.CMD.SaveAs=	0;
			
			
			switch( t->Internal.CurrOperation ){
				
				
				case CSV_OP_OPEN:
					
					
					if( t->Internal.ProcessStatus == 0 ){
						
						t->OUT.STAT.Busy=	0;
						t->OUT.STAT.Done=	1;
						
						t->OUT.STAT.State=	CSV_ST_IDLE;
						
					}
					else{
						
						csvSetError( t->Internal.ProcessStatus, t );
					
					}
					
					
					break;
					
				
				case CSV_OP_SAVE:
					
					
					if( 	t->Internal.ExpandError
						&&	( (t->Internal.ExpandStatus == CSV_ERR_INVALIDINPUT) || (t->Internal.ExpandStatus == CSV_ERR_BUFFERFULL) )
						){
						
						csvSetError( t->Internal.ExpandStatus, t );
					
					} // Fatal expand error //
					
					
					else if( 	(t->Internal.ProcessStatus == 0)
							||	(t->Internal.ProcessStatus == CSV_ERR_LINEFAILURE)
							){
						
						t->OUT.STAT.State=	CSV_ST_DELETETEMP;
						
					} // Non-fatal or no process error //
					
					
					else{
						
						csvSetError( t->Internal.ProcessStatus, t );
					
					} // Fatal process error //
					
					
					break;
				
					
				default: csvSetError(CSV_ERR_INVALIDOPERATION, t); break;
				
				
			} // switch(CurrOperation) //
			
			
		} // Done saving log //
		
		else if( t->Internal.FIOWrap.OUT.STAT.Error ){
			
			t->Internal.FIOWrap.IN.CMD.SaveAs=	0;
			
			csvSetError( t->Internal.FIOWrap.OUT.STAT.ErrorID, t );
			
			
			/* If there is a FIOWrap error, copy ErrorString out */
			
			strncpy( t->OUT.STAT.ErrorString, t->Internal.FIOWrap.OUT.STAT.ErrorString, CSV_STRLEN_ERROR );
			
		} // Error saving log //
		
		break;


	case CSV_ST_DELETETEMP:
	
		
		/* Delete the temporary file if it exists */
		
		t->Internal.FIOWrap.IN.CMD.Delete=	1;
		
		strcpy( t->Internal.TempFileName, t->IN.PAR.FileName );
		strcat( t->Internal.TempFileName, ".temp" );
		
		//Make temp file hidden
		STRING tempstring[CSV_STRLEN_TEMPFILENAME] = "";
		int iter = strlen(t->Internal.TempFileName) -1;

		//Start at the end and look for the first folder.
		for( ; iter >= 0 ; iter-- ){
		
			//Check if this is a folder, and if the name contains a '.' indicating a hidden file
			if(((char*)t->Internal.TempFileName)[iter]=='\\' && ((char*)t->Internal.TempFileName)[iter+1]!='.' )
			{
				//Save the string after the folder
				strcpy(tempstring,((char*)t->Internal.TempFileName + iter+1));

				//append a '.null'
				((char*)t->Internal.TempFileName)[iter+1]='.';
				((char*)t->Internal.TempFileName)[iter+2]=0;				

				//Concat the file name to the folder\. to make the file hidden.
				strcat(t->Internal.TempFileName,tempstring);			
				break;
			}
		}

		strcpy( t->Internal.FIOWrap.IN.PAR.FileName, t->Internal.TempFileName );
		

		if( t->Internal.FIOWrap.OUT.STAT.Done ){
			
			t->Internal.FIOWrap.IN.CMD.Delete=	0;
			
			t->OUT.STAT.State=	CSV_ST_SAVETEMP;
				
		}
		
		else if( t->Internal.FIOWrap.OUT.STAT.Error ){
			
			t->Internal.FIOWrap.IN.CMD.Delete=	0;
			
			csvSetError( t->Internal.FIOWrap.OUT.STAT.ErrorID, t );
			
			
			/* If there is a FIOWrap error, copy ErrorString out */
			
			strncpy( t->OUT.STAT.ErrorString, t->Internal.FIOWrap.OUT.STAT.ErrorString, CSV_STRLEN_ERROR );
			
		}
		
		break;


	case CSV_ST_SAVETEMP:
	
		
		/* Save the file to a temporary file name */
		
		t->Internal.FIOWrap.IN.CMD.SaveAs=	1;
		
		strcpy( t->Internal.FIOWrap.IN.PAR.FileName, t->Internal.TempFileName );
		
		t->Internal.FIOWrap.IN.PAR.pData=	t->Internal.WriteBuffer.pData;
		t->Internal.FIOWrap.IN.PAR.len=		t->Internal.WriteBuffer.CurrentLength;
		t->Internal.FIOWrap.IN.PAR.offset=	0;
		
		
		if( t->Internal.FIOWrap.OUT.STAT.Done ){
			
			t->Internal.FIOWrap.IN.CMD.SaveAs=	0;
			
			t->OUT.STAT.State=	CSV_ST_DELETEORIGINAL;
				
		}
		
		else if( t->Internal.FIOWrap.OUT.STAT.Error ){
			
			t->Internal.FIOWrap.IN.CMD.SaveAs=	0;
			
			csvSetError( t->Internal.FIOWrap.OUT.STAT.ErrorID, t );
			
			
			/* If there is a FIOWrap error, copy ErrorString out */
			
			strncpy( t->OUT.STAT.ErrorString, t->Internal.FIOWrap.OUT.STAT.ErrorString, CSV_STRLEN_ERROR );
			
		}
		
		break;


	case CSV_ST_DELETEORIGINAL:
	
		
		/* Delete the original file */
		
		t->Internal.FIOWrap.IN.CMD.Delete=	1;
		
		strcpy( t->Internal.FIOWrap.IN.PAR.FileName, t->IN.PAR.FileName );
		
		
		if( t->Internal.FIOWrap.OUT.STAT.Done ){
			
			t->Internal.FIOWrap.IN.CMD.Delete=	0;
			
			t->OUT.STAT.State=	CSV_ST_RENAMETEMP;
				
		}
		
		else if( t->Internal.FIOWrap.OUT.STAT.Error ){
			
			t->Internal.FIOWrap.IN.CMD.Delete=	0;
			
			csvSetError( t->Internal.FIOWrap.OUT.STAT.ErrorID, t );
			
			
			/* If there is a FIOWrap error, copy ErrorString out */
			
			strncpy( t->OUT.STAT.ErrorString, t->Internal.FIOWrap.OUT.STAT.ErrorString, CSV_STRLEN_ERROR );
			
		}
		
		break;	
		
		
	case CSV_ST_RENAMETEMP:
	
		
		/* Rename temp file to FileName */
		
		t->Internal.FileRename.enable=		1;
		t->Internal.FileRename.pDevice=		(UDINT)&(t->IN.PAR.FileDevice);
		t->Internal.FileRename.pName=		(UDINT)&(t->Internal.TempFileName);
		t->Internal.FileRename.pNewName=	(UDINT)&(t->IN.PAR.FileName);
		
		
		/* Check status */
		
		if( t->Internal.FileRename.status < ERR_FUB_ENABLE_FALSE ){
  			
			
			/* Turn off FUB */
			
			t->Internal.FileRename.enable=	0;
  

  			if( t->Internal.FileRename.status == 0 ){
   				
			
				/* All good, Check ProcessStatus and ExpandError */
				
				if(		(t->Internal.ProcessStatus == 0)
					&&	!(t->Internal.ExpandError)
					){
				
					t->OUT.STAT.Busy=	0;
					t->OUT.STAT.Done=	1;
				
					t->OUT.STAT.State=	CSV_ST_IDLE;
				
				} // No error while expanding or processing //
				
				
				else if( t->Internal.ExpandError ){
				
					 /* Line failure errors will direct the user to the log file. */
					 
					csvSetError( CSV_ERR_LINEFAILURE, t );
					
				} // Error while expanding //
					
				
				else{
				
					csvSetError( t->Internal.ProcessStatus, t );	
				
				} // Error while processing //
								
								
			}	
			else {
			  	
				csvSetError( t->Internal.FileRename.status, t );
				
			} // If error //
			
		} // If done //
		
		
		break;	


	case CSV_ST_DELETEFILE:
	
		
		t->Internal.FIOWrap.IN.CMD.Delete=	1;
		
		strcpy( t->Internal.FIOWrap.IN.PAR.FileName, t->IN.PAR.FileName );
		
		
		if( t->Internal.FIOWrap.OUT.STAT.Done ){
			
			t->Internal.FIOWrap.IN.CMD.Delete=	0;
			
			t->OUT.STAT.Busy=	0;
			t->OUT.STAT.Done=	1;
				
			t->OUT.STAT.State=	CSV_ST_IDLE;
				
		}
		
		else if( t->Internal.FIOWrap.OUT.STAT.Error ){
			
			t->Internal.FIOWrap.IN.CMD.Delete=	0;
			
			csvSetError( t->Internal.FIOWrap.OUT.STAT.ErrorID, t );
			
			
			/* If there is a FIOWrap error, copy ErrorString out */
			
			strncpy( t->OUT.STAT.ErrorString, t->Internal.FIOWrap.OUT.STAT.ErrorString, CSV_STRLEN_ERROR );
			
		}
		
		break;	
		
		
	/********************************************************************************/
	/* Error state																	*/
	/********************************************************************************/
		
	case CSV_ST_ERROR:
	
		
		if(	t->IN.CMD.AcknowledgeError ){
			
			t->Internal.FIOWrap.IN.CMD.AcknowledgeError=	1;
			
			t->OUT.STAT.Error=		0;
			t->OUT.STAT.ErrorID=	0;
			strcpy( t->OUT.STAT.ErrorString, "" );
			t->OUT.STAT.ErrorLevel=	0;
			t->OUT.STAT.ErrorState=	0;
			
			t->OUT.STAT.State=		CSV_ST_IDLE;
			
   		}
		
		break;
				

} // Main State Machine //


/* Reset AcknowledgeError CMD */

t->IN.CMD.AcknowledgeError=	0;


/****************************************/
/* Handle File IO						*/
/****************************************/

/* Set inputs */

strcpy( t->Internal.FIOWrap.IN.PAR.FileDevice, t->IN.PAR.FileDevice );


/* Call FIOWrapFn_Cyclic */

FIOWrapFn_Cyclic( &(t->Internal.FIOWrap) );


/* Call FileRename FUB */

FileRename( &(t->Internal.FileRename) );


return 0;
	
	
} // End Fn //
