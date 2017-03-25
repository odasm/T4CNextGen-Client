// EasyMail.h: interface for the EasyMail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EASYMAIL_H__4A60F8DF_1525_11D3_8518_00E02922FA40__INCLUDED_)
#define AFX_EASYMAIL_H__4A60F8DF_1525_11D3_8518_00E02922FA40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import "emsmtp.dll" no_namespace

#include <list>

class EasyMail  
{
public:
    bool operator ()( const int dwPort,const char *szMailServer, const char *szFrom, const char *szTo, const char *szSubject, const char *szBodyText, const char *szAttach, const char *szAccount = NULL, const char *szPassword = NULL, std::list< const char * > *extraAttach = NULL ){
        HRESULT hr;
        long nResult;
 
        hr = OleInitialize( NULL );
        
        if( ( hr == S_OK ) || ( hr == S_FALSE ) ){
            IEasyMailSMTPObjPtr pSMTP( "EasyMail.SMTP.5" );
            pSMTP->TimeOut = 3; //5 sec
            pSMTP->MailPort = dwPort;//24;
            pSMTP->MailServer = szMailServer;
            pSMTP->FromAddr =   szFrom;
            pSMTP->AddRecipient( "", szTo, 1 );
            pSMTP->Subject =    szSubject;
            pSMTP->BodyText =   szBodyText;
            pSMTP->LicenseKey = "Vircom, Inc. (Single Developer)/00D0530210911E009A00";
            if( szAccount != NULL || szPassword != NULL ){
                pSMTP->ESMTP_AuthMode = 2;
            }
            
            if( szAccount != NULL ){
                pSMTP->ESMTP_Account = szAccount;
            }
            if( szPassword != NULL ){
                pSMTP->ESMTP_Password = szPassword;
            }
            if( szAttach != NULL ){
                pSMTP->AddAttachment( szAttach, 0 );
            }
            if( extraAttach != NULL ){
                std::list< const char * >::iterator i;
                for( i = extraAttach->begin(); i != extraAttach->end(); i++ ){
                    pSMTP->AddAttachment( *i, 0 );
                }
            }

            nResult =           pSMTP->Send();
            
            if( nResult == 0 ){
                return true;
            }
        }

        return false;
    }

};

#endif // !defined(AFX_EASYMAIL_H__4A60F8DF_1525_11D3_8518_00E02922FA40__INCLUDED_)
