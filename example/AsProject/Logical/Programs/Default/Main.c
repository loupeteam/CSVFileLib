
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <string.h>


/* ------------------------------------------------------------------ */
/* Bit string (BYTE/WORD/DWORD) test suite for CSVFileLib              */
/*                                                                     */
/* Runs once a few scans after boot; set runTest to run it again.      */
/*                                                                     */
/* The suite PASSED only when suiteOk is 1. It is cleared at the start  */
/* of every run and set only at the end, after the full assertion       */
/* count has been checked - so a run that fails part way and skips the  */
/* end, or one that never completes, leaves it 0. Checking testFail     */
/* alone is not enough: that stays 0 when a phase never ran.            */
/* ------------------------------------------------------------------ */

/* Every assertion the suite makes, counted at the call sites:
	1 PV_ninfo + 41 read + 12 write + 36 round trip + 1 file round trip
	+ 1 crafted.csv + 1 badline.csv */

#define CSVTEST_EXPECTED_ASSERTIONS 93


typedef struct readCase_typ {
	char*			Value;			/* what the file contains */
	unsigned long	DataType;		/* CSV_TYPE_BYTE / WORD / DWORD */
	unsigned short	ExpectStatus;	/* 0, or the CSV_ERR_ expected */
	unsigned long	ExpectStored;	/* only checked when ExpectStatus is 0 */
} readCase_typ;


static const readCase_typ readCases[] = {

	/* --- every accepted prefix, DWORD --- */
	{ "0xFF",			CSV_TYPE_DWORD,	0,	0xFF		},
	{ "0XFF",			CSV_TYPE_DWORD,	0,	0xFF		},
	{ "16#FF",			CSV_TYPE_DWORD,	0,	0xFF		},
	{ "$FF",			CSV_TYPE_DWORD,	0,	0xFF		},

	/* --- bare digits are decimal, never hex --- */
	{ "10",				CSV_TYPE_DWORD,	0,	10			},
	{ "255",			CSV_TYPE_DWORD,	0,	255			},
	{ "0",				CSV_TYPE_DWORD,	0,	0			},

	/* --- case, padding, whitespace --- */
	{ "0xdeadbeef",		CSV_TYPE_DWORD,	0,	0xDEADBEEF	},
	{ "0xDEADBEEF",		CSV_TYPE_DWORD,	0,	0xDEADBEEF	},
	{ "16#000000FF",	CSV_TYPE_DWORD,	0,	0xFF		},
	{ "  0x10  ",		CSV_TYPE_DWORD,	0,	0x10		},
	{ "0x00000000",		CSV_TYPE_DWORD,	0,	0			},
	{ "0000000000255",	CSV_TYPE_DWORD,	0,	255			},

	/* --- full 32 bit range, both notations --- */
	{ "0xFFFFFFFF",		CSV_TYPE_DWORD,	0,	0xFFFFFFFF	},
	{ "4294967295",		CSV_TYPE_DWORD,	0,	0xFFFFFFFF	},
	{ "0x80000000",		CSV_TYPE_DWORD,	0,	0x80000000	},

	/* --- width limits per type --- */
	{ "0xFF",			CSV_TYPE_BYTE,	0,	0xFF		},
	{ "255",			CSV_TYPE_BYTE,	0,	255			},
	{ "0xFFFF",			CSV_TYPE_WORD,	0,	0xFFFF		},
	{ "65535",			CSV_TYPE_WORD,	0,	65535		},

	/* --- out of range for the target width --- */
	{ "0x100",			CSV_TYPE_BYTE,	CSV_ERR_INVALIDVALUE,	0 },
	{ "256",			CSV_TYPE_BYTE,	CSV_ERR_INVALIDVALUE,	0 },
	{ "0x10000",		CSV_TYPE_WORD,	CSV_ERR_INVALIDVALUE,	0 },
	{ "65536",			CSV_TYPE_WORD,	CSV_ERR_INVALIDVALUE,	0 },

	/* --- out of range for a UDINT --- */
	{ "0x100000000",	CSV_TYPE_DWORD,	CSV_ERR_INVALIDVALUE,	0 },
	{ "4294967296",		CSV_TYPE_DWORD,	CSV_ERR_INVALIDVALUE,	0 },
	{ "99999999999",	CSV_TYPE_DWORD,	CSV_ERR_INVALIDVALUE,	0 },

	/* --- malformed --- */
	{ "",				CSV_TYPE_DWORD,	CSV_ERR_INVALIDVALUE,	0 },
	{ "   ",			CSV_TYPE_DWORD,	CSV_ERR_INVALIDVALUE,	0 },
	{ "0x",				CSV_TYPE_DWORD,	CSV_ERR_INVALIDVALUE,	0 },
	{ "16#",			CSV_TYPE_DWORD,	CSV_ERR_INVALIDVALUE,	0 },
	{ "$",				CSV_TYPE_DWORD,	CSV_ERR_INVALIDVALUE,	0 },
	{ "0xGG",			CSV_TYPE_DWORD,	CSV_ERR_INVALIDVALUE,	0 },
	{ "FF",				CSV_TYPE_DWORD,	CSV_ERR_INVALIDVALUE,	0 },
	{ "0xFF junk",		CSV_TYPE_DWORD,	CSV_ERR_INVALIDVALUE,	0 },
	{ "12abc",			CSV_TYPE_DWORD,	CSV_ERR_INVALIDVALUE,	0 },
	{ "-0x10",			CSV_TYPE_DWORD,	CSV_ERR_INVALIDVALUE,	0 },
	{ "-1",				CSV_TYPE_DWORD,	CSV_ERR_INVALIDVALUE,	0 },
	{ "+0x10",			CSV_TYPE_DWORD,	CSV_ERR_INVALIDVALUE,	0 },
	{ "0x0xFF",			CSV_TYPE_DWORD,	CSV_ERR_INVALIDVALUE,	0 },
	{ "1_000",			CSV_TYPE_DWORD,	CSV_ERR_INVALIDVALUE,	0 },

};


