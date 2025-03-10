@echo off
setlocal enabledelayedexpansion

echo ========================================================
echo         Installing KiCad Dependencies via vcpkg
echo ========================================================

REM Set vcpkg path
set VCPKG_ROOT=D:\git\vcpkg
if not exist "%VCPKG_ROOT%" (
    echo ERROR: vcpkg directory not found at %VCPKG_ROOT%
    echo Please install vcpkg or edit this script with the correct path.
    exit /b 1
)

REM Get the KiCad source directory (where this script is located)
set KICAD_SOURCE_DIR=%~dp0
echo KiCad source directory: %KICAD_SOURCE_DIR%

cd %VCPKG_ROOT%
echo Installing KiCad dependencies from vcpkg.json...

REM Install dependencies from the vcpkg.json manifest file
%VCPKG_ROOT%\vcpkg.exe install --triplet=x64-windows --x-manifest-root=%KICAD_SOURCE_DIR%  


echo.
echo Dependencies installed successfully!
echo You can now run build_kicad.bat to build KiCad
echo ========================================================

endlocal 