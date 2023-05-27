@set UMBA2C=.out\msvc2019\x64\Debug\umba-2c.exe
@rem set DIFF=meld
@set DIFF=tgmerge


@2c       -h > 2c-help.txt
@%UMBA2C% -h > umba-2c-help.txt

@if not exist test_res mkdir test_res

@2c       --filename --size --lf --static --output-string --line-size=142 test_data\enum_gen_cpp_templates.txt test_res\enum_gen_cpp_templates_txt_c.c
@%UMBA2C% --filename --size --lf --static --output-string --line-size=142 test_data\enum_gen_cpp_templates.txt test_res\enum_gen_cpp_templates_txt_u.c
@rem --output-array

@2c       --base64 --filename --size --lf --static --output-string --line-size=142 test_data\enum_gen_cpp_templates.txt test_res\enum_gen_cpp_templates_txt_b64_c.c
@%UMBA2C% --base64 --filename --size --lf --static --output-string --line-size=142 test_data\enum_gen_cpp_templates.txt test_res\enum_gen_cpp_templates_txt_b64_u.c



@2c       --no-ext --bin --array --static test_data\symbols_map.dat test_res\symbols_map_dat_c.c
@%UMBA2C% --no-ext --bin --array --static test_data\symbols_map.dat test_res\symbols_map_dat_u.c

@2c       --text --string --crlf --static --compress-ws --rtrim --size --filename --header test_data\translations.json test_res\translations.json_c.h
@%UMBA2C% --text --string --crlf --static --compress-ws --rtrim --size --filename --header test_data\translations.json test_res\translations.json_u.h

@2c       --filename --text --lf --xor=rnd,rnd,rnd --static         test_data\_sources_brief.txt test_res\_sources_brief_txt_xor_c.c
@%UMBA2C% --filename --text --lf --xor=rnd,rnd,rnd --static         test_data\_sources_brief.txt test_res\_sources_brief_txt_xor_u.c
@2c       --filename --text --lf                   --static         test_data\_sources_brief.txt test_res\_sources_brief_txt_noxor__c.c
@%UMBA2C% --filename --text --lf                   --static         test_data\_sources_brief.txt test_res\_sources_brief_txt_noxor__u.c

@rem exit /B 1

@start %DIFF% test_res\_sources_brief_txt_xor_c.c          test_res\_sources_brief_txt_xor_u.c
@start %DIFF% test_res\_sources_brief_txt_noxor__c.c       test_res\_sources_brief_txt_noxor__u.c
@start %DIFF% test_res\enum_gen_cpp_templates_txt_c.c      test_res\enum_gen_cpp_templates_txt_u.c
@start %DIFF% test_res\enum_gen_cpp_templates_txt_b64_c.c  test_res\enum_gen_cpp_templates_txt_b64_u.c
@start %DIFF% test_res\symbols_map_dat_c.c                 test_res\symbols_map_dat_u.c
@start %DIFF% test_res\translations.json_c.h               test_res\translations.json_u.h