typedef struct writeCase_typ {
	unsigned long	Value;
	unsigned long	DataType;
	unsigned char	Format;			/* CSV_BITFORMAT_ */
	char*			Expect;
} writeCase_typ;


static const writeCase_typ writeCases[] = {

	/* --- padded to the width of the source type --- */
	{ 0xFF,			CSV_TYPE_BYTE,	CSV_BITFORMAT_HEX_0X,	"0xFF"			},
	{ 0x05,			CSV_TYPE_BYTE,	CSV_BITFORMAT_HEX_0X,	"0x05"			},
	{ 0xFF,			CSV_TYPE_WORD,	CSV_BITFORMAT_HEX_0X,	"0x00FF"		},
	{ 0xFF,			CSV_TYPE_DWORD,	CSV_BITFORMAT_HEX_0X,	"0x000000FF"	},

	/* --- IEC notation --- */
	{ 0xFF,			CSV_TYPE_BYTE,	CSV_BITFORMAT_HEX_IEC,	"16#FF"			},
	{ 0xFF,			CSV_TYPE_DWORD,	CSV_BITFORMAT_HEX_IEC,	"16#000000FF"	},

	/* --- decimal, deliberately not padded --- */
	{ 0xFF,			CSV_TYPE_BYTE,	CSV_BITFORMAT_DECIMAL,	"255"			},
	{ 0x05,			CSV_TYPE_BYTE,	CSV_BITFORMAT_DECIMAL,	"5"				},
	{ 0xFFFFFFFF,	CSV_TYPE_DWORD,	CSV_BITFORMAT_DECIMAL,	"4294967295"	},

	/* --- extremes --- */
	{ 0,			CSV_TYPE_DWORD,	CSV_BITFORMAT_HEX_0X,	"0x00000000"	},
	{ 0xFFFFFFFF,	CSV_TYPE_DWORD,	CSV_BITFORMAT_HEX_0X,	"0xFFFFFFFF"	},

	/* --- an out of range setting must not silently corrupt output --- */
	{ 0xFF,			CSV_TYPE_BYTE,	99,						"0xFF"			},

};


