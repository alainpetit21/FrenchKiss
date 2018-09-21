<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: FrenchKISS - Win32 (WCE x86em) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "C:\DOCUME~1\Alain\LOCALS~1\Temp\RSP137.tmp" with contents
[
/nologo /W3 /Zi /Od /I "inc" /D "_POCKET_PC" /D "DEBUG" /D _WIN32_WCE=300 /D "WIN32" /D "STRICT" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "WIN32_PLATFORM_PSPC" /D "i486" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_X86_" /D "x86" /D "EXPORTS" /Fp"X86EMDbg/FrenchKISS.pch" /Yu"stdafx.h" /Fo"X86EMDbg/" /Fd"X86EMDbg/" /Gz /c 
"C:\projects\FrenchKISS\PocketPC\3DDisplay.cpp"
"C:\projects\FrenchKISS\PocketPC\audio.cpp"
"C:\projects\FrenchKISS\PocketPC\file.cpp"
"C:\projects\FrenchKISS\PocketPC\FrenchKISS.cpp"
"C:\projects\FrenchKISS\PocketPC\graphic.cpp"
"C:\projects\FrenchKISS\PocketPC\input.cpp"
"C:\projects\FrenchKISS\PocketPC\mathFP.cpp"
"C:\projects\FrenchKISS\PocketPC\mixer.cpp"
"C:\projects\FrenchKISS\PocketPC\resman.cpp"
"C:\projects\FrenchKISS\PocketPC\sys.cpp"
]
Creating command line "cl.exe @C:\DOCUME~1\Alain\LOCALS~1\Temp\RSP137.tmp" 
Creating temporary file "C:\DOCUME~1\Alain\LOCALS~1\Temp\RSP138.tmp" with contents
[
/nologo /W3 /Zi /Od /I "inc" /D "_POCKET_PC" /D "DEBUG" /D _WIN32_WCE=300 /D "WIN32" /D "STRICT" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "WIN32_PLATFORM_PSPC" /D "i486" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_X86_" /D "x86" /D "EXPORTS" /Fp"X86EMDbg/FrenchKISS.pch" /Yc"stdafx.h" /Fo"X86EMDbg/" /Fd"X86EMDbg/" /Gz /c 
"C:\projects\FrenchKISS\PocketPC\StdAfx.cpp"
]
Creating command line "cl.exe @C:\DOCUME~1\Alain\LOCALS~1\Temp\RSP138.tmp" 
Creating temporary file "C:\DOCUME~1\Alain\LOCALS~1\Temp\RSP139.tmp" with contents
[
commctrl.lib coredll.lib corelibc.lib gx.lib /nologo /stack:0x10000,0x1000 /dll /incremental:yes /pdb:"X86EMDbg/FrenchKISS.pdb" /debug /nodefaultlib:"OLDNAMES.lib" /nodefaultlib:libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib /nodefaultlib:oldnames.lib /out:"X86EMDbg/FrenchKISS.dll" /implib:"X86EMDbg/FrenchKISS.lib" /windowsce:emulation /MACHINE:IX86 
.\X86EMDbg\3DDisplay.obj
.\X86EMDbg\audio.obj
.\X86EMDbg\file.obj
.\X86EMDbg\FrenchKISS.obj
.\X86EMDbg\graphic.obj
.\X86EMDbg\input.obj
.\X86EMDbg\mathFP.obj
.\X86EMDbg\mixer.obj
.\X86EMDbg\resman.obj
.\X86EMDbg\StdAfx.obj
.\X86EMDbg\sys.obj
]
Creating command line "link.exe @C:\DOCUME~1\Alain\LOCALS~1\Temp\RSP139.tmp"
<h3>Output Window</h3>
Compiling...
StdAfx.cpp
Compiling...
3DDisplay.cpp
audio.cpp
file.cpp
FrenchKISS.cpp
graphic.cpp
input.cpp
mathFP.cpp
mixer.cpp
resman.cpp
sys.cpp
Generating Code...
Linking...
   Creating library X86EMDbg/FrenchKISS.lib and object X86EMDbg/FrenchKISS.exp
Creating temporary file "C:\DOCUME~1\Alain\LOCALS~1\Temp\RSP13D.bat" with contents
[
@echo off
copy .\X86EMDbg\*.dll .\bin\X86EMDbg
copy .\X86EMDbg\*.lib .\lib\X86EMDbg
]
Creating command line "C:\DOCUME~1\Alain\LOCALS~1\Temp\RSP13D.bat"

.\X86EMDbg\FrenchKISS.dll
        1 file(s) copied.
.\X86EMDbg\FrenchKISS.lib
        1 file(s) copied.



<h3>Results</h3>
FrenchKISS.dll - 0 error(s), 0 warning(s)
</pre>
</body>
</html>
