/***************************************************************************
 * Exception Filter Class				                                   *
 * Benoit Martin                                                           *
 * Copyright (C) 1997, Vircom Inc.										   *
 ***************************************************************************/

// THIS CLASS MUST BE THREAD SAFE!!!
// DO NOT FORGET TO INCLUDE "imagehlp.lib" in your LINK libraries..
// StackWalk will no longer be called, if IMAGEHLP.DLL not present..

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <imagehlp.h>
#include "ExpFltr.h"
#include "MemoryManager.h"

// static globals
CRITICAL_SECTION CExpFltr::m_crit;
TCHAR CExpFltr::m_report_file[_MAX_PATH]={0};
TCHAR CExpFltr::m_app_title[MAX_SZ_APP_TITLE];
WORD CExpFltr::m_num_modules=0;
DWORD CExpFltr::m_ptr_module_addr[MAX_MODULES];
BYTE CExpFltr::m_GP_count=0; 
BYTE CExpFltr::m_max_GP=10; // default value
VoidFunction CExpFltr::m_stop=NULL;
BOOL CExpFltr::m_bUseStackWalk=FALSE;
TCHAR CExpFltr::m_szRestartCommand[_MAX_PATH]={0};
TCHAR CExpFltr::m_szRestartArguments[_MAX_PATH]={0};

extern bool noException;
extern bool g_boQuitApp;


// so that, we can only have one instance at a time! <singleton>
CExpFltr CExpFltr::m_Instance;
static int exceptionCode=0; // for stack overflow..

// debugging purposes (exported symbols)
EXPORT void ExpFltr_start(void) {}

// constructor
CExpFltr::CExpFltr(void)
{
	InitializeCriticalSection(&CExpFltr::m_crit);
	// load library, if not found, return from function here.. else we continue..
	m_dll = ::LoadLibrary("ImageHlp.dll");
	if (m_dll) 
	{
		m_bUseStackWalk = TRUE;
		FreeLibrary(m_dll);
	}
}

// Destructor
CExpFltr::~CExpFltr(void)
{
	DeleteCriticalSection(&CExpFltr::m_crit);
}

CExpFltr &CExpFltr::GetInstance(void)
{
	return m_Instance;
}


void CExpFltr::
InitFilter(void *stop, BYTE max_GP)
// Just set the global unhandled exception filter .. and the callBack Stop function
// if we reach a certain GP threshold.. (m_GP_count)
{
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CExpFltr::Filter);
	m_stop = (VoidFunction)stop;
	m_max_GP = max_GP;
	return;
}

LONG CExpFltr::
Filter(LPEXCEPTION_POINTERS lp)
// Function called when an Exception is raised..
{
	TCHAR errbuf[256];

	LPEXCEPTION_RECORD lpER = lp->ExceptionRecord;
	LONG retval = EXCEPTION_EXECUTE_HANDLER;

	if (m_GP_count >= m_max_GP) 
	{
		killIT();
		return retval; // we stop there...
	}

	if (m_GP_count < 255) m_GP_count++;

	exceptionCode = lpER->ExceptionCode; // we save the exception code for later use..

	switch(lpER->ExceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		_stprintf(errbuf, _T("Access Violation: Attempt to %s data at %x"), 
			lpER->ExceptionInformation[0]? _T("write") : _T("read"), 
			lpER->ExceptionInformation[1]);
		break;
	
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		_stprintf(errbuf,_T("Array bounds exceeded"));
		break;

	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		_stprintf(errbuf, _T("FLT Division by zero"));
		break;

	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		_stprintf(errbuf, _T("INT Division by zero"));
		break;

	case EXCEPTION_STACK_OVERFLOW:
		_stprintf(errbuf,_T("Stack overflow"));
		break;

	case EXCEPTION_ILLEGAL_INSTRUCTION:
		_stprintf(errbuf,_T("Illegal instruction"));
		break;

	case EXCEPTION_DATATYPE_MISALIGNMENT:
		_stprintf(errbuf,_T("Data type misalignment"));
		break;

	case EXCEPTION_IN_PAGE_ERROR:
		_stprintf(errbuf, _T("Page fault: read error"));
		break;

	case EXCEPTION_INT_OVERFLOW:
		_stprintf(errbuf, _T("Integer overflow"));
		break;

	case EXCEPTION_FLT_OVERFLOW:
		_stprintf(errbuf, _T("Float overflow"));
		break;

	default:
		_stprintf(errbuf, _T("Exception error %lu"), lpER->ExceptionCode);
		break;
	}

	LogException(lp->ContextRecord, errbuf, lpER->ExceptionAddress);

	// Verify if process MUST DIE.. anything else might cause an exception cascade!
	if (1)
	{
		killIT();
		ExitProcess(1);
	}

	return retval;
}


