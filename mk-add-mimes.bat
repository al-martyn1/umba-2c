@set MK_ADD_MIMES=.out\msvc2019\x64\Debug\mk-add-mime.exe
@if not exist _distr_conf mkdir _distr_conf
@if not exist _distr_conf\conf mkdir _distr_conf\conf
@if not exist %MK_ADD_MIMES% goto FAIL
@%MK_ADD_MIMES% >_distr_conf\conf\umba-2c.options
@goto DONE

:FAIL
@exit /B 1

:DONE
@exit /B 0
