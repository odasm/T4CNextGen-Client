# Microsoft Developer Studio Project File - Name="T4C Client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=T4C Client - Win32 Debug Maison
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "T4C Client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "T4C Client.mak" CFG="T4C Client - Win32 Debug Maison"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "T4C Client - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "T4C Client - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "T4C Client - Win32 Debug Maison" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/_T4C Server V2/T4C CLIENT_SRC_V2", GOZFAAAA"
# PROP Scc_LocalPath ".."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /w /W0 /GR /GX /I "..\..\SXL\include" /I "..\..\TFCCryptC\include" /I "Zlib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "NLOG_LOCK" /Fr /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "NDEBUG"
# ADD RSC /l 0x1009 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib sxl.lib TFCCrypt.lib /nologo /subsystem:windows /machine:I386 /out:"../Release/T4C.EXE" /libpath:"..\..\SXL\lib" /libpath:"..\..\TFCCryptC\Lib" /libpath:"Zlib"
# SUBTRACT LINK32 /pdb:none /map /debug

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GR /GX /ZI /Od /I "..\..\SXL\include" /I "..\..\TFCCryptC\include" /I "Zlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "NLOG_LOCK" /FR /YX"global.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "_DEBUG"
# ADD RSC /l 0x1009 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 vfw32.lib zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib sxl.lib TFCCrypt.Lib /nologo /stack:0xf4240 /subsystem:windows /debug /debugtype:both /machine:I386 /out:"../Debug/T4C.EXE" /pdbtype:sept /libpath:"..\..\SXL\lib" /libpath:"..\..\TFCCryptC\Lib" /libpath:"Zlib"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "T4C_Client___Win32_Debug_Maison"
# PROP BASE Intermediate_Dir "T4C_Client___Win32_Debug_Maison"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GR /GX /ZI /Od /I "C:\marc\SXL\include" /I "..\..\SXL\include" /I "C:\marc\TFCCrypt\include" /I "..\..\TFCCrypt\include" /I "Zlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "NLOG_LOCK" /FR /YX"global.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GR /GX /ZI /Od /I "..\..\SXL\include" /I "..\..\TFCCryptC\include" /I "Zlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "NLOG_LOCK" /FR /YX"global.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "_DEBUG"
# ADD RSC /l 0x1009 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 vfw32.lib zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib sxl.lib TFCCrypt.Lib /nologo /subsystem:windows /debug /debugtype:both /machine:I386 /pdbtype:sept /libpath:"C:\marc\SXL\lib" /libpath:"..\..\SXL\lib" /libpath:"C:\marc\TFCCrypt\Lib" /libpath:"..\..\TFCCrypt\Lib" /libpath:"Zlib"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 vfw32.lib zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib sxl.lib TFCCrypt.Lib /nologo /subsystem:windows /debug /debugtype:both /machine:I386 /out:"../Debug/T4C M.exe" /pdbtype:sept /libpath:"..\..\SXL\lib" /libpath:"..\..\TFCCryptC\Lib" /libpath:"Zlib"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "T4C Client - Win32 Release"
# Name "T4C Client - Win32 Debug"
# Name "T4C Client - Win32 Debug Maison"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "MP3 Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\2.cpp
# End Source File
# Begin Source File

SOURCE=.\Audio.cpp
# End Source File
# Begin Source File

SOURCE=.\buffer_nt.cpp
# End Source File
# Begin Source File

SOURCE=.\Formats.cpp
# End Source File
# Begin Source File

SOURCE=.\Getopt.cpp
# End Source File
# Begin Source File

SOURCE=.\Util.cpp
# End Source File
# End Group
# Begin Group "New Interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NewInterface\AnimUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\BoxUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\ButtonUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\BuyUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\CharacterUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\ChatterUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\ChestUI.cpp
# End Source File
# Begin Source File

SOURCE=.\NewInterface\ControlUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\DropZoneUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\EditUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\EffectStatusUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\EmptyEvent.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\EventVisitor.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\GameUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\GraphUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\GridUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\GroupPlayUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\GuildUI.cpp
# End Source File
# Begin Source File

SOURCE=.\NewInterface\InventoryUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\ListUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\MacroUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\OptionsUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\RobUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\RootBoxUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\RTHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\NewInterface\RTItemI.cpp
# End Source File
# Begin Source File

SOURCE=.\NewInterface\RTMap.cpp
# End Source File
# Begin Source File