void CExpFltr::
SetAutoRestart(char *szCommand, char *szArguments)
{
	if (!szCommand || !szCommand[0]) return;

	lstrcpyn(m_szRestartCommand, szCommand, sizeof(m_szRestartCommand));
	m_szRestartCommand[sizeof(m_szRestartCommand)-1]=0;

	if (szArguments && szArguments[0])
	{
		lstrcpyn(m_szRestartArguments, szArguments, sizeof(m_szRestartArguments));
		m_szRestartArguments[sizeof(m_szRestartArguments)-1]=0;
	}
	return;
}


void CExpFltr::
DisableAutoRestart()
{
	m_szRestartCommand[0]=0;
	m_szRestartArguments[0]=0;
	return;
}


void CExpFltr::
LogException(CONTEXT *ctxtRec, LPTSTR errorMsg, void *excpAddr)
// Will create an exception report and log it to file.. 
// Parameters: CONTEXT pointer that contain specific information (platform dependant)
//             regarding register values
//             ErrorMsg to generate and the address of the exception
//  
{
   if (/*!noException*/ !g_boQuitApp) {
   
   HANDLE hFile=INVALID_HANDLE_VALUE;
	__try {
		// file handling, so need a crit section
		EnterCriticalSection(&m_crit);

		hFile=CreateFile(m_report_file, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile != INVALID_HANDLE_VALUE)
		{
			TCHAR buffer[256];
			DWORD nBytes=0;

			SetFilePointer(hFile,0,NULL,FILE_END);
			// print header
			SYSTEMTIME sysTime; 
			GetLocalTime(&sysTime);

			// Output: Date Time APP_TITLE EXCEPTION
			_stprintf(buffer, _T("%d/%d/%d %d:%02d:%02d  %s  EXCEPTION\r\n"), 
				sysTime.wMonth,sysTime.wDay,sysTime.wYear, sysTime.wHour,
				sysTime.wMinute,sysTime.wSecond, m_app_title);
			WriteFile(hFile, buffer , lstrlen(buffer), &nBytes, NULL);
	
			// Output: Address: <addr>  Error: Error message generated
			_stprintf(buffer, _T("Address:%#lx  Error: %s\r\n"), excpAddr, errorMsg);
			WriteFile(hFile, buffer, lstrlen(buffer), &nBytes, NULL);

			// Output: Register variables listing
			_stprintf(buffer,_T("IntReg:\tDI=%08X SI=%08X AX=%04X BX=%04X CX=%04X DX=%04X\r\n"),
				ctxtRec->Edi,ctxtRec->Esi,ctxtRec->Eax,ctxtRec->Ebx,ctxtRec->Ecx,
				ctxtRec->Edx);
			WriteFile(hFile, buffer, lstrlen(buffer), &nBytes, NULL);

			_stprintf(buffer,
				_T("CtlReg:\tBP=%08X IP=%08X SP=%08X Flags=%08X\r\n\tSegCS=%08X SegSS=%08X\r\n"),
				ctxtRec->Ebp,ctxtRec->Eip,ctxtRec->Esp,ctxtRec->EFlags,
				ctxtRec->SegCs,ctxtRec->SegSs);
			WriteFile(hFile, buffer, lstrlen(buffer), &nBytes, NULL);	

			_stprintf(buffer,_T("Code at current IP: "));
			short i, pos;
			LPBYTE p = (LPBYTE)excpAddr;
			for (i=0; i < 16 ; i++,p++) {
				if (IsBadCodePtr((FARPROC)p)) {
					break;
				}
				pos = lstrlen(buffer);
				_stprintf(&buffer[pos],_T("%02X "),*p);
			}
			lstrcat(buffer, _T("\r\n"));
			WriteFile(hFile, buffer, lstrlen(buffer), &nBytes, NULL);

			// Print some memory statistics informations
			MEMORYSTATUS ms;
			ms.dwLength = sizeof(MEMORYSTATUS);
			GlobalMemoryStatus(&ms);
			_stprintf(buffer, _T("\r\nMemory Statistics:\tLoad=%ld  TotPhys=%ld  AvailPhys=%ld\r\n"), 
				ms.dwMemoryLoad, ms.dwTotalPhys, ms.dwAvailPhys);
			WriteFile(hFile, buffer, lstrlen(buffer), &nBytes, NULL);

			_stprintf(buffer, 
				_T("\t\t\tTotPage=%ld  AvailPage=%ld\r\n\t\t\tTotVirtual=%ld  AvailVirtual=%ld\r\n"),
				ms.dwTotalPageFile, ms.dwAvailPageFile, ms.dwTotalVirtual, ms.dwAvailVirtual);
			WriteFile(hFile, buffer, lstrlen(buffer), &nBytes, NULL);

			// Print the memory map
			DWORD *stackStart[MAX_STACK], *stackEnd[MAX_STACK];
			DWORD nStacks= MAX_STACK;
			log_mem_map(hFile, stackStart, stackEnd, &nStacks);

			// Print the stack and the calling stack functions
			log_stack(hFile, (DWORD *)ctxtRec->Esp, (DWORD *)ctxtRec->Ebp, stackStart, 
				stackEnd, nStacks, excpAddr, ctxtRec);

			// add two lines...
			_stprintf(buffer, _T("\r\n\r\n"));
			WriteFile(hFile, buffer, lstrlen(buffer), &nBytes, NULL);
		}
	}
	__finally 
	{
		LeaveCriticalSection(&m_crit);
		if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	}
	return;
   }
}


