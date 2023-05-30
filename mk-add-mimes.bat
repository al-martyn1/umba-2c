@call set_2c_names.bat
@if not exist _distr_conf mkdir _distr_conf
@if not exist _distr_conf\conf mkdir _distr_conf\conf
@if not exist %MK_ADD_MIMES% goto FAIL
@%MK_ADD_MIMES% >_distr_conf\conf\umba-2c.options
@goto DONE

:FAIL
@exit /B 1

:DONE
@call copy_distr_conf_to_msvc2019_out.bat
@exit /B 0
