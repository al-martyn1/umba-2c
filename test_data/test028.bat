2c --filename --text --lf --xor=rnd,rnd,rnd --static         ..\..\doc\_sources_brief.txt ..\tests\_sources_brief_txt.c
2c --filename --text --lf                   --static         ..\..\doc\_sources_brief.txt ..\tests\_sources_brief_txt_noxor.c
@rem 2c --filename --text --lf --xor=rnd,rnd,rnd --static --class ..\..\doc\_sources_brief.txt ..\tests\_sources_brief_txt.cpp

@rem ..\doc\_sources_brief.txt

@rem --no-ext  Remove file extention from autogenerated C-lang var name.
@rem --bin     Input is binary.
@rem --array   Generate output as byte array.
@rem --header  Generate header file (.h) instead of .c.
@rem --static  Generate static data.