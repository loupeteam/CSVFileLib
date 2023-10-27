/********************************************************************
 * COPYRIGHT -- Automation Resources Group
 ********************************************************************
 * Library: CSVFileLib
 * File: CSVFn_HMI_Cyclic.c
 * Author: David Blackburn
 * Created: March 22, 2012
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
/* CSVFileMgr cyclic HMI function 									*/
/********************************************************************/

/* Check for null pointers				DONE and TESTED DFB 20120402 */

/* Initialize HMI						DONE and TESTED DFB 20120402 */

/* Lock buttons appropriately			DONE and TESTED DFB 20120402 */

/* Handling new CMDs					DONE and TESTED DFB 20120402 */

	/* OpenFile							DONE and TESTED DFB 20120402 */
		/* Layer handling				DONE and TESTED DFB 20120402 */
		/* Action						DONE and TESTED DFB 20120402 */
		/* Cancel						DONE and TESTED DFB 20120402 */
		
	/* UpdateFile						DONE and TESTED DFB 20120402 */
		/* Layer handling				DONE and TESTED DFB 20120402 */
		/* Action						DONE and TESTED DFB 20120402 */
		/* Cancel						DONE and TESTED DFB 20120402 */
		
	/* SaveVariableListToFile			DONE and TESTED DFB 20120402 */
		/* Layer handling				DONE and TESTED DFB 20120402 */
		/* Action						DONE and TESTED DFB 20120402 */
		/* Cancel						DONE and TESTED DFB 20120402 */
		
	/* DeleteFile						DONE and TESTED DFB 20120402 */
		/* Layer handling				DONE and TESTED DFB 20120402 */
		/* Action						DONE and TESTED DFB 20120402 */
			/* Yes						DONE and TESTED DFB 20120402 */
			/* No						DONE and TESTED DFB 20120402 */
		/* Cancel						DONE and TESTED DFB 20120402 */
				
/* Check CSVFileMgr.Done				DONE and TESTED DFB 20120402 */
	/* Reset HMI CMDs					DONE and TESTED DFB 20120402 */
	/* Leave non-HMI CMDs				DONE and TESTED DFB 20120402 */
	/* Refresh FileList					DONE and TESTED DFB 20120402 */
		
/* FileName handling					DONE and TESTED DFB 20120402 */
	/* PopulateFileList					see csvhmiPopulateFileList.st */
	/* Refresh							DONE and TESTED DFB 20120402 */
	/* PageUp							DONE and TESTED DFB 20120402 */
	/* PageDown							DONE and TESTED DFB 20120402 */
	/* Completion						DONE and TESTED DFB 20120402 */
	/* Copy to CSVFileMgr				DONE and TESTED DFB 20120402 */

/* Progress 							DONE and TESTED DFB 20120402 */
	/* Layer							DONE and TESTED DFB 20120402 */
	/* Timer							DONE and TESTED DFB 20120402 */
	/* Text Snippet						DONE and TESTED DFB 20120402 */

/* Error handling						DONE and TESTED DFB 20120402 */
	/* Layer							DONE and TESTED DFB 20120402 */
	/* Reset HMI CMDs					DONE and TESTED DFB 20120402 */
	/* Error text snippets				DONE and TESTED DFB 20120402 */
	/* AcknowledgeError CMD				DONE and TESTED DFB 20120402 */
	

