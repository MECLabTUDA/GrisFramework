echo off

set APP_NAME=%1
set MODULE_NAME=%2
set BUILD_DIR=%3
set PATH_DEPLOY=%4
set SOURCE_DIR=%5


if not exist "%PATH_DEPLOY%" mkdir "%PATH_DEPLOY%"

call :app_copy %BUILD_DIR%\src\%MODULE_NAME%\%APP_NAME%\RelWithDebInfo\%APP_NAME% %PATH_DEPLOY%

call :dll_copy %BUILD_DIR%\src\gstd\RelWithDebInfo\gstd %PATH_DEPLOY%

goto:EOF

:app_copy
	xcopy "%1.exe" "%2" /S /D /Y
	xcopy "%1.pdb" "%2" /S /D /Y	
	goto:EOF

:dll_copy
	xcopy "%1.dll" "%2" /S /D /Y
	xcopy "%1.pdb" "%2" /S /D /Y
	goto:EOF

:single_dll_copy
	xcopy "%1.dll" "%2" /S /D /Y
	goto:EOF

:itk_copy
	xcopy "%1-%ITK_VER%.dll" "%2" /S /D /Y
	goto:EOF

:vtk_copy
	xcopy "%1-%VTK_VER%.dll" "%2" /S /D /Y
	goto:EOF


pause