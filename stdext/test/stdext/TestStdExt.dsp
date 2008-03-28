# Microsoft Developer Studio Project File - Name="TestStdExt" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=TestStdExt - Win32 DebugNoSgiStl
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TestStdExt.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TestStdExt.mak" CFG="TestStdExt - Win32 DebugNoSgiStl"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TestStdExt - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "TestStdExt - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "TestStdExt - Win32 ReleaseStaticLib" (based on "Win32 (x86) Console Application")
!MESSAGE "TestStdExt - Win32 ReleaseST" (based on "Win32 (x86) Console Application")
!MESSAGE "TestStdExt - Win32 DebugNoSgiStl" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TestStdExt - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "../../include" /I "../../../stdext/include" /I "../../../boost" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /Zm128 /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"../../../winsdk/lib" /libpath:"../../../pthreads-w32/lib" /libpath:"../../../boost\libs\regex\build\vc6" /libpath:"../../../apr/Release"

!ELSEIF  "$(CFG)" == "TestStdExt - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../include" /I "../../../stdext/include" /I "../../../boost" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "APR_DECLARE_STATIC" /FD /GZ /Zm200 /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"../../../winsdk/lib" /libpath:"../../../pthreads-w32/lib" /libpath:"../../../boost\libs\regex\build\vc6" /libpath:"../../../apr/Debug" /libpath:"../../../apr/LibD"

!ELSEIF  "$(CFG)" == "TestStdExt - Win32 ReleaseStaticLib"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TestStdExt___Win32_ReleaseStaticLib"
# PROP BASE Intermediate_Dir "TestStdExt___Win32_ReleaseStaticLib"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseStaticLib"
# PROP Intermediate_Dir "ReleaseStaticLib"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "../../include" /I "../../../stdext/include" /I "../../../boost" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "../../include" /I "../../../stdext/include" /I "../../../boost" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "APR_DECLARE_STATIC" /FD /Zm128 /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"../../../winsdk/lib" /libpath:"../../../pthreads-w32/lib" /libpath:"../../../boost\libs\regex\build\vc6" /libpath:"../../../apr/Release"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"../../../winsdk/lib" /libpath:"../../../pthreads-w32/lib" /libpath:"../../../boost\libs\regex\build\vc6" /libpath:"../../../apr/LibR"

!ELSEIF  "$(CFG)" == "TestStdExt - Win32 ReleaseST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TestStdExt___Win32_ReleaseST"
# PROP BASE Intermediate_Dir "TestStdExt___Win32_ReleaseST"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseST"
# PROP Intermediate_Dir "ReleaseST"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "../../include" /I "../../../stdext/include" /I "../../../boost" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "APR_DECLARE_STATIC" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "../../include" /I "../../../stdext/include" /I "../../../boost" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /Zm128 /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"../../../winsdk/lib" /libpath:"../../../pthreads-w32/lib" /libpath:"../../../boost\libs\regex\build\vc6" /libpath:"../../../apr/LibR"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"../../../winsdk/lib" /libpath:"../../../pthreads-w32/lib" /libpath:"../../../boost\libs\regex\build\vc6" /libpath:"../../../apr/Release"

!ELSEIF  "$(CFG)" == "TestStdExt - Win32 DebugNoSgiStl"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TestStdExt___Win32_DebugNoSgiStl"
# PROP BASE Intermediate_Dir "TestStdExt___Win32_DebugNoSgiStl"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugNoSgiStl"
# PROP Intermediate_Dir "DebugNoSgiStl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../include" /I "../../../stdext/include" /I "../../../boost" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "APR_DECLARE_STATIC" /FD /GZ /Zm200 /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../include" /I "../../../stdext/include" /I "../../../boost" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "APR_DECLARE_STATIC" /D "NO_SGISTL" /FD /GZ /Zm200 /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"../../../winsdk/lib" /libpath:"../../../pthreads-w32/lib" /libpath:"../../../boost\libs\regex\build\vc6" /libpath:"../../../apr/Debug" /libpath:"../../../apr/LibD"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"../../../winsdk/lib" /libpath:"../../../pthreads-w32/lib" /libpath:"../../../boost\libs\regex\build\vc6" /libpath:"../../../apr/Debug" /libpath:"../../../apr/LibD"

!ENDIF 

# Begin Target

# Name "TestStdExt - Win32 Release"
# Name "TestStdExt - Win32 Debug"
# Name "TestStdExt - Win32 ReleaseStaticLib"
# Name "TestStdExt - Win32 ReleaseST"
# Name "TestStdExt - Win32 DebugNoSgiStl"
# Begin Group "src"

# PROP Default_Filter ""
# Begin Group "text"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\stdext\text\BasicAlgo.h
# End Source File
# Begin Source File

SOURCE=..\..\include\stdext\text\BasicString.h
# End Source File
# Begin Source File

SOURCE=..\..\include\stdext\text\BasicStringBuilder.h
# End Source File
# Begin Source File

SOURCE=..\..\include\stdext\text\StringAlgo.h
# End Source File
# Begin Source File

SOURCE=..\..\include\stdext\text\TempString.h
# End Source File
# Begin Source File

SOURCE=..\..\include\stdext\text\TextPool.h
# End Source File
# End Group
# Begin Group "archive"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\stdext\Archive.h
# End Source File
# Begin Source File

SOURCE=..\..\include\stdext\archive\Basic.h
# End Source File
# Begin Source File

SOURCE=..\..\include\stdext\archive\ReadArchive.h
# End Source File
# Begin Source File

SOURCE=..\..\include\stdext\archive\Reader.h
# End Source File
# Begin Source File

SOURCE=..\..\include\stdext\archive\Stdio.h
# End Source File
# Begin Source File

SOURCE=..\..\include\stdext\archive\Stream.h
# End Source File
# Begin Source File

SOURCE=..\..\include\stdext\archive\WriteArchive.h
# End Source File
# Begin Source File

SOURCE=..\..\include\stdext\archive\Writer.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\TestBoostRegEx.cpp
# End Source File
# Begin Source File

SOURCE=.\TestHashMap.cpp
# End Source File
# Begin Source File

SOURCE=.\TestProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\TestStdExt.cpp
# End Source File
# Begin Source File

SOURCE=.\TestStdNew.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\include\stdext\msvcrt.h
# End Source File
# End Target
# End Project