void CExpFltr::
log_mem_map(HANDLE hFile, DWORD **stackStart, DWORD **stackEnd, DWORD *nStacks)
// Log to the report file, the memory map!
// Parameters: hFile handle to report file
//             stackStart/End : start/end of stack region (taken from log_exception)
//             (note that they are not initialized...but it's not needed)
//             nStacks: number of them..
{
	TCHAR buffer[256];
	DWORD nBytes;
	_stprintf(buffer,_T("\r\nMemory Map (does not include free regions):"));
	WriteFile(hFile, buffer, lstrlen(buffer), &nBytes, NULL);

	void *memStart=0x0;
	RGNINFO rgnInfo;
	DWORD stack_num=0;
	
	// will do a loop of VirtualQueries...and output each result as a single line
	// Will also store start/end of stack regions
	while (get_region_info(memStart,&rgnInfo)) {

/*		if (stackStart[stack_num] != NULL && stackEnd[stack_num] == NULL) 
		{
			// add end of stack region..
			stackEnd[stack_num++]= (DWORD *)rgnInfo.rgnAddress;
		}
*/
		if (stack_num < MAX_STACK && rgnInfo.rgnIsStack) 
		{
			// add start of end region..
			stackStart[stack_num]=(DWORD *)rgnInfo.rgnAddress;
			stackEnd[stack_num++]=(DWORD *)((DWORD)rgnInfo.rgnAddress+(DWORD)rgnInfo.rgnSize);
		}

		buffer[0]='\r';
		buffer[1]='\n';

		// create memory line info out of virtual query results

		// map line only if it's not free memory
		if (rgnInfo.rgnType != MEM_FREE) 
		{
			region_map_line(&rgnInfo, buffer+2,
				(sizeof(buffer)/sizeof(TCHAR)) - (2*sizeof(TCHAR)));
			WriteFile(hFile, buffer, lstrlen(buffer), &nBytes, NULL);
		}

		// go to next memory address space
		memStart=(char *)rgnInfo.rgnAddress+rgnInfo.rgnSize;
     }
     *nStacks=stack_num; // total number of stacks found
}


BOOL CExpFltr::
get_region_info (void *memStart, RGNINFO *rgnInfo)
// Find information on next memory region
// Parameters: memStart points to the beginning of the memory region we want info on
//             rgnInfo is the buffer where we put info on region
{
	MEMORY_BASIC_INFORMATION memInfo;
	memset(rgnInfo,0,sizeof(*rgnInfo));

	if (fixVirtualQuery(memStart,&memInfo,sizeof(memInfo)) 
		!= sizeof(memInfo)) {
		return FALSE;
	}

    if (memInfo.State == MEM_FREE) 
	{
		// we have a block of free address space that has not been reserved
		rgnInfo->rgnAddress=memInfo.BaseAddress;
		rgnInfo->rgnProtection=memInfo.AllocationProtect;
		rgnInfo->rgnSize=memInfo.RegionSize;
		rgnInfo->rgnType=MEM_FREE;
		rgnInfo->rgnBlocks=0;
		rgnInfo->rgnGuardBlks=0;
		rgnInfo->rgnIsStack=FALSE;
		return TRUE;
	}

	// either a reserve or commit blok
	rgnInfo->rgnAddress=memInfo.AllocationBase;
	rgnInfo->rgnProtection=memInfo.AllocationProtect;

	// to get complete information, we need to iterate through all the region's block
	walkMemRegion(rgnInfo);
	return(TRUE);
}


DWORD CExpFltr::
fixVirtualQuery (LPVOID lpvAddr, PMEMORY_BASIC_INFORMATION pmem_bi, DWORD cbLen)
// Seems there is a bug in VirtualQuery, so need to fix some values reported..
{
	DWORD dwRetVal = VirtualQuery(lpvAddr,pmem_bi, cbLen);
	if (dwRetVal == cbLen) 
	{
		if (((DWORD) pmem_bi->AllocationBase % 0x1000) == 0xFFF) 
		{
			// If the AllocationBase ends with 0xFFF, it is 1 byte off.
			pmem_bi->AllocationBase= (PVOID)((PBYTE) pmem_bi->AllocationBase + 1);
		}
        
		if ((pmem_bi->RegionSize % 0x1000) == 0xFFF) 
		{
			// If the RegionSize ends with 0xFFF, the size is 1 byte off.
			pmem_bi->RegionSize++;
		}
		if (pmem_bi->State != MEM_FREE && pmem_bi->AllocationProtect == 0) 
		{
			// If the the region is not free and the AllocationProtect is 0,
			// AllocationProtect should be PAGE_READONLY.
			pmem_bi->AllocationProtect = PAGE_READONLY;
		}
	}
	return(dwRetVal);
}

