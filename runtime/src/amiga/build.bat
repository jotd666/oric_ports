@echo off
cd /D %~PD0
;set opt=-IK:/jff/AmigaHD/amiga39_JFF_OS/include
vasmm68k_mot  -devpac -Faout %opt% -o ..\..\obj\m68k-amigaos\asm_sound_and_gfx.o asm_sound_and_gfx.asm
if %ERRORLEVEL%==1 goto end
echo asm built OK, linking...
del K:\jff\AmigaHD\PROJETS\waydor\waydor
gprbuild -Xhal=native -Xarch=m68k-amigaos -XBuild=Debug -P..\..\..\conversions_done\waydor\waydor.gpr
:end
pause
