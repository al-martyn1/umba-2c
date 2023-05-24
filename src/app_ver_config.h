std::string appFullName   = "Umba 2c";
std::string appVersion    = "1.03";
std::string appCommitHash;  //UNDONE
std::string appBuildDate  = __DATE__;
std::string appBuildTime  = __TIME__;

const char *appHomeUrl    = "https://github.com/al-martyn1/umba-2c";
const char *appistrPath   = "";

#if defined(WIN32) || defined(_WIN32)
    const char *appSubPath    = "bin/umba-2c.exe";
#else
    const char *appSubPath    = "bin/umba-2c";
#endif
