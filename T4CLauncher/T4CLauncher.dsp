# Microsoft Developer Studio Project File - Name="T4CLauncher" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=T4CLauncher - Win32 Debug Maison
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "T4CLauncher.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "T4CLauncher.mak" CFG="T4CLauncher - Win32 Debug Maison"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "T4CLauncher - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "T4CLauncher - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "T4CLauncher - Win32 Debug Maison" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/_T4C Server V2/T4C CLIENT_SRC_V2", GOZFAAAA"
# PROP Scc_LocalPath ".."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "T4CLauncher - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /w /W0 /GX /Od /I "..\DirectX8Lib\include" /I "ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0c /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc0c /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 Zlib.lib Bass.Lib winmm.lib msacm32.lib olepro32.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib ole32.lib oleaut32.lib advapi32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../Release/T4CLauncher.dll" /libpath:"..\DirectX8Lib\Lib" /libpath:"ZLib"

!ELSEIF  "$(CFG)" == "T4CLauncher - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\DirectX8Lib\include" /I "ZLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0c /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc0c /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Zlib.lib Bass.Lib winmm.lib msacm32.lib olepro32.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib ole32.lib oleaut32.lib advapi32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../Debug/T4CLauncher.dll" /pdbtype:sept /libpath:"..\DirectX8Lib\Lib" /libpath:"ZLib"

!ELSEIF  "$(CFG)" == "T4CLauncher - Win32 Debug Maison"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "T4CLauncher___Win32_Debug_Maison"
# PROP BASE Intermediate_Dir "T4CLauncher___Win32_Debug_Maison"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\DirectX8Lib\include" /I "ZLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\DirectX8Lib\include" /I "ZLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0c /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc0c /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Zlib.lib Bass.Lib winmm.lib msacm32.lib olepro32.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib ole32.lib oleaut32.lib advapi32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"..\DirectX8Lib\Lib" /libpath:"ZLib"
# ADD LINK32 Zlib.lib Bass.Lib winmm.lib msacm32.lib olepro32.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib ole32.lib oleaut32.lib advapi32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../Debug/T4CLauncher.dll" /pdbtype:sept /libpath:"..\DirectX8Lib\Lib" /libpath:"ZLib"
# SUBTRACT LINK32 /force

!ENDIF 

# Begin Target

# Name "T4CLauncher - Win32 Release"
# Name "T4CLauncher - Win32 Debug"
# Name "T4CLauncher - Win32 Debug Maison"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\encrandom.cpp

!IF  "$(CFG)" == "T4CLauncher - Win32 Release"

!ELSEIF  "$(CFG)" == "T4CLauncher - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4CLauncher - Win32 Debug Maison"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GUILocalString.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\MacroDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MainDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\NMDShow.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\T4CLauncher.cpp
# End Source File
# Begin Source File

SOURCE=.\T4CLauncher.def
# End Source File
# Begin Source File

SOURCE=.\T4CLauncher.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\encrandom.h
# End Source File
# Begin Source File

SOURCE=.\GUILocalString.h
# End Source File
# Begin Source File

SOURCE=.\MacroDialog.h
# End Source File
# Begin Source File

SOURCE=.\MainDialog.h
# End Source File
# Begin Source File

SOURCE=.\NMDShow.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ServerDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\T4CLauncher.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BackIntro.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnFrame1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnFrame2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnFrame3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnFrame4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnFrame5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CCHSB.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CreditDialsoft.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CreditIntro.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CreditV2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CreditVircom.bmp
# End Source File
# Begin Source File

SOURCE=.\res\H_POINT.CUR
# End Source File
# Begin Source File

SOURCE=.\res\IpContour.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LINK.CUR
# End Source File
# Begin Source File

SOURCE=.\res\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MainDlg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NORMAL01.CUR
# End Source File
# Begin Source File

SOURCE=.\res\ServerDlg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\T4CLauncher.rc2
# End Source File
# End Group
# Begin Group "Controls"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Controls\ColoredListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Controls\ColoredListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Controls\Common.cpp
# End Source File
# Begin Source File

SOURCE=.\Controls\Common.h
# End Source File
# Begin Source File

SOURCE=.\Controls\CreditsCtrl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\Controls\CreditsCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Controls\DIB.cpp
# End Source File
# Begin Source File

SOURCE=.\Controls\DIB.h
# End Source File
# Begin Source File

SOURCE=.\Controls\DropBtn.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\Controls\DropBtn.h
# End Source File
# Begin Source File

SOURCE=.\Controls\EditTrans.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\Controls\EditTrans.h
# End Source File
# Begin Source File

SOURCE=.\Controls\NMTransListCtrl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\Controls\NMTransListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Controls\SkinButton.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\Controls\SkinButton.h
# End Source File
# Begin Source File

SOURCE=.\Controls\uicLabel.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\Controls\uicLabel.h
# End Source File
# End Group
# Begin Group "Lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\DirectX8Lib\Lib\strmiids.lib
# End Source File
# Begin Source File

SOURCE=..\DirectX8Lib\Lib\quartz.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
