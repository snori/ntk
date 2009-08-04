# Microsoft Developer Studio Project File - Name="titoon" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=titoon - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "titoon.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "titoon.mak" CFG="titoon - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "titoon - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "titoon - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "titoon - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../bin"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../.." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ntk.lib zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /libpath:"../../lib"

!ELSEIF  "$(CFG)" == "titoon - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../bin"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "../.." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ntkd.lib zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../../bin/titoond.exe" /pdbtype:sept /libpath:"../../lib"

!ENDIF 

# Begin Target

# Name "titoon - Win32 Release"
# Name "titoon - Win32 Debug"
# Begin Group "app"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\titoon.cpp
# End Source File
# Begin Source File

SOURCE=.\titoon.h
# End Source File
# Begin Source File

SOURCE=.\titoon.rc
# End Source File
# End Group
# Begin Group "core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\animation.cpp
# End Source File
# Begin Source File

SOURCE=.\animation.h
# End Source File
# Begin Source File

SOURCE=.\brush.cpp
# End Source File
# Begin Source File

SOURCE=.\brush.h
# End Source File
# Begin Source File

SOURCE=.\brushstroke.cpp
# End Source File
# Begin Source File

SOURCE=.\brushstroke.h
# End Source File
# Begin Source File

SOURCE=.\image.cpp
# End Source File
# Begin Source File

SOURCE=.\image.h
# End Source File
# End Group
# Begin Group "ui"

# PROP Default_Filter ""
# Begin Group "boxes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\compositebox.cpp
# End Source File
# Begin Source File

SOURCE=.\compositebox.h
# End Source File
# Begin Source File

SOURCE=.\editbox.cpp
# End Source File
# Begin Source File

SOURCE=.\editbox.h
# End Source File
# Begin Source File

SOURCE=.\effectbox.cpp
# End Source File
# Begin Source File

SOURCE=.\effectbox.h
# End Source File
# Begin Source File

SOURCE=.\iobox.cpp
# End Source File
# Begin Source File

SOURCE=.\iobox.h
# End Source File
# Begin Source File

SOURCE=.\paintbox.cpp
# End Source File
# Begin Source File

SOURCE=.\paintbox.h
# End Source File
# End Group
# Begin Group "dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\newdocumentdialog.cpp
# End Source File
# Begin Source File

SOURCE=.\newdocumentdialog.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\canvas.cpp
# End Source File
# Begin Source File

SOURCE=.\canvas.h
# End Source File
# Begin Source File

SOURCE=.\previewpanel.cpp
# End Source File
# Begin Source File

SOURCE=.\previewpanel.h
# End Source File
# Begin Source File

SOURCE=.\timecontrol.cpp
# End Source File
# Begin Source File

SOURCE=.\timecontrol.h
# End Source File
# Begin Source File

SOURCE=.\timeline.cpp
# End Source File
# Begin Source File

SOURCE=.\timeline.h
# End Source File
# Begin Source File

SOURCE=.\titoonwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\titoonwindow.h
# End Source File
# Begin Source File

SOURCE=.\toolbox.cpp
# End Source File
# Begin Source File

SOURCE=.\toolbox.h
# End Source File
# End Group
# Begin Group "util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\document.cpp
# End Source File
# Begin Source File

SOURCE=.\document.h
# End Source File
# Begin Source File

SOURCE=.\zfile.cpp
# End Source File
# Begin Source File

SOURCE=.\zfile.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\changelog.txt
# End Source File
# Begin Source File

SOURCE=.\readme.txt
# End Source File
# End Target
# End Project
