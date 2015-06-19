nasm blentry16.asm -f bin -o Debug/blentry.com
if %errorlevel% neq 0 exit /b %errorlevel%

"C:\Program Files (x86)\MSBuild\14.0\Bin\msbuild.exe" /property:Configuration=Debug /property:Platform=Ecma /p:"VCBuildAdditionalOptions=/arch:SSE2

cd Debug
copy /b blentry.com+Loader.exe Stage2.bin
cd ..