unsigned short CSVFn_HMI_Cyclic(CSVFileMgr_HMI_typ* HMI, struct CSVFileMgr_typ* t)
{


	/************************************************/
	/* Check for invalid inputs						*/
	/************************************************/

	if( 	(HMI == 0)
		||	(t == 0)
	){
	
		return (UINT)CSV_ERR_INVALIDINPUT;
		
	} // Check for null pointer //


	/********************************************************************/
	/* Lock buttons if appropriate				 						*/
	/********************************************************************/

	if( t->OUT.STAT.Busy ){

		HMI_Lock( &(HMI->Main.OpenFile.StatusDP) );
		HMI_Lock( &(HMI->Main.UpdateFile.StatusDP) );
		HMI_Lock( &(HMI->Main.SaveVariableListToFile.StatusDP) );
		HMI_Lock( &(HMI->Main.DeleteFile.StatusDP) );
	
		HMI_Lock( &(HMI->DialogBox.Action.StatusDP) );
		HMI_Lock( &(HMI->DialogBox.Cancel.StatusDP) );
		HMI_Lock( &(HMI->DialogBox.PageUp.StatusDP) );
		HMI_Lock( &(HMI->DialogBox.PageDown.StatusDP) );

		HMI_Lock( &(HMI->DeleteConfirm.Yes.StatusDP) );
		HMI_Lock( &(HMI->DeleteConfirm.No.StatusDP) );

		HMI_Lock( &(HMI->ErrorStatus.AcknowledgeError.StatusDP) );

	}
	else {
	
		HMI_Unlock( &(HMI->Main.OpenFile.StatusDP) );
		HMI_Unlock( &(HMI->Main.UpdateFile.StatusDP) );
		HMI_Unlock( &(HMI->Main.SaveVariableListToFile.StatusDP) );
		HMI_Unlock( &(HMI->Main.DeleteFile.StatusDP) );
	
		HMI_Unlock( &(HMI->DialogBox.Action.StatusDP) );
		HMI_Unlock( &(HMI->DialogBox.Cancel.StatusDP) );
		HMI_Unlock( &(HMI->DialogBox.PageUp.StatusDP) );
		HMI_Unlock( &(HMI->DialogBox.PageDown.StatusDP) );
		
		HMI_Unlock( &(HMI->DeleteConfirm.Yes.StatusDP) );
		HMI_Unlock( &(HMI->DeleteConfirm.No.StatusDP) );

		HMI_Unlock( &(HMI->ErrorStatus.AcknowledgeError.StatusDP) );
	
	}

	/********************************************************************/
	/* Handle new commands 												*/
	/********************************************************************/

	HMI_Hide( &(HMI->DialogBox.StatusDP) );
	HMI_Hide( &(HMI->DeleteConfirm.StatusDP) );
	
	if( HMI->Main.OpenFile.CMD ){
		
		/* Show dialog box */
		HMI_Show( &(HMI->DialogBox.StatusDP) );
	
	
		/* Set Action text snippet string */
		strcpy( HMI->TextSnippets.PopupHeader, "Open:" );
		strcpy( HMI->TextSnippets.Action, "Open" );

	
		/* Wait for action or cancel */
	
		if( HMI->DialogBox.Action.CMD ){
			
			/* Reset Action CMD */
			HMI->DialogBox.Action.CMD=	0;
		
			
			//Check if a folder is selected
			
			if( strncmp(HMI->DialogBox.FileNameList[HMI->DialogBox.FileBrowser.FileIndex], "\\", 1) == 0 )
			{
				HMI->DialogBox.FileBrowser.BrowseToDirectory= 1;//browse to selected folder
			}
			else//file is selected
			{
			
				/* Set IN.CMD */
				t->IN.CMD.OpenFile=		1;
			
				/* Reset OpenFile CMD */
				HMI->Main.OpenFile.CMD=	0;
			
				/* Set HMI_CMD_Active */
				HMI->Internal.HMI_CMD_Active=	1;
		
			}
		
		
		
		}
		else if( HMI->DialogBox.Cancel.CMD )
		{		
			//reset any filtering/selections
//			strcpy(HMI->DialogBox.FileBrowser.FileSelection, "");
			
			/* Reset both HMI CMDs */
			HMI->Main.OpenFile.CMD=		0;
			HMI->DialogBox.Cancel.CMD=	0;
		}
	
	} // OpenFile //


	else if( HMI->Main.UpdateFile.CMD ){

		/* Show dialog box */
		HMI_Show( &(HMI->DialogBox.StatusDP) );
	
		/* Set Action text snippet string */
		//Check if a folder is selected
		if( strncmp( HMI->DialogBox.FileNameList[HMI->DialogBox.FileBrowser.FileIndex], "\\", 1) == 0)
		{
			strcpy( HMI->TextSnippets.PopupHeader, "Update:" );
			strcpy( HMI->TextSnippets.Action, "Open" );
		}
		else//file selected
		{
			strcpy( HMI->TextSnippets.PopupHeader, "Update:" );
			strcpy( HMI->TextSnippets.Action, "Update" );
		}
		
		/* Wait for action or cancel */
	
		if( HMI->DialogBox.Action.CMD )
		{
			
			/* Reset both HMI CMDs */
			HMI->Main.UpdateFile.CMD=	0;//close popup window
			HMI->DialogBox.Action.CMD=	0;
			
			//Check if a folder is selected
			if (strncmp( HMI->DialogBox.FileBrowser.FileSelection, "\\", 1 ) == 0)
			{
				HMI->DialogBox.FileBrowser.BrowseToDirectory= 1;//browse to selected folder
			}
			else//file is selected
			{				
				/* Set IN.CMD */
				t->IN.CMD.UpdateFile=		1;

				/* Set HMI_CMD_Active */
				HMI->Internal.HMI_CMD_Active=	1;
			}	
		
		}
		else if( HMI->DialogBox.Cancel.CMD ){
			
			//reset any filtering/selections
			strcpy(HMI->DialogBox.FileBrowser.FileSelection, "");
			
			/* Reset both HMI CMDs */
			HMI->Main.UpdateFile.CMD=	0;
			HMI->DialogBox.Cancel.CMD=	0;
	
		}
	
	} // Update File //	
	
	
	else if( HMI->Main.SaveVariableListToFile.CMD ){
		
		/* Show dialog box */

		HMI_Show( &(HMI->DialogBox.StatusDP) );
	
	
		/* Set Action text snippet string */
		//Check if a folder is selected
		if( strncmp( HMI->DialogBox.FileNameList[HMI->DialogBox.FileBrowser.FileIndex], "\\", 1) == 0)
		{
			strcpy( HMI->TextSnippets.PopupHeader, "Save:" );
			strcpy( HMI->TextSnippets.Action, "Open" );
		}
		else//file selected
		{
			strcpy( HMI->TextSnippets.PopupHeader, "Save:" );
			strcpy( HMI->TextSnippets.Action, "Save" );
		}
	
		
		/* Wait for action or cancel */
	
		if( HMI->DialogBox.Action.CMD )
		{
			/* Reset both HMI CMDs */
			HMI->DialogBox.Action.CMD=	0;
			
			//Check if a folder is selected
			if (strncmp( HMI->DialogBox.FileBrowser.FileSelection, "\\", 1 ) == 0)
			{
				HMI->DialogBox.FileBrowser.BrowseToDirectory= 1;//browse to selected folder
			}
			else//file is selected
			{
			
				// If the filename contains does not contain a '.' then append file extension //
				if(		(strstr(HMI->DialogBox.FileBrowser.FileSelection, ".") == 0)
					&&	(strcmp(HMI->DialogBox.FileBrowser.FileExtSelection, "") != 0)
				){
					strcat(HMI->DialogBox.FileBrowser.FileSelection, ".");
					strcat(HMI->DialogBox.FileBrowser.FileSelection, HMI->DialogBox.FileBrowser.FileExtSelection);
				}
				
				/* Set IN.CMD */
				t->IN.CMD.SaveVariableListToFile=		1;
				
				/* Reset CMD */
				HMI->Main.SaveVariableListToFile.CMD=	0;//close popup window
	
				/* Set HMI_CMD_Active */
				HMI->Internal.HMI_CMD_Active=	1;

			}	
			
		}
		else if( HMI->DialogBox.Cancel.CMD ){
			
			//reset any filtering/selections
//			strcpy(HMI->DialogBox.FileBrowser.FileSelection, "");
			
			/* Reset both HMI CMDs */
		
			HMI->Main.SaveVariableListToFile.CMD=	0;
			HMI->DialogBox.Cancel.CMD=	0;
	
		}
	
	} // Save Variable List To File //	
	

	else if( HMI->Main.DeleteFile.CMD ){
		
		/* Show dialog box */

		HMI_Show( &(HMI->DialogBox.StatusDP) );
	
	
		/* Set Action text snippet string */
		if( strncmp(HMI->DialogBox.FileNameList[HMI->DialogBox.FileBrowser.FileIndex], "\\", 1) == 0)
		{
			strcpy( HMI->TextSnippets.PopupHeader, "Delete:" );
			strcpy( HMI->TextSnippets.Action, "Open" );
		}
		else//file selected
		{
			strcpy( HMI->TextSnippets.PopupHeader, "Delete:" );
			strcpy( HMI->TextSnippets.Action, "Delete" );
		}
	
		/* Wait for action or cancel */
	
		if( HMI->DialogBox.Action.CMD ){
			
			//Check if a folder is selected
			if( strncmp(HMI->DialogBox.FileNameList[HMI->DialogBox.FileBrowser.FileIndex], "\\", 1) == 0)
			{
				HMI->DialogBox.Action.CMD= 0;
				HMI->DialogBox.FileBrowser.BrowseToDirectory= 1;//browse to selected folder
			}
			else//file is selected
			{
				/* Show Delete Confirm Layer */
				HMI_Show( &(HMI->DeleteConfirm.StatusDP) );	
				/* Populate FileName Text Snippet string */
				strcpy( HMI->TextSnippets.FileName, HMI->DialogBox.FileBrowser.FileSelection );
			}	
			
			/* Wait for Yes or No */
			if( HMI->DeleteConfirm.Yes.CMD )
			{	
				/* Reset all HMI CMDs */
				HMI->Main.DeleteFile.CMD=	0;
				HMI->DialogBox.Action.CMD=	0;
				HMI->DeleteConfirm.Yes.CMD=	0;
		
				/* Set IN.CMD */
				t->IN.CMD.DeleteFile=	1;
		
				/* Set HMI_CMD_Active */
				HMI->Internal.HMI_CMD_Active=	1;
			}
			else if( HMI->DeleteConfirm.No.CMD ){

				/* Reset two HMI CMDs */
		
				HMI->DialogBox.Action.CMD=	0;
				HMI->DeleteConfirm.No.CMD=	0;
	
			} // Yes or No //
		
		}
		else if( HMI->DialogBox.Cancel.CMD ){
			
			//reset any filtering/selections
//			strcpy(HMI->DialogBox.FileBrowser.FileSelection, "");
			
			/* Reset both HMI CMDs */
		
			HMI->Main.DeleteFile.CMD=	0;
			HMI->DialogBox.Cancel.CMD=	0;
	
	
		}
	
	} // Delete file //


	/********************************************************************/
	/* Reset completed HMI commands										*/
	/********************************************************************/

	if( 	(t->OUT.STAT.Done)
		&&	(HMI->Internal.HMI_CMD_Active)
	){
	
		/* Reset all IN.CMDs if the IN.CMD was set by the HMI */
	
		t->IN.CMD.OpenFile=		0;
		t->IN.CMD.UpdateFile=	0;
		t->IN.CMD.SaveVariableListToFile=	0;
		t->IN.CMD.DeleteFile=	0;
	
		/* Reset HMI_CMD_Active */
	
		HMI->Internal.HMI_CMD_Active=	0;

		
		//reset any filtering/selections
//		strcpy(HMI->DialogBox.FileBrowser.FileSelection, "");

	}


	/********************************************************************/
	/* File Browser 													*/
	/********************************************************************/
	
	HMI->DialogBox.FileBrowser.Enable=					1;
	HMI->DialogBox.FileBrowser.EnableDirectorySearch=	1;
	strcpy(HMI->DialogBox.FileBrowser.FileDevice, t->IN.PAR.FileDevice);
	HMI->DialogBox.FileBrowser.pFileList=				(UDINT)&(HMI->DialogBox.FileNameList);
	HMI->DialogBox.FileBrowser.MaxFiles=				(sizeof(HMI->DialogBox.FileNameList)/sizeof(HMI->DialogBox.FileNameList[0]));
	
	HMIFileBrowse( &(HMI->DialogBox.FileBrowser) );

	
	/********************************************************************/
	/* Handle FileName													*/
	/********************************************************************/

	if( HMI->Internal.HMI_CMD_Active
	&& !HMI->Internal.HMI_CMD_Active_Old
	){
		HMI->Internal.HMI_CMD_Active_Old=	1;
		memset( &(t->IN.PAR.FileName), 0, sizeof(t->IN.PAR.FileName) );
		strcpy(t->IN.PAR.FileName, HMI->DialogBox.FileBrowser.CurrentPath);
		strcat(t->IN.PAR.FileName, HMI->DialogBox.FileBrowser.FileSelection); // this now works with the current FileBrowse implementation
	} // if(HMI_CMD_Active) //
	else if(!HMI->Internal.HMI_CMD_Active){
		HMI->Internal.HMI_CMD_Active_Old=	0;		
//		if(HMI->DialogBox.StatusDP == 1){			
//			strcpy(HMI->DialogBox.FileBrowser.FileSelection,t->IN.PAR.FileName);
//		}
	}

	/********************************************************************/
	/* Handle Progress Layer				 							*/
	/********************************************************************/

	/* Layer visibility */

	if( t->OUT.STAT.Busy ){
		HMI_Show( &(HMI->Progress.StatusDP) );
	}
	else{
		HMI_Hide( &(HMI->Progress.StatusDP) );
	}


	/* Timer */

	HMI->Internal.ProgressTimer.IN=	t->OUT.STAT.Busy;
	HMI->Internal.ProgressTimer.PT=	1000000000;
	TON_10ms( &(HMI->Internal.ProgressTimer) );

	/* Text Snippet */

	switch( t->Internal.CurrOperation ){
	
		case CSV_OP_OPEN:	strcpy( HMI->TextSnippets.ProgressLabel, "Opening..." ); 
			if(t->Internal.FIOWrap.Internal.FileLen>0){
				//Estimate a line is 60 characters
				HMI->Progress.Progress=	(t->Internal.LineNumber*50)*100/t->Internal.FIOWrap.Internal.FileLen;
			}
			break;
		case CSV_OP_SAVE:	strcpy( HMI->TextSnippets.ProgressLabel, "Saving..." ); 
			HMI->Progress.Progress=	HMI->Internal.ProgressTimer.ET/10;
			break;
		case CSV_OP_DELETE:	strcpy( HMI->TextSnippets.ProgressLabel, "Deleting..." ); 
			HMI->Progress.Progress=	HMI->Internal.ProgressTimer.ET/10;
			break;
		default:			strcpy( HMI->TextSnippets.ProgressLabel, "..." ); 
			HMI->Progress.Progress=	HMI->Internal.ProgressTimer.ET/10;
			break;
	
	} // switch(CurrOperation) //


	/****************************************************************/
	/* Error handling												*/
	/****************************************************************/

	HMI_Hide( &(HMI->ErrorStatus.StatusDP) );


	if( t->OUT.STAT.Error ){
		
	
		/* Show ErrorStatus layer */
		
		HMI_Show( &(HMI->ErrorStatus.StatusDP) );


		/* Reset HMI commands */
	
		if( HMI->Internal.HMI_CMD_Active ){
	
	
			/* Reset all IN.CMDs if the IN.CMD was set by the HMI */
	
			t->IN.CMD.OpenFile=		0;
			t->IN.CMD.UpdateFile=	0;
			t->IN.CMD.SaveVariableListToFile=	0;
			t->IN.CMD.DeleteFile=	0;
	
	
			/* Reset HMI_CMD_Active */
	
			HMI->Internal.HMI_CMD_Active=	0;

		}

		/* Set Error Text Snippets */

		HMI->TextSnippets.ErrorID=	t->OUT.STAT.ErrorID;
		strcpy( HMI->TextSnippets.ErrorString, t->OUT.STAT.ErrorString );


	} // if(Error) //


	if( HMI->ErrorStatus.AcknowledgeError.CMD ){

		HMI->ErrorStatus.AcknowledgeError.CMD=	0;

		t->IN.CMD.AcknowledgeError=	1;

	} // AcknowledgeError //


	return 0;


} // End Fn //