static void recordFail(char* what)
{
	testFail++;

	if(strlen((char*)firstFail) == 0){

		/* An empty input is itself a case, so it needs a label of its own
			rather than leaving firstFail looking unset */

		if(strlen(what) == 0)	strcpy((char*)firstFail, "(empty value)");
		else					strncpy((char*)firstFail, what, sizeof(firstFail)-1);

	}
}


/* Confirm PV_ninfo really reports 17/18/19 for BYTE/WORD/DWORD on this
   runtime. If it does not, the whole feature is unreachable. */

static void checkPvTypes(void)
{
	UDINT	Len;
	UINT	Dim;

	pvTypeByte=		0xFFFFFFFF;
	pvTypeWord=		0xFFFFFFFF;
	pvTypeDword=	0xFFFFFFFF;

	PV_ninfo( "Default:tByte",  &pvTypeByte,  &Len, &Dim );
	PV_ninfo( "Default:tWord",  &pvTypeWord,  &Len, &Dim );
	PV_ninfo( "Default:tDword", &pvTypeDword, &Len, &Dim );

	pvTypeOk=	(pvTypeByte  == CSV_TYPE_BYTE)
			&&	(pvTypeWord  == CSV_TYPE_WORD)
			&&	(pvTypeDword == CSV_TYPE_DWORD);

	if(pvTypeOk) testPass++; else recordFail("PV_ninfo type codes");
}


static void runReadCases(void)
{
	UDINT				i;
	CSVFileVariable_typ	Var;
	UDINT				Storage;
	UDINT				Got;
	UINT				Status;

	for(i=0; i<(sizeof(readCases)/sizeof(readCases[0])); i++){

		Storage=	0xA5A5A5A5;

		memset(&Var, 0, sizeof(Var));
		strcpy(Var.Name, "testVar");
		strcpy(Var.Value, readCases[i].Value);
		Var.DataType=	readCases[i].DataType;
		Var.Address=	(UDINT)&Storage;
		Var.Length=		4;

		Status=	csvOpenVar( 1, &Var, &CsvMgr );

		if(Status != readCases[i].ExpectStatus){
			recordFail(readCases[i].Value);
			continue;
		}

		if(Status == 0){

			/* Compare only the bytes the type actually writes. This reads the
				low bytes, which assumes a little endian target - true for
				ARsim/x86 and for the X20 ARM target this project builds. */

			Got=	Storage;
			if(readCases[i].DataType == CSV_TYPE_BYTE)		Got=	Storage & 0xFF;
			else if(readCases[i].DataType == CSV_TYPE_WORD)	Got=	Storage & 0xFFFF;

			if(Got != readCases[i].ExpectStored){
				recordFail(readCases[i].Value);
				continue;
			}

		}
		else{

			/* A rejected value must leave the target untouched */
			if(Storage != 0xA5A5A5A5){
				recordFail(readCases[i].Value);
				continue;
			}

		}

		testPass++;

	}
}


static void runWriteCases(void)
{
	UDINT				i;
	CSVFileVariable_typ	Var;
	UDINT				Storage;
	UINT				Status;

	for(i=0; i<(sizeof(writeCases)/sizeof(writeCases[0])); i++){

		Storage=	writeCases[i].Value;

		memset(&Var, 0, sizeof(Var));
		strcpy(Var.Name, "testVar");
		Var.DataType=	writeCases[i].DataType;
		Var.Address=	(UDINT)&Storage;
		Var.Length=		4;

		CsvMgr.IN.CFG.BitStringFormat=	writeCases[i].Format;

		Status=	csvSaveVar( 1, &Var, &CsvMgr );

		if( (Status != 0) || (strcmp(Var.Value, writeCases[i].Expect) != 0) ){
			recordFail(writeCases[i].Expect);
		}
		else{
			testPass++;
		}

	}

	CsvMgr.IN.CFG.BitStringFormat=	CSV_BITFORMAT_HEX_0X;
}


