mkdir Debug

nasm Fat12_BS.asm -f bin -o Debug\Fat12_BS.bin
if %errorlevel% neq 0 exit /b %errorlevel%