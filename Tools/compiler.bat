@echo off

set /p input=Podaj nazwe pliku wejsciowego: 
set /p output=Podaj nazwe pliku wyjsciowego: 

luac.exe -o %output% %input%

pause

@echo on