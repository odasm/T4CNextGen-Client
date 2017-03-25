/***************************************************************************
 * Exception Filter Class - Header			                               *
 * Benoit Martin                                                           *
 * Copyright (C) 1997, Vircom Inc.										   *
 ***************************************************************************/
#ifndef __EXPFLTR_H_
#define __EXPFLTR_H_

// comments if not using the LOGGER CLASS
// #define _USE_LOGGER_ // for expfltr.h

#define MAX_SZ_APP_TITLE	64
#define MAX_STACK			32
#define MAX_MODULES			512
#define MAX_FUNC			20  // maximum number of functions in call stack

#define makePtr(cast,ptr,addValue) (cast)((DWORD)(ptr)+(DWORD)(addValue))
#ifndef EXPORT
#define EXPORT	extern "C" __declspec (dllexport)
#endif

typedef void (* VoidFunction)(void *);

typedef struct {                   // region info
	void *rgnAddress;             //   address of mem region
	DWORD rgnProtection;          //   protection attribute
	DWORD rgnSize;                //   size
	DWORD rgnType;                //   type (Free,Image,Mapped,Private)
	DWORD rgnBlocks;              //   number of blocks
	DWORD rgnGuardBlks;           //   blocks with "guard" attribute
	BOOL rgnIsStack;             //   is region a thread stack
} RGNINFO;


class CExpFltr {
public:
	~CExpFltr(void);
	static void InitFilter(void *stop, BYTE max_GP);
	static LONG Filter(LPEXCEPTION_POINTERS lp);
	static void SetReportFile(LPTSTR file_name);
	static void SetAppTitle(LPTSTR app_title);
	static void LogException(CONTEXT *ctxtRec, LPTSTR errorMsg, void *excpAddr);
	static CExpFltr &GetInstance(void);
	static void SetAutoRestart(char *szCommand, char *szArguments);
	static void DisableAutoRestart();

private: // private functions
	CExpFltr(void);
	HMODULE m_dll; // for Dynamic IMAGEHLP

	static void log_mem_map(HANDLE hFile, DWORD **stackStart, DWORD **stackEnd, 
		DWORD *nStacks);
	static BOOL get_region_info (void *memStart, RGNINFO *rgnInfo);
	static DWORD fixVirtualQuery (LPVOID lpvAddr, PMEMORY_BASIC_INFORMATION pmem_bi, 
		DWORD cbLen);
	static void walkMemRegion(RGNINFO *rgnInfo);
	static void region_map_line(RGNINFO *rgnInfo, LPTSTR buf, WORD buf_size);
	static void add_module(LPTSTR buf, short buf_len, LPTSTR module_name);
	static void log_stack(HANDLE hFile, DWORD *esp, DWORD *esb, DWORD **stackStart, 
		DWORD **stackEnd, DWORD nStacks, void *excpAddr, LPCONTEXT lpContext);
	static void get_func_names(DWORD pFunc, TCHAR funcNames[][64],DWORD funcAddr[]);
	static BOOL loadExportLists(DWORD module_addr, DWORD **functions, 
		WORD **ordinals, LPTSTR **names, DWORD *nFunc, DWORD *nNames);

	static void killIT(void);
	static LPTSTR memType2Text(DWORD memType);

	// variables..
	static CRITICAL_SECTION m_crit;
	static TCHAR m_report_file[_MAX_PATH];
	static TCHAR m_app_title[MAX_SZ_APP_TITLE];
	static WORD m_num_modules;
	static DWORD m_ptr_module_addr[MAX_MODULES];
	static BYTE m_GP_count;
	static BYTE m_max_GP;
	static VoidFunction m_stop;

	static CExpFltr m_Instance;
	static BOOL m_bUseStackWalk;
	static TCHAR m_szRestartCommand[_MAX_PATH];
	static TCHAR m_szRestartArguments[_MAX_PATH];
};

#endif