void CExpFltr::
walkMemRegion(RGNINFO *rgnInfo)
// Walk through the memory region and retrieve information specific to each block in 
// that region
// Parameters: rgninfo structure to fill with the retrieved info
{
	MEMORY_BASIC_INFORMATION MBI;
	void *memStart;
	DWORD dwProtectBlock[4]={ 0 };// 0 = reserved, PAGE_NOACCESS, PAGE_READWRITE
//	short idx=0;

	memStart=rgnInfo->rgnAddress;
	rgnInfo->rgnType=MEM_PRIVATE; // default

	while (fixVirtualQuery(memStart,&MBI,sizeof(MBI)) == sizeof(MBI))
	{
		// Check to see whether the block we got info for is contained in the region
		if (MBI.AllocationBase != rgnInfo->rgnAddress) break;
		
		// we found a block contained in region

		// Now let's detect Stacks in Windows95? maybe not essential...but.
		// Win95 stacks are in a region wherein the last 4 blocks have the following 
		// attributes: reserved, PAGE_NOACCESS, PAGE_READWRITE and another reserved..

		// if 0 through 3rd block, make a note of the block's protection
//		idx = (short) (rgnInfo->rgnBlocks % 4);

		if (rgnInfo->rgnBlocks < 4) 
			dwProtectBlock[rgnInfo->rgnBlocks]= (MBI.State == MEM_RESERVE) ? 0 : MBI.Protect;
		else
		{
			MoveMemory(&dwProtectBlock[0], &dwProtectBlock[1], 
				sizeof(dwProtectBlock) - sizeof(DWORD));
			dwProtectBlock[3] = (MBI.State == MEM_RESERVE) ? 0 : MBI.Protect;
		}

		// add 1 to the number of blocks in the region
		rgnInfo->rgnBlocks++;

		// add block size to the reserved region size
		rgnInfo->rgnSize+=MBI.RegionSize;

		// if PAGE_GUARD protection attribute flag update guard block number
		if (MBI.Protect&PAGE_GUARD) rgnInfo->rgnGuardBlks++;

		// take a guess for physical storage.. guess because some blocks can
		// convert from MEM_IMAGE to PRIVATE or from MAPPED to PRIVATE
		// PRIVATE can always be overridden by IMAGE or MAPPED...
		if (rgnInfo->rgnType == MEM_PRIVATE) rgnInfo->rgnType=MBI.Type;

		// get address of next block
		memStart=(char *)memStart+MBI.RegionSize;
	}

	// After examining region, check to see if it's a thread stack
	// Windows NT: Assume thread stack if PAGE_GUARD flag
	// Windows 95: Assume thread stack if last 4 blocks have attributes mentionned above
	rgnInfo->rgnIsStack = 
		(rgnInfo->rgnGuardBlks > 0) ||
		((rgnInfo->rgnBlocks >= 4) && 
		(dwProtectBlock[0] == 0) && (dwProtectBlock[1] == PAGE_NOACCESS) &&
		(dwProtectBlock[2] == PAGE_READWRITE) && (dwProtectBlock[3]==0));

	return;
}


void CExpFltr::
region_map_line(RGNINFO *rgnInfo, LPTSTR buf, WORD buf_size)
// create the region information line for the memory mapping output
// Parameters: Region info structure
//             buffer to fill with info
//			   size of this buffer...
// format is:
// MemAddr  Mem Type                    Size  ModuleName   File Attributes
// 00000000 Free/Private/Reserve/Mapped 65536 ModName      ModAttr
{

	_stprintf(buf,_T("%08X %s %10lu  "),rgnInfo->rgnAddress, 
		memType2Text(rgnInfo->rgnType),rgnInfo->rgnSize);

	short len= lstrlen(buf);
	if (rgnInfo->rgnAddress != NULL) 
	{
		TCHAR mod_name[_MAX_PATH];
		if (GetModuleFileName((HINSTANCE)rgnInfo->rgnAddress,mod_name, 
			sizeof(mod_name)) > 0) 
		{
			add_module(&buf[len],buf_size-len,mod_name);
			if (m_num_modules < MAX_MODULES) 
			{
				m_ptr_module_addr[m_num_modules]=(DWORD)rgnInfo->rgnAddress;
				m_num_modules += 1;
			}
		}
	}

	if (rgnInfo->rgnAddress == GetProcessHeap())
	{
		if (lstrlen(buf) < buf_size + 20)
			lstrcat(buf, _T("Default Process Heap"));
	}

	if (rgnInfo->rgnIsStack)
	{
		if (lstrlen(buf) < buf_size + 12)
			lstrcat(buf, _T("Thread Stack"));
	}
	return;
}


