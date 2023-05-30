#ifndef UMBA_2RCFS
std::string appFullName   = "Umba 2c";
#else
std::string appFullName   = "Umba 2rcfs";
#endif

std::string appVersion    = "1.04";
std::string appCommitHash;  //UNDONE
std::string appBuildDate  = __DATE__;
std::string appBuildTime  = __TIME__;

const char *appHomeUrl    = "https://github.com/al-martyn1/umba-2c";
const char *appistrPath   = "";

#if defined(WIN32) || defined(_WIN32)
    #ifndef UMBA_2RCFS
    const char *appSubPath    = "bin/umba-2c.exe";
    #else
    const char *appSubPath    = "bin/umba-2rcfs.exe";
    #endif
#else
    #ifndef UMBA_2RCFS
    const char *appSubPath    = "bin/umba-2c";
    #else
    const char *appSubPath    = "bin/umba-2rcfs";
    #endif
#endif
