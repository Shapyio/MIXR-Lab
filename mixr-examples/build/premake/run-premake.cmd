rem
rem delete old solution/workspace files
rem
rmdir /S /Q ..\gmake
rmdir /S /Q ..\vs2015
rmdir /S /Q ..\vs2017
rmdir /S /Q ..\obj
rem
rem delete old compiled libraries
rem
del ..\..\lib\*.lib
rem
rem create solution/workspace files
rem
premake5-alpha12.exe --os=windows --file=premake5.lua vs2015
premake5-alpha12.exe --os=windows --file=premake5.lua vs2017