void CExpFltr::
add_module(LPTSTR buf, short buf_len, LPTSTR module_name)
// Add a module file name, date, size and time to our global structure..
// Parameters: buf represents our current line of output (at file name position)
//             buf_len what's left of space in that line of output
{
	HANDLE hfile;
    WIN32_FIND_DATA findData;

	if (buf_len < 47) return;

	// try to find file on disk...
	if ((hfile=FindFirstFile(module_name,&findData)) != INVALID_HANDLE_VALUE) 
	{
		// file found
		FindClose(hfile);

		LPTSTR mptr = module_name;
		short mod_len = lstrlen(mptr);
		if (mod_len > 20) 
		{
			// truncate file name to last 20 characters...
			mptr+=mod_len-20;
			*mptr='.'; *(mptr+1)='.'; // add .. before file name truncated
		}
		_stprintf(buf,_T("%-20s"),mptr);

		// get file informations
		FILETIME fileTime;
		SYSTEMTIME sysTime;
		FileTimeToLocalFileTime(&findData.ftLastWriteTime,&fileTime);
		FileTimeToSystemTime(&fileTime,&sysTime);
		short len=lstrlen(buf);
		_stprintf(&buf[len],_T("  %7lu  %2d/%02d/%02d %2d:%02d:%02d"), 
			findData.nFileSizeLow, sysTime.wMonth,sysTime.wDay,
			sysTime.wYear-(sysTime.wYear > 1999 ? 2000 : 1900),
			sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
	}
	else {
		if (lstrlen(module_name)+17 < buf_len) 
		{
			_stprintf(buf,_T("%s:file not found"),module_name);
		}
	}
	return;
}


void CExpFltr::
log_stack(HANDLE hFile, DWORD *esp, DWORD *ebp, DWORD **stackStart, 
		  DWORD **stackEnd, DWORD nStacks, void *excpAddr, LPCONTEXT lpContext)
// Print the stack information
// Parameters: hFile handle to output file
//             esp and ebp are the register pointers
//             stackStart/End delimits address region of stack
//             expAddr : exception address
{
	TCHAR buffer[256];
	DWORD nBytes;

	// verify that we have at least one stack
    _stprintf(buffer,_T("\r\n%lu stacks found."), nStacks);
	WriteFile(hFile, buffer, lstrlen(buffer),&nBytes,NULL);

	// verify that we have at least one stack
	if (nStacks == 0) {
		lstrcpy(buffer,_T("\r\n\r\nNo stack found."));
		WriteFile(hFile, buffer, lstrlen(buffer),&nBytes,NULL);
		return;
	}

	BOOL valid=FALSE;
	// verify that the current ESP register is in stack region
	short stack_num;
	for (stack_num=0; stack_num < (short)nStacks ; stack_num++) 
	{
		_stprintf(buffer,_T("\r\nStack %02lu : %08X - %08X"),stack_num, stackStart[stack_num],stackEnd[stack_num]);
		if ((esp >= stackStart[stack_num]) && (esp <= stackEnd[stack_num]))
		{
            lstrcat(buffer,_T("*"));
			valid=TRUE;
			break;
		}
		else
		{
			// If the exception was a stack overflow, chances are ESP went
			// further then the stack end.
			if( exceptionCode == EXCEPTION_STACK_OVERFLOW )
			{
				// Find the first stack after which ESP lies.
				// If there is no next stack.
				if( (DWORD)stack_num + 1 >= nStacks )
				{
					// Then ESP probably overflowed from the last stack.
					lstrcat( buffer, _T("***"));
					valid=TRUE;
					break;
				}
				else if( esp <= stackStart[ stack_num + 1 ] ) 
				{
					// If the next stack's start is further then ESP, then
					// ESP probably overflowed from this stack.
					lstrcat( buffer, _T("**"));
					valid=TRUE;
					break;
				}            
			}
		}
		WriteFile(hFile, buffer, lstrlen(buffer),&nBytes,NULL);
	}
	
	if (valid) 
	{
		WriteFile(hFile, buffer, lstrlen(buffer),&nBytes,NULL);
	}
	else 
	{
		lstrcpy(buffer,_T("\r\nStack Not Found."));
		WriteFile(hFile, buffer, lstrlen(buffer),&nBytes,NULL);
		return;
    }
	
	lstrcpy(buffer,_T("\r\n\r\nStack Region:"));
	WriteFile(hFile, buffer, lstrlen(buffer),&nBytes,NULL);

	short idxFunc=0, i, j;
	DWORD Func[MAX_FUNC];
	Func[idxFunc++]=(DWORD)excpAddr;// first entry is where it actually happened
	BOOL fGotBpReg=FALSE, fDone=FALSE; 

	// line is constructed like this:
	// 0x 0x 0x 0x 0x stack value
	LPTSTR pbuf, ptmp, pEsp;
	TCHAR tmpbuf[17];
	while (!fDone) {
		// first entry of line
		_stprintf(buffer,_T("\r\n%08X    "),esp);
		
		pbuf=buffer+12;
		ptmp = tmpbuf;
		for (i=0; i<4; i++,esp++) 
		{
			if (esp >= stackEnd[stack_num] || IsBadReadPtr(esp,4)) 
			{
				fDone=TRUE;
			}
            
			pEsp = (LPTSTR)esp;
			for (j=0; j<4; j++,pEsp++,ptmp++) 
			{
				// builds up tmp buffer with char values of each stack address
				if (fDone) *ptmp='-';
				else if (IsCharAlphaNumeric(*pEsp)) *ptmp=*pEsp;
                else *ptmp='.';
			}
			if (fDone) 
			{
				_stprintf(pbuf,_T("--------  "));
				pbuf+=10;
			}
			else {
				if (esp == ebp)	*(pbuf-1)='>';

				// output stack address
				_stprintf(pbuf,_T("%08X"),*esp);
				pbuf+=8;
				if (fGotBpReg) {
					Func[idxFunc++]=*esp;
					fGotBpReg=FALSE;
				}
				if (esp == ebp) {
					*pbuf++='<';
					ebp=(DWORD *)*esp;
					fGotBpReg=TRUE;
				}
				else *pbuf++=' ';
				*pbuf++=' ';
			}
		}
		*ptmp='\0';
		lstrcpy(pbuf,tmpbuf); // copy over the char values of stack
		WriteFile(hFile, buffer,lstrlen(buffer),&nBytes,NULL);
	}

	// CALL STACK FUNCTIONS

	TCHAR funcNames[2][64]; 
	DWORD funcAddr[2];

	lstrcpy(buffer,_T("\r\n\r\nCall Stack functions:"));
	WriteFile(hFile, buffer,lstrlen(buffer),&nBytes,NULL);
	for (i=0 ; i < idxFunc ; i++) {
		// get the function names around the stack address 
		get_func_names(Func[i],funcNames,funcAddr);
		_stprintf(buffer,_T("\r\n%08X %-20.20s  <  %08X  <  %08X %-20.20s"),
			funcAddr[0],funcNames[0],Func[i],funcAddr[1],funcNames[1]);
		WriteFile(hFile, buffer, lstrlen(buffer),&nBytes,NULL);
	}
	lstrcpy(buffer,_T("\r\n"));
	WriteFile(hFile, buffer, lstrlen(buffer),&nBytes,NULL);

	//////////////////////////////////////////////////////////////
	// Added by Francois Leblanc

	if (!m_bUseStackWalk) return; // just leave it alone..
	
	// Add our banner :)
	lstrcpy(buffer, _T("\r\nStackWalk:"));
	WriteFile(hFile, buffer, lstrlen(buffer), &nBytes, NULL);
	// secret code to correct all bugs -- 049983
	BOOL boHaveSymbols = FALSE;
	SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME);
	if(SymInitialize(GetCurrentProcess(), NULL, TRUE)){
		boHaveSymbols = TRUE;
	}


	STACKFRAME s_StackFrame;
	memset(&s_StackFrame, 0, sizeof(STACKFRAME));
	s_StackFrame.AddrPC.Offset       = lpContext->Eip;
    s_StackFrame.AddrPC.Mode         = AddrModeFlat;
    s_StackFrame.AddrStack.Offset    = lpContext->Esp;
    s_StackFrame.AddrStack.Mode      = AddrModeFlat;
    s_StackFrame.AddrFrame.Offset    = lpContext->Ebp;
    s_StackFrame.AddrFrame.Mode      = AddrModeFlat;	

	while(StackWalk(IMAGE_FILE_MACHINE_I386, GetCurrentProcess(), GetCurrentThread(), &s_StackFrame,
		lpContext, NULL, (PFUNCTION_TABLE_ACCESS_ROUTINE)SymFunctionTableAccess, 
		(PGET_MODULE_BASE_ROUTINE)SymGetModuleBase, NULL)){										
	
		// If the stack frame is ok
		if(s_StackFrame.AddrFrame.Offset){
			if(boHaveSymbols){
				BYTE  lpSymbolBuffer[ sizeof( IMAGEHLP_SYMBOL ) +1024 ];
				PIMAGEHLP_SYMBOL pSymbol = (PIMAGEHLP_SYMBOL)lpSymbolBuffer;
				DWORD dwSymDisplacement = 0;
				
				pSymbol->SizeOfStruct = sizeof(lpSymbolBuffer);
				pSymbol->MaxNameLength = 1024;
				
				// Displacement of the input address, relative to the start of thesymbol
				dwSymDisplacement = 0;
				if(SymGetSymFromAddr(GetCurrentProcess(), s_StackFrame.AddrPC.Offset, 
									 &dwSymDisplacement, pSymbol)){
					// print the symbol name
					wsprintf(buffer, _T("\r\n%s"), pSymbol->Name);
					WriteFile(hFile, buffer, lstrlen(buffer),&nBytes,NULL);
				}
							
				// If we don't have access to the debug symbols, then try to locate the functions
				// using the export table
				get_func_names(s_StackFrame.AddrPC.Offset,funcNames,funcAddr);
				wsprintf(buffer,_T("\r\n%08X %-20s  <  %08X  <  %08X %-20s"),
					funcAddr[0],funcNames[0], s_StackFrame.AddrPC.Offset, funcAddr[1],funcNames[1]);
				WriteFile(hFile, buffer, lstrlen(buffer),&nBytes,NULL);
			}
		
			wsprintf(buffer, _T("\r\n\tAddrPC %08X  AddrReturn %08X  AddrFrame %08X  AddrStack %08X"),
					s_StackFrame.AddrPC.Offset, s_StackFrame.AddrReturn.Offset,
					s_StackFrame.AddrStack.Offset, s_StackFrame.AddrFrame);
			WriteFile(hFile, buffer, lstrlen(buffer),&nBytes,NULL);
			wsprintf(buffer, _T("\r\n\tParams %08X %08X %08X %08X"),
					s_StackFrame.Params[0], s_StackFrame.Params[1],
					s_StackFrame.Params[2], s_StackFrame.Params[3]);
			WriteFile(hFile, buffer, lstrlen(buffer),&nBytes,NULL);
			
			if(!boHaveSymbols){
				// If we don't have access to the debug symbols, then try to locate the functions
				// using the export table			
				wsprintf(buffer, _T("\r\n\tReturn to: "));
				WriteFile(hFile, buffer, lstrlen(buffer),&nBytes,NULL);
		
				get_func_names(s_StackFrame.AddrReturn.Offset,funcNames,funcAddr);
				wsprintf(buffer, _T("\r\n\t\t%08X %-20s  <  %08X  <  %08X %-20s"),
					funcAddr[0],funcNames[0], s_StackFrame.AddrReturn.Offset, funcAddr[1],funcNames[1]);
				WriteFile(hFile, buffer, lstrlen(buffer),&nBytes,NULL);
			}
		}
	}

	if(boHaveSymbols) SymCleanup(GetCurrentProcess());
	return;
}