SOURCE=.\NewInterface\ScrollUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\SellUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\SideMenu.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\SkillTeachUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\SkillTrainUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\SliderUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\SpellUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\StaticTextUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /GR
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /GR
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\TextPageUI.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yu"..\pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewInterface\Todo.txt
# End Source File
# Begin Source File

SOURCE=.\NewInterface\TradeUI.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\App.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Autolock.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CDAduio.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Comm.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CommCenter.cpp
# End Source File
# Begin Source File

SOURCE=.\database2.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DebugLoggerAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectSocket.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DirectXInput.CPP

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Disp.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\encrandom.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Encrypter.cpp
# End Source File
# Begin Source File

SOURCE=.\Expfltr.cpp
# End Source File
# Begin Source File

SOURCE=.\Fading.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FileLogger.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FormatText.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GameIcons.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GameMusic.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\GUILocalString.cpp
# End Source File
# Begin Source File

SOURCE=.\Icon3D.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IDirectXDraw.Cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IDirectXImage.Cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IDirectXPcx.Cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LightMap.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LocalString.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MacroHandler.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\main2.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Mathieu.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MemMapFile.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MemoryManager.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Mouse.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MouseAction.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewFont.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NewSound.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\newstring.cpp
# End Source File
# Begin Source File

SOURCE=.\NMVideoCapture.cpp
# End Source File
# Begin Source File

SOURCE=.\NTime.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Packet.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\packethandling.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\pch.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /GR /Yc"pch.h"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# ADD BASE CPP /Yc
# ADD CPP /Yc

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pf.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PowerDebug.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ProcStat.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Puppet.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PvpRanking.cpp
# End Source File
# Begin Source File

SOURCE=.\Random.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Request.cpp
# End Source File
# Begin Source File

SOURCE=.\Resources.rc
# End Source File
# Begin Source File

SOURCE=.\SaveGame.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Spell.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sprite.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SysMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemAction.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SystemInfo.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\T3 Sound Buffer.cpp"
# End Source File
# Begin Source File

SOURCE=".\T3 Sound Manager.cpp"
# End Source File
# Begin Source File

SOURCE=".\T3 Wave Filter.cpp"
# End Source File
# Begin Source File

SOURCE=.\TFCObject.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TFCPacket.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TFCPlayer.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TFCSocket.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tileset.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UIMouseEvent.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\V2Database.cpp
# End Source File
# Begin Source File

SOURCE=.\V2PalManager.cpp
# End Source File
# Begin Source File

SOURCE=.\V2Sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\version.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VideoCapture.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VisualObjectList.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# ADD CPP /O2
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VKey.cpp

!IF  "$(CFG)" == "T4C Client - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "T4C Client - Win32 Debug Maison"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\weather.cpp
# End Source File
# Begin Source File

SOURCE=.\XPStat.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "MP3 Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Audio.h
# End Source File
# Begin Source File

SOURCE=.\Audioio.h
# End Source File
# Begin Source File

SOURCE=.\Formats.h
# End Source File
# Begin Source File

SOURCE=.\Getopt.h
# End Source File
# Begin Source File

SOURCE=.\Proto.h
# End Source File
# End Group
# Begin Group "New Interface No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NewInterface\AnimUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\BoxUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\ButtonUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\BuyUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\CharacterUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\ChatterUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\ChestUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\ControlUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\DropZoneUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\EditUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\EffectStatusUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\EmptyEvent.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\EventVisitor.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\GameUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\GraphUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\GridUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\GroupPlayUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\GuildUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\InventoryUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\ListUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\MacroUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\OptionsUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\RobUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\RootBoxUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\RTHelp.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\RTItemI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\RTMap.h
# End Source File
# Begin Source File

SOURCE=.\ScrollUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\SellUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\SideMenu.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\SkillTeachUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\SkillTrainUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\SliderUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\SpellUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\StaticTextUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\TextPageUI.h
# End Source File
# Begin Source File

SOURCE=.\NewInterface\TradeUI.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\App.h
# End Source File
# Begin Source File

SOURCE=.\Apparence.h
# End Source File
# Begin Source File

SOURCE=.\AutoLock.h
# End Source File
# Begin Source File

SOURCE=.\AviFile.h
# End Source File
# Begin Source File

SOURCE=.\BasicSprite.h
# End Source File
# Begin Source File

SOURCE=.\Bitmap.h
# End Source File
# Begin Source File

SOURCE=.\BoxListing.h
# End Source File
# Begin Source File

SOURCE=.\C2.h
# End Source File
# Begin Source File

