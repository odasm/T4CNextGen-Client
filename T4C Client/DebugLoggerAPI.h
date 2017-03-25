// DebugLoggerAPI.h: interface for the DebugLoggerAPI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEBUGLOGGERAPI_H__EE8C70E6_987A_11D4_B635_00E0296B9056__INCLUDED_)
#define AFX_DEBUGLOGGERAPI_H__EE8C70E6_987A_11D4_B635_00E0296B9056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DebugLoggerAPI  
{
public:
	virtual ~DebugLoggerAPI();

    static DebugLoggerAPI *GetInstance();

    void Log( const char *sz );

private:
    DebugLoggerAPI();

};

#endif // !defined(AFX_DEBUGLOGGERAPI_H__EE8C70E6_987A_11D4_B635_00E0296B9056__INCLUDED_)