void CExpFltr::
get_func_names(DWORD pFunc, TCHAR funcNames[][64],DWORD funcAddr[])
// Get both functions around stack address given (pFunc): return their name and address
// Parameters: pFunc : address of stack pointer
//			   funcNames : array to fill with each function names
//			   funcAddr: array to fill with each function address
{
	// init to default values
	lstrcpy(funcNames[0],_T("Unknown")); funcAddr[0]=0;
	lstrcpy(funcNames[1],_T("Unknown")); funcAddr[1]=0;
	if (m_num_modules == 0) return;
	
	// find in which modules pFunc address is located
	short nmod;
	for (nmod=0 ; nmod < m_num_modules ; nmod++) {
		if (m_ptr_module_addr[nmod] > pFunc) break;
	}
	if (nmod > 0) nmod--;

	// now go load that module's information...
	DWORD *functions; WORD *ordinals;
	LPTSTR *names; DWORD nFunc,nNames;

	if (!loadExportLists(m_ptr_module_addr[nmod],&functions,&ordinals,&names,
		&nFunc,&nNames)) 
	{
		return;
	}

	short idxBefore,idxAfter,i;
	idxBefore=0;idxAfter=0;i=0;//BLBLBL 29 mars 2009 initialisation des vraribles à 9

	DWORD diff,diffBefore,diffAfter;
	BYTE *flag_ptr; DWORD add_addr, func_addr;
	// find which function's address are before and after the stack address
	for (i=0,diffBefore=diffAfter=0xFFFFFFFF ; i < (short)nFunc ; i++) {
		if (functions[i] != 0) { // skip over gaps
			// get function real address
			func_addr = functions[i]+m_ptr_module_addr[nmod];

			// update address if they jmp to another address 
			// jmp code is E9 followed by the address to add (little endian)+ 5 bytes
			flag_ptr = (BYTE *)func_addr;
			if (*flag_ptr == 0xE9) {
				// get jmp address (which is in little endian value)
				add_addr = (DWORD) (flag_ptr[1] + flag_ptr[2]*256 + flag_ptr[3]*256*256
					+ flag_ptr[4]*256*256*256) + 5;
				func_addr += add_addr; 
			}

			if (pFunc > func_addr) {
				diff=pFunc-func_addr;
				if (diff < diffBefore) {
					// function is closer to stack address then previous one
					diffBefore=diff;
					idxBefore=i;
					// update new address in struct since we make calculation here
					funcAddr[0] = func_addr;
				}
			}
			else {
				diff=func_addr-pFunc;
				if (diff < diffAfter) {
					// function is closer to stack address then previous one
					diffAfter=diff;
					idxAfter=i;
					// update new address in struct since we make calculation here
					funcAddr[1] = func_addr;
				}
			}
		}
	}

	LPTSTR pNameBefore,pNameAfter;
	pNameBefore=pNameAfter=NULL;
	// get names associated with functions found (before/after)
	if (diffBefore != 0xFFFFFFFF) {
		for (i=0 ; i < (short)nNames ; i++) {
			if (ordinals[i] == idxBefore) {
				pNameBefore=names[i]+m_ptr_module_addr[nmod];
				break;
			}
		}
	}
	if (diffAfter != 0xFFFFFFFF) {
		for (i=0 ; i < (short)nNames ; i++) {
			if (ordinals[i] == idxAfter) {
				pNameAfter=names[i]+m_ptr_module_addr[nmod];
				break;
			}
		}
	}

	// print up the function names in the return structures..
	if (pNameBefore != NULL) {
		lstrcpyn(funcNames[0],pNameBefore,(sizeof(funcNames[0])/sizeof(TCHAR)));
	}
	
	if (pNameAfter != NULL) {
		lstrcpyn(funcNames[1],pNameAfter,(sizeof(funcNames[1])/sizeof(TCHAR)));
	}
}


