echo off

echo Building the app...

rem -O3 -- max speed optimization

set CFLAGS=-fdiagnostics-color=always -g -O3 -std=c++11 -Wall
set INCLUDE=.\include
set SRC=.\src\main.cpp .\src\record.cpp .\src\register.cpp .\src\printers.cpp
set TARGET=.\build\main.exe

if not exist .\build mkdir .\build

g++ %CFLAGS% -I %INCLUDE% %SRC% -o %TARGET%

if %ERRORLEVEL%==0 (echo [92mSuccessfully built the app:[0m %TARGET%) else (echo [91mError![0m)