/* Every value the formatter can produce must read back as itself, for
   each of the three formats and each of the three widths. */

static void runRoundTrip(void)
{
	UDINT				iFmt, iType, iVal;
	CSVFileVariable_typ	Var;
	UDINT				Storage, Original;
	STRING				Written[CSV_STRLEN_VARVALUE+1];

	UDINT	types[3];
	UDINT	values[4];

	types[0]=	CSV_TYPE_BYTE;
	types[1]=	CSV_TYPE_WORD;
	types[2]=	CSV_TYPE_DWORD;

	for(iFmt=0; iFmt<3; iFmt++){
		for(iType=0; iType<3; iType++){

			values[0]=	0;
			values[1]=	1;

			if(types[iType] == CSV_TYPE_BYTE){
				values[2]=	0x7F;
				values[3]=	0xFF;
			}
			else if(types[iType] == CSV_TYPE_WORD){
				values[2]=	0x7FFF;
				values[3]=	0xFFFF;
			}
			else{
				values[2]=	0x7FFFFFFF;
				values[3]=	0xFFFFFFFF;
			}

			for(iVal=0; iVal<4; iVal++){

				Original=	values[iVal];
				Storage=	Original;

				memset(&Var, 0, sizeof(Var));
				strcpy(Var.Name, "testVar");
				Var.DataType=	types[iType];
				Var.Address=	(UDINT)&Storage;
				Var.Length=		4;

				CsvMgr.IN.CFG.BitStringFormat=	(USINT)iFmt;

				if( csvSaveVar( 1, &Var, &CsvMgr ) != 0 ){
					recordFail("round trip save");
					continue;
				}

				strcpy(Written, Var.Value);

				Storage=	0;
				strcpy(Var.Value, Written);

				if( csvOpenVar( 1, &Var, &CsvMgr ) != 0 ){
					recordFail(Written);
					continue;
				}

				if(Storage != Original){
					recordFail(Written);
					continue;
				}

				testPass++;

			}
		}
	}

	CsvMgr.IN.CFG.BitStringFormat=	CSV_BITFORMAT_HEX_0X;
}


void _INIT ProgramInit(void)
{
	memset(&CsvMgr, 0, sizeof(CsvMgr));
	CsvMgr.IN.CFG.DisableLogging=	1;
	strcpy(CsvMgr.IN.PAR.FileDevice, "USER");
	CSVFn_Init(&CsvMgr);

	testState=	0;
}