BOOL CExpFltr::
loadExportLists(DWORD module_addr, DWORD **functions, WORD **ordinals, LPTSTR **names, 
				DWORD *nFunc, DWORD *nNames)
// Loads exported sections of a module: functions names/address/ordinals
// Parameters: module_addr : address of module to load
//             functions: fill structure with address of functions
//             ordinals: fill struct with ordinals of functions
//             names: fill struct with names of functions
//             nFunc and nNames returns the number of functions address/names found
{
	PIMAGE_NT_HEADERS pNTHeader=NULL;
	PIMAGE_DOS_HEADER pDOSHeader;
	PIMAGE_EXPORT_DIRECTORY pExportDir;
	DWORD exportsStartRVA;
	
	pDOSHeader=(PIMAGE_DOS_HEADER)module_addr;
	pNTHeader=makePtr(PIMAGE_NT_HEADERS,pDOSHeader,pDOSHeader->e_lfanew);

	// First, verify that the e_lfanew field gave us a reasonable
	// pointer, then verify the PE signature.
	__try {
		if (pNTHeader->Signature != IMAGE_NT_SIGNATURE) return FALSE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER) {   // Should only get here if pNTHeader is bogus
          return FALSE;
	}
	
	exportsStartRVA= pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	pExportDir=makePtr(PIMAGE_EXPORT_DIRECTORY,module_addr,exportsStartRVA);
	*functions=(PDWORD)((DWORD)pExportDir->AddressOfFunctions+module_addr);
	*ordinals=(PWORD)((DWORD)pExportDir->AddressOfNameOrdinals+module_addr);
	*names=(LPTSTR *)((DWORD)pExportDir->AddressOfNames+module_addr);
	*nFunc=pExportDir->NumberOfFunctions;
	*nNames=pExportDir->NumberOfNames;
	return(TRUE);
}

