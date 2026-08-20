@rem xcopy /Y /S /E /I /F /R _distr_conf\conf\* .out\msvc2019\x64\conf
@rem xcopy /Y /S /E /I /F /R _distr_conf\conf\* .out\msvc2019\x86\conf

@rem if exist "%~dp0\.set_sln.bat" @call "%~dp0\.set_sln.bat"
@rem call "%~dp0\.umba.bat\_start_msvc_impl.bat" %SLN%
