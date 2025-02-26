@echo off

@set "Png2Dmp=%~dp0/Png2Dmp.exe"

@cd %~dp0/png

@dir *.png /b > png.txt

@for /f "tokens=*" %%m in (png.txt) do (
"%Png2Dmp%" "%%m" --lz77
echo %%m
)

@del png.txt

echo Done!

pause