SOURCE=.\CombatCursor.h
# End Source File
# Begin Source File

SOURCE=.\Comm.h
# End Source File
# Begin Source File

SOURCE=.\CommCenter.h
# End Source File
# Begin Source File

SOURCE=.\ComPacketHeader.h
# End Source File
# Begin Source File

SOURCE=.\Cs.h
# End Source File
# Begin Source File

SOURCE=.\database2.h
# End Source File
# Begin Source File

SOURCE=.\DebugLoggerAPI.h
# End Source File
# Begin Source File

SOURCE=.\Dinput.h
# End Source File
# Begin Source File

SOURCE=.\DirectSocket.h
# End Source File
# Begin Source File

SOURCE=.\DirectXInput.H
# End Source File
# Begin Source File

SOURCE=.\Disp.h
# End Source File
# Begin Source File

SOURCE=.\EasyMail.h
# End Source File
# Begin Source File

SOURCE=.\encrandom.h
# End Source File
# Begin Source File

SOURCE=.\Encrypter.h
# End Source File
# Begin Source File

SOURCE=.\Expfltr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source~1\t4cwor~1\EXECPTION\Expfltr.h
# End Source File
# Begin Source File

SOURCE=.\Fading.h
# End Source File
# Begin Source File

SOURCE=.\FileLogger.h
# End Source File
# Begin Source File

SOURCE=.\FormatText.h
# End Source File
# Begin Source File

SOURCE=.\Franck.h
# End Source File
# Begin Source File

SOURCE=.\GameIcons.h
# End Source File
# Begin Source File

SOURCE=.\GameMusic.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\GUILocalString.h
# End Source File
# Begin Source File

SOURCE=.\Icon3D.h
# End Source File
# Begin Source File

SOURCE=.\IDirectXDraw.h
# End Source File
# Begin Source File

SOURCE=.\IDirectXFont.H
# End Source File
# Begin Source File

SOURCE=.\IDirectXImage.H
# End Source File
# Begin Source File

SOURCE=.\IDirectXPcx.H
# End Source File
# Begin Source File

SOURCE=.\IDirectXSprite.H
# End Source File
# Begin Source File

SOURCE=.\LightMap.h
# End Source File
# Begin Source File

SOURCE=.\LocalString.h
# End Source File
# Begin Source File

SOURCE=.\Lock.h
# End Source File
# Begin Source File

SOURCE=.\logger.h
# End Source File
# Begin Source File

SOURCE=.\MacroHandler.h
# End Source File
# Begin Source File

SOURCE=.\MemMapFile.h
# End Source File
# Begin Source File

SOURCE=.\MemoryManager.h
# End Source File
# Begin Source File

SOURCE=.\Mouse.h
# End Source File
# Begin Source File

SOURCE=.\MouseAction.h
# End Source File
# Begin Source File

SOURCE=.\NewFont.h
# End Source File
# Begin Source File

SOURCE=.\NewSound.h
# End Source File
# Begin Source File

SOURCE=.\newstring.h
# End Source File
# Begin Source File

SOURCE=.\NMVideoCapture.h
# End Source File
# Begin Source File

SOURCE=.\NTime.h
# End Source File
# Begin Source File

SOURCE=.\ObjectListing.h
# End Source File
# Begin Source File

SOURCE=.\Old.h
# End Source File
# Begin Source File

SOURCE=.\Packet.h
# End Source File
# Begin Source File

SOURCE=.\packethandling.h
# End Source File
# Begin Source File

SOURCE=.\PacketList.h
# End Source File
# Begin Source File

SOURCE=.\PacketTypes.h
# End Source File
# Begin Source File

SOURCE=.\pch.h
# End Source File
# Begin Source File

SOURCE=.\Pf.h
# End Source File
# Begin Source File

SOURCE=.\PowerDebug.h
# End Source File
# Begin Source File

SOURCE=.\ProcStat.h
# End Source File
# Begin Source File

SOURCE=.\Puppet.h
# End Source File
# Begin Source File

SOURCE=.\PvpRanking.h
# End Source File
# Begin Source File

SOURCE=.\Random.h
# End Source File
# Begin Source File

SOURCE=.\Request.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SaveGame.h
# End Source File
# Begin Source File

SOURCE=.\Spell.h
# End Source File
# Begin Source File

SOURCE=.\Sprite.h
# End Source File
# Begin Source File

SOURCE=.\SysMsg.h
# End Source File
# Begin Source File

