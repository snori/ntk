# Microsoft Developer Studio Project File - Name="ntks" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ntks - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "ntks.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "ntks.mak" CFG="ntks - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "ntks - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "ntks - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ntks - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../lib"
# PROP Intermediate_Dir "./ntks"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../.." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "ntks - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../lib"
# PROP Intermediate_Dir "./ntksd"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "../.." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "NTK_CHECK_MEMORY_LEAKS" /D "NTK_USE_DEBUG_NEW" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/ntksd.lib"

!ENDIF 

# Begin Target

# Name "ntks - Win32 Release"
# Name "ntks - Win32 Debug"
# Begin Group "Application Kit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\ntk\application\src\application.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\application.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\application.i
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\src\applicationdefs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\applicationdefs.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\src\clipboard.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\clipboard.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\defs.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\src\handler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\handler.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\handler.i
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\src\invoker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\invoker.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\kit.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\src\looper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\looper.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\looper.i
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\src\message.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\message.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\message.i
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\src\messagefilter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\messagefilter.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\messagefilter.i
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\src\messagequeue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\messagequeue.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\src\messenger.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\application\messenger.h
# End Source File
# End Group
# Begin Group "Interface Kit Basic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\ntk\interface\src\bitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\bitmap.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\dc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\dc.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\defs.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\gdiobject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\gdiobject.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\interfacedefs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\interfacedefs.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\interfaceutility.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\interfaceutility.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\kit.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\panel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\panel.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\point.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\point.i
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\rect.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\rect.i
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\utility.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\view.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\view.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\view.i
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\window.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\window.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\window.i
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\windowcontrol.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\windowcontrol.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\wnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\wnd.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\wnd.i
# End Source File
# End Group
# Begin Group "Interface Kit Controls"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\ntk\interface\src\alert.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\alert.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\button.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\button.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\control.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\control.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\defaultwindowcontrol.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\defaultwindowcontrol.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\filepanel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\filepanel.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\fontpanel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\fontpanel.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\glview.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\glview.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\htmlview.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\htmlview.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\menu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\menu.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\menubar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\menubar.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\menuitem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\menuitem.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\menuwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\menuwindow.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\scintilla.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\scintilla.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\screen.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\screen.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\scrollbar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\scrollbar.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\scrollview.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\scrollview.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\statusbar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\statusbar.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\stringview.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\stringview.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\systemcontrol.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\systemcontrol.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\tabbar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\tabbar.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\terminal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\terminal.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\terminalview.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\terminalview.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\src\textview.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\textview.h
# End Source File
# End Group
# Begin Group "Interface Kit STD Controls"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\ntk\interface\std\menu.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\std\menubar.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\std\menuitem.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\std\src\stdmenu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\std\stdmenu.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\std\src\stdmenubar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\std\stdmenubar.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\std\src\stdmenuitem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\std\stdmenuitem.h
# End Source File
# End Group
# Begin Group "Interface Kit Utility Controls"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\ntk\interface\src\dialogpanel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\dialogpanel.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\interface\layout.h
# End Source File
# End Group
# Begin Group "Kernel Kit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\ntk\kernel\kit.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\kernel\src\locker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\kernel\locker.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\kernel\src\thread.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\kernel\thread.h
# End Source File
# End Group
# Begin Group "Storage Kit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\ntk\storage\src\directory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\storage\directory.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\storage\src\entry.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\storage\entry.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\storage\src\entrylist.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\storage\entrylist.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\storage\src\file.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\storage\file.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\storage\kit.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\storage\src\node.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\storage\node.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\storage\src\path.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\storage\path.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\storage\src\symlink.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\storage\symlink.h
# End Source File
# End Group
# Begin Group "Support Kit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\ntk\support\src\archivable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\archivable.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\autolock.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\src\dataio.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\dataio.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\src\debug.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\debug.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\debug.i
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\src\flattenable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\flattenable.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\src\hash.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\hash.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\kit.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\src\observable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\observable.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\refcount.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\src\setting.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\setting.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\src\statable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\statable.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\src\status.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\status.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\status.i
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\src\stopwatch.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\stopwatch.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\src\string.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\string.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\src\supportutility.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\supportutility.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\utility.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\src\version.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\version.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\support\version.i
# End Source File
# End Group
# Begin Group "Windows Kit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\ntk\windows\src\atl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\windows\atl.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\windows\windows.h
# End Source File
# End Group
# Begin Group "Translation Kit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\ntk\translation\kit.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\translation\src\translationutility.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\translation\translationutility.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\translation\src\translator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\translation\translator.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\translation\src\translatorroster.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ntk\translation\translatorroster.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\translation\utility.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\changelog.txt
# End Source File
# Begin Source File

SOURCE=..\..\coding.txt
# End Source File
# Begin Source File

SOURCE=..\..\ntk\config.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\defs.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\defs.i
# End Source File
# Begin Source File

SOURCE=..\..\ntk\main.h
# End Source File
# Begin Source File

SOURCE=..\..\ntk\ntk.h
# End Source File
# Begin Source File

SOURCE=..\..\readme.txt
# End Source File
# End Target
# End Project