void _CYCLIC ProgramCyclic(void)
{
	CSVFn_Cyclic(&CsvMgr);

	/* Run once automatically a few scans after boot, so no external
	   trigger is needed. runTest re-runs it on demand. */

	if(bootDelay < 50){
		bootDelay++;
		if(bootDelay == 50) runTest= 1;
	}

	switch(testState){

		case 0:

			if(runTest){

				runTest=		0;
				testPass=		0;
				testFail=		0;
				testDone=		0;
				roundTripOk=	0;

				/* Cleared here, not recomputed on the way out: the error
					paths in states 1 and 2 jump straight to state 3 and
					never reach the state 9 that sets it, so a previous
					passing run would otherwise leave it TRUE */

				suiteOk=		0;
				strcpy((char*)firstFail, "");

				checkPvTypes();
				runReadCases();
				runWriteCases();
				runRoundTrip();

				/* Now the end to end path: write real PVs to a file,
				   clear them, and read the file back. This is the only
				   part that exercises csvGetVarInfo/PV_ninfo for real. */

				tByte=	0xAB;
				tWord=	0xBEEF;
				tDword=	0xDEADBEEF;
				tUsint=	42;

				/* Cleared in full: CSVFn_Cyclic walks every entry regardless of
					the empty terminator, so a shorter list would leave rows
					behind from the previous run */

				memset( CsvMgr.IN.PAR.VariableList, 0, sizeof(CsvMgr.IN.PAR.VariableList) );

				strcpy(CsvMgr.IN.PAR.FileName, "bitstring.csv");
				strcpy(CsvMgr.IN.PAR.VariableList[0], "Default:tByte");
				strcpy(CsvMgr.IN.PAR.VariableList[1], "Default:tWord");
				strcpy(CsvMgr.IN.PAR.VariableList[2], "Default:tDword");
				strcpy(CsvMgr.IN.PAR.VariableList[3], "Default:tUsint");
				strcpy(CsvMgr.IN.PAR.VariableList[4], "");

				CsvMgr.IN.CMD.SaveVariableListToFile=	1;

				testState=	1;

			}

			break;


		case 1:

			if(CsvMgr.OUT.STAT.Done){

				CsvMgr.IN.CMD.SaveVariableListToFile=	0;

				/* Wipe the PVs so a successful read is unambiguous */
				tByte=	0;
				tWord=	0;
				tDword=	0;
				tUsint=	0;

				CsvMgr.IN.CMD.OpenFile=	1;
				testState=	2;

			}
			else if(CsvMgr.OUT.STAT.Error){
				CsvMgr.IN.CMD.SaveVariableListToFile=	0;
				recordFail("save to file");
				CsvMgr.IN.CMD.AcknowledgeError=	1;
				testState=	3;
			}

			break;


		case 2:

			if(CsvMgr.OUT.STAT.Done){

				CsvMgr.IN.CMD.OpenFile=	0;

				roundTripOk=	(tByte  == 0xAB)
							&&	(tWord  == 0xBEEF)
							&&	(tDword == 0xDEADBEEF)
							&&	(tUsint == 42);

				if(roundTripOk) testPass++; else recordFail("file round trip");

				testState=	5;

			}
			else if(CsvMgr.OUT.STAT.Error){
				CsvMgr.IN.CMD.OpenFile=	0;
				recordFail("open file");
				CsvMgr.IN.CMD.AcknowledgeError=	1;
				testState=	3;
			}

			break;


		case 3:

			CsvMgr.IN.CMD.AcknowledgeError=	0;

			testDone=	1;
			testState=	0;
			break;


		/* Read a hand written file, exercising csvParseLine and
		   csvGetVarInfo rather than calling csvOpenVar directly */

		case 5:

			tByte= 0; tWord= 0; tDword= 0; tUsint= 0;

			strcpy(CsvMgr.IN.PAR.FileName, "crafted.csv");
			CsvMgr.IN.CMD.OpenFile=	1;
			testState=	6;
			break;


		case 6:

			if(CsvMgr.OUT.STAT.Done || CsvMgr.OUT.STAT.Error){

				CsvMgr.IN.CMD.OpenFile=	0;

				c1Byte=		tByte;
				c1Word=		tWord;
				c1Dword=	tDword;
				c1Usint=	tUsint;
				c1Status=	CsvMgr.OUT.STAT.ErrorID;

				if( (c1Status == 0) && (c1Byte == 0x7F) && (c1Word == 0x00FF)
					&& (c1Dword == 0xFFFFFFFF) && (c1Usint == 99) ){
					testPass++;
				}
				else{
					recordFail("crafted.csv values");
				}

				CsvMgr.IN.CMD.AcknowledgeError=	CsvMgr.OUT.STAT.Error;
				testState=	7;

			}

			break;


		/* A file with one out of range line: the bad line must fail and
		   the good lines must still load */

		case 7:

			CsvMgr.IN.CMD.AcknowledgeError=	0;

			tByte= 0x11; tWord= 0; tDword= 0;

			strcpy(CsvMgr.IN.PAR.FileName, "badline.csv");
			CsvMgr.IN.CMD.OpenFile=	1;
			testState=	8;
			break;


		case 8:

			if(CsvMgr.OUT.STAT.Done || CsvMgr.OUT.STAT.Error){

				CsvMgr.IN.CMD.OpenFile=	0;

				c2Byte=		tByte;
				c2Word=		tWord;
				c2Dword=	tDword;
				c2ErrorID=	CsvMgr.OUT.STAT.ErrorID;

				/* 0x1FF does not fit a BYTE, so tByte keeps 0x11, the
				   other two load, and the operation reports a line failure */

				if( (c2Byte == 0x11) && (c2Word == 0x1234)
					&& (c2Dword == 0xABCDEF) && (c2ErrorID == CSV_ERR_LINEFAILURE) ){
					testPass++;
				}
				else{
					recordFail("badline.csv handling");
				}

				CsvMgr.IN.CMD.AcknowledgeError=	CsvMgr.OUT.STAT.Error;
				testState=	9;

			}

			break;


		case 9:

			CsvMgr.IN.CMD.AcknowledgeError=	0;

			/* Computed before the results are written, so the file carries
				this run's verdict. The count is checked as well as the
				failure tally, so a phase that never ran cannot pass by
				silence. */

			suiteOk=	(testFail == 0)
					&&	(testPass == CSVTEST_EXPECTED_ASSERTIONS)
					&&	(pvTypeOk != 0)
					&&	(roundTripOk != 0);

			memset( CsvMgr.IN.PAR.VariableList, 0, sizeof(CsvMgr.IN.PAR.VariableList) );

			strcpy(CsvMgr.IN.PAR.FileName, "result.csv");
			strcpy(CsvMgr.IN.PAR.VariableList[0], "Default:suiteOk");
			strcpy(CsvMgr.IN.PAR.VariableList[1], "Default:testPass");
			strcpy(CsvMgr.IN.PAR.VariableList[2], "Default:testFail");
			strcpy(CsvMgr.IN.PAR.VariableList[3], "Default:firstFail");
			strcpy(CsvMgr.IN.PAR.VariableList[4], "Default:pvTypeByte");
			strcpy(CsvMgr.IN.PAR.VariableList[5], "Default:pvTypeWord");
			strcpy(CsvMgr.IN.PAR.VariableList[6], "Default:pvTypeDword");
			strcpy(CsvMgr.IN.PAR.VariableList[7], "Default:pvTypeOk");
			strcpy(CsvMgr.IN.PAR.VariableList[8], "Default:roundTripOk");
			strcpy(CsvMgr.IN.PAR.VariableList[9], "Default:c1Byte");
			strcpy(CsvMgr.IN.PAR.VariableList[10], "Default:c1Word");
			strcpy(CsvMgr.IN.PAR.VariableList[11], "Default:c1Dword");
			strcpy(CsvMgr.IN.PAR.VariableList[12], "Default:c1Usint");
			strcpy(CsvMgr.IN.PAR.VariableList[13], "Default:c2Byte");
			strcpy(CsvMgr.IN.PAR.VariableList[14], "Default:c2Word");
			strcpy(CsvMgr.IN.PAR.VariableList[15], "Default:c2Dword");
			strcpy(CsvMgr.IN.PAR.VariableList[16], "Default:c2ErrorID");
			strcpy(CsvMgr.IN.PAR.VariableList[17], "");

			CsvMgr.IN.CMD.SaveVariableListToFile=	1;
			testState=	10;
			break;


		case 10:

			if(CsvMgr.OUT.STAT.Done || CsvMgr.OUT.STAT.Error){
				CsvMgr.IN.CMD.SaveVariableListToFile=	0;
				CsvMgr.IN.CMD.AcknowledgeError=			CsvMgr.OUT.STAT.Error;
				testDone=	1;
				testState=	3;
			}

			break;

	}
}


void _EXIT ProgramExit(void)
{
}
