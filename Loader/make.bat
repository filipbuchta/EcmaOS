nasm blentry16.asm -f bin -o Debug/blentry.com
if %errorlevel% neq 0 exit /b %errorlevel%

"C:\Program Files (x86)\MSBuild\14.0\Bin\msbuild.exe"

cd Debug
copy /b blentry.com+Loader.exe Stage2.bin
cd ..