SOURCE=.\System.h
# End Source File
# Begin Source File

SOURCE=.\SystemAction.h
# End Source File
# Begin Source File

SOURCE=.\SystemInfo.h
# End Source File
# Begin Source File

SOURCE=.\T3Sound.h
# End Source File
# Begin Source File

SOURCE=.\TemplateList.h
# End Source File
# Begin Source File

SOURCE=.\templatequeues.h
# End Source File
# Begin Source File

SOURCE=.\TFCFlag.h
# End Source File
# Begin Source File

SOURCE=.\TFCLanddef.h
# End Source File
# Begin Source File

SOURCE=.\TFCObject.h
# End Source File
# Begin Source File

SOURCE=.\TFCPacket.h
# End Source File
# Begin Source File

SOURCE=.\TFCPlayer.h
# End Source File
# Begin Source File

SOURCE=.\TFCSocket.h
# End Source File
# Begin Source File

SOURCE=.\TileSet.h
# End Source File
# Begin Source File

SOURCE=.\UIMouseEvent.h
# End Source File
# Begin Source File

SOURCE=.\V2Database.h
# End Source File
# Begin Source File

SOURCE=.\V2PalManager.h
# End Source File
# Begin Source File

SOURCE=.\V2Sprite.h
# End Source File
# Begin Source File

SOURCE=.\Vdlist.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# Begin Source File

SOURCE=.\VideoCapture.h
# End Source File
# Begin Source File

SOURCE=.\VisualObjectList.h
# End Source File
# Begin Source File

SOURCE=.\VKey.h
# End Source File
# Begin Source File

SOURCE=.\WarningBox.h
# End Source File
# Begin Source File

SOURCE=.\weather.h
# End Source File
# Begin Source File

SOURCE=.\XPStat.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\newicons.ico
# End Source File
# End Group
# Begin Group "Text Files"

# PROP Default_Filter "txt"
# Begin Source File

SOURCE=.\Cd.txt
# End Source File
# Begin Source File

SOURCE=.\ENGLISH.lng
# End Source File
# Begin Source File

SOURCE=.\french.txt
# End Source File
# Begin Source File

SOURCE=.\Ignore.txt
# End Source File
# Begin Source File

SOURCE=.\Language.txt
# End Source File
# Begin Source File

SOURCE=.\License.txt
# End Source File
# Begin Source File

SOURCE=.\Release.txt
# End Source File
# Begin Source File

SOURCE=.\ServerList.Txt
# End Source File
# End Group
# Begin Group "Lib Files"

# PROP Default_Filter "lib"
# Begin Source File

SOURCE=.\Lib\Ddraw.lib
# End Source File
# Begin Source File

SOURCE=.\Lib\Dinput.lib
# End Source File
# Begin Source File

SOURCE=.\Lib\Dsound.lib
# End Source File
# Begin Source File

SOURCE=.\Lib\Dxguid.lib
# End Source File
# Begin Source File

SOURCE=.\Lib\Imagehlp.lib
# End Source File
# Begin Source File

SOURCE=.\Lib\Winmm.lib
# End Source File
# Begin Source File

SOURCE=.\Lib\Wsock32.lib
# End Source File
# End Group
# Begin Group "MD5"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MD5\MD5Checksum.cpp
# End Source File
# Begin Source File

SOURCE=.\MD5\MD5Checksum.h
# End Source File
# Begin Source File

SOURCE=.\MD5\MD5ChecksumDefines.h
# End Source File
# End Group
# Begin Group "UDP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\UDP\LoopThread.cpp
# End Source File
# Begin Source File

SOURCE=.\UDP\LoopThread.h
# End Source File
# Begin Source File

SOURCE=.\UDP\Mutex.cpp
# End Source File
# Begin Source File

SOURCE=.\UDP\Mutex.h
# End Source File
# Begin Source File

SOURCE=.\UDP\Queue.cpp
# End Source File
# Begin Source File

SOURCE=.\UDP\Queue.h
# End Source File
# Begin Source File

SOURCE=.\UDP\QueueVector.cpp
# End Source File
# Begin Source File

SOURCE=.\UDP\QueueVector.h
# End Source File
# Begin Source File

SOURCE=.\UDP\Semaphore.cpp
# End Source File
# Begin Source File

SOURCE=.\UDP\Semaphore.h
# End Source File
# Begin Source File

SOURCE=.\UDP\Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\UDP\Thread.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\WebPatch.exe
# End Source File
# End Target
# End Project