LPTSTR CExpFltr::
memType2Text(DWORD memType) 
// Converts memory type to a string format
{
	switch (memType) {
	case MEM_FREE:
		return(_T("Free   "));
	case MEM_RESERVE:
		return(_T("Reserve"));
	case MEM_IMAGE:
		return(_T("Image  "));
	case MEM_MAPPED:
		return(_T("Mapped "));
	case MEM_PRIVATE:
		return(_T("Private"));
	case MEM_COMMIT:
		return(_T("Commit"));
	}
	return(_T("Unknown"));
}


void CExpFltr::
killIT(void)
// kill the program..
{
	// STOP service/program notification in this case..
	if (m_stop) m_stop(NULL);
	else 
	{
		// launch AutoRestart if specified
		if (m_szRestartCommand[0])
		{
			TCHAR szCommandLine[_MAX_PATH*2+32];
			wsprintf(szCommandLine, "\"%s\" %s", m_szRestartCommand, m_szRestartArguments[0]? m_szRestartArguments : "");

			STARTUPINFO si;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);

			PROCESS_INFORMATION pi;
			ZeroMemory(&pi, sizeof(pi));
				
			BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, 0, NULL, 
				NULL, &si, &pi);

			if (fSuccess)
			{
				// close all handles, so child process can run detached...
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
			}
		}
		ExitProcess(1);
	}
	return;
}

// outside UTILITIES..
void CExpFltr::
SetReportFile(LPTSTR file_name)
// set the report file to be used for reports of exception errors
{
	__try {
		EnterCriticalSection(&m_crit);
		if (lstrlen(file_name) < _MAX_PATH)
			lstrcpy(CExpFltr::m_report_file, file_name);
	}
	__finally{ LeaveCriticalSection(&m_crit); }
	return;
}

void CExpFltr::
SetAppTitle(LPTSTR app_title)
// Set APPlication Title that will appear in the reports
{
	__try {
		EnterCriticalSection(&m_crit);
		if (lstrlen(app_title) < MAX_SZ_APP_TITLE)
			lstrcpy(CExpFltr::m_app_title, app_title);
	}
	__finally{ LeaveCriticalSection(&m_crit); }
	return;
}

// debugging purposes (exported symbols)
EXPORT void ExpFltr_end(void) {}
