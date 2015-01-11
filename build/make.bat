cls

mkdir Debug

cd ..\BootSectors
call make.bat
cd ..\build

copy "..\BootSectors\Debug\Fat12_BS.bin" Debug\Fat12_BS.bin 
if %errorlevel% neq 0 exit /b %errorlevel%

cd ..\Loader
call make.bat
cd ..\build

copy ..\Loader\Debug\Stage2.bin Debug\Stage2.bin
if %errorlevel% neq 0 exit /b %errorlevel%


cd Debug

imagefs c ecmaos.img 2880
imagefs b ecmaos.img Fat12_BS.bin
imagefs a ecmaos.img Stage2.bin

cd ..



IF "%1"=="-debug" GOTO DEBUG

bochs -f bochconfig.txt -q
GOTO END


:DEBUG
bochsdbg -f bochconfigdebug.txt -q


:END