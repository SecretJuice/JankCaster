mingw32-make
if errorLevel 1 goto failure
echo Compiled: Executing...
main
exit
:failure
echo Makefile returned an error.
exit