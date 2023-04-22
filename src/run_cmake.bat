call ..\..\..\SetEnv.bat
set
pause

rd /s /q ..\.settings
rd /s /q ..\build
rd /s /q ..\CMakeFiles
rd /s /q ..\generated
rd /s /q ..\pico-sdk
rd /s /q ..\pioasm
rd /s /q ..\elf2uf2

del /s /q ..\.cproject
del /s /q ..\.project
del /s /q ..\Makefile
del /s /q ..\cmake_install.cmake
del /s /q ..\*.elf
del /s /q ..\*.uf2
del /s /q ..\CMakeCache.txt

del /s /q ..\*ninja*.*
del /s /q ..\*.map
del /s /q ..\*.ninja
del /s /q ..\*.bin
del /s /q ..\*.dis
del /s /q ..\*.hex



pause


cd ..

cmake -S src -B . -G "Eclipse CDT4 - Ninja" -D CMAKE_BUILD_TYPE=Debug
pause


