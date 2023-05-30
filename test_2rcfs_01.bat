@call set_2c_names.bat
@set LINE_SIZES=--bin-line-size=256 --text-line-size=128

%UMBA2RCFSC% @test_2rcfs_01.rsp --input-bin --output-string --scan=test_data test_res\2rcfs_01.c

