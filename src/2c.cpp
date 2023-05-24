/*! \file
    \brief Утилита umba-2c - конвертер файлов в С-массивы
 */

// Должна быть первой
#include "umba/umba.h"
//---

//#-sort
#include "umba/simple_formatter.h"
#include "umba/char_writers.h"
//#+sort



#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <ios>
#include <iomanip>

#include <sys/stat.h>
#include <time.h>
#include <stdio.h>

#include "textUtils.h"

#include "umba/filename.h"
#include "umba/filesys.h"
#include "umba/debug_helpers.h"
#include "umba/cli_tool_helpers.h"


#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/src_normalization.h"


#if defined(WIN32) || defined(_WIN32)
    #include <windows.h>
#endif

#include "cppHelpers.h"



umba::StdStreamCharWriter coutWriter(std::cout);
umba::StdStreamCharWriter cerrWriter(std::cerr);
umba::NulCharWriter       nulWriter;

umba::SimpleFormatter umbaLogStreamErr(&cerrWriter);
umba::SimpleFormatter umbaLogStreamMsg(&cerrWriter);
umba::SimpleFormatter umbaLogStreamNul(&nulWriter);

bool umbaLogGccFormat   = false; // true;
bool umbaLogSourceInfo  = false;


// std::string exeFullName = "2c";
// std::string progBinPath     = ".";
// std::string progRootPath    = ".";
//std::string progConfPath    = ".\\conf" ;
//std::string progIncludePath = ".\\include";


//LogOptions   logOptions;


// #include "logerr.h"
//  
//  
// #include "rdlc-core/trims.h"
// #include "rdlc-core/splits.h"
// #include "rdlc-core/cpp.h"
// #include "rdlc-core/case.h"
// #include "rdlc-core/isa.h"
// #include "rdlc-core/utils.h"
// #include "rdlc-core/fsUtils.h"
// #include "rdlc-core/prog.h"
// #include "rdlc-core/cli_impl.h"
// #include "rdlc-core/textUtils.h"

#include "app_ver_config.h"
// #include "_2cver.h"
#include "mimes.h"
#include "base64.h"
#include "encoding/encoding.h"


#include "2c_common/_2c_xor_encrypt.h"

std::string inputFilename;
std::string outputFilename;

#include "log.h"
// #include "print_help.h"


bool                    quet           = false;
//std::set<std::string>   argsNeedHelp;
//bool                    hasHelpOption  = false;

bool                    bOverwrite = false;


bool disableEndLinefeed = false;
bool dontKeepExt = false;
bool binInput = false;
std::string outputLineSep;
std::string cname;

bool outputAsString = false;
size_t lineSize = 0;
bool staticArray = false;
bool nonConstArray = false;
bool decFormat  = false;
bool compressWhitespaces = false;
bool trimLines = false;

std::string inputEnc;
std::string outputEnc = "UTF-8";

bool addMimeType     = false;
std::string mimeType = "";

bool addLastModified     = false;
std::string lastModified = "";

bool addResourceFileName = false;
std::string resourceFileName;

bool addResourceFileSize = false;

bool generateClass = false;
std::string className;

std::vector< std::string > classNamespaces;

bool generateHeader= false;

bool base64 = false;
int  base64LineLen = 0; // no break
bool base64Filling = false;

_2c::EKeySize xorEncKeySize = _2c::EKeySize::Unknown;
unsigned      xorEncSeed    = 0;
unsigned      xorEncInc     = 0;


#include "arg_parser.h"



inline
int absDelta( int i1, int i2 )
{
    int res = i1 - i2;
    return res>0 ? res : -res;
}

//-----------------------------------------------------------------------------
#if 0
// 0 - ok, 1 normal stop, -1 - error
int parseArg( std::string a, ICommandLineOptionCollector *pCol, bool fBuiltin, bool ignoreInfos)
{

    trim(a);

    if (a.empty())
    {
        LOG_ERR_OPT<<"invalid (empty) argument\n";
        return -1;
    }

    CommandLineOption opt(a, pCol);

    pCol->setCollectMode( opt.isHelpOption() );

    if (opt.isOption())
    {
        std::string errMsg;
        int intVal;
      
        if (opt.name.empty())
        {
            LOG_ERR_OPT<<"invalid (empty) option name\n";
            return -1;
        }

       if (opt.isOption("quet") || opt.isOption('q') || opt.setDescription("Operate quetly"))
        {
            quet = true;
        }
        #if defined(WIN32) || defined(_WIN32)
        else if (opt.isOption("home") || opt.isOption('H') || opt.setDescription("Open homepage"))
        {
            if (hasHelpOption) return 0;

            ShellExecuteA( 0, "open", _2cHomeUrl, 0, 0, SW_SHOW );
            return 1;
        }
        #endif
        else if (opt.isOption("version") || opt.isOption('v') || opt.setDescription("Show version info"))
        {
            if (hasHelpOption) return 0;

            if (!ignoreInfos)
            {
                printOnlyVersion();
                return 1;
            }
        }
        /*
        else if (opt.infoIgnore() || opt.isOption("credits") || opt.setDescription(""))
        {
            if (!fBuiltin)
            {
                printNameVersion();
                // LOG_MSG_OPT<<"RDL Language Design         : Alexander Martynov amart@mail.ru\n";
                // LOG_MSG_OPT<<"RDLC Compiler Implementation: Alexander Martynov amart@mail.ru\n";
                // LOG_MSG_OPT<<"Jokes                       : Alexander Martynov amart@mail.ru\n";
                // LOG_MSG_OPT<<"                            : Roman Hazansky\n";
                return 1;
            }
            return 0; // simple skip then parse builtins
        }
        */
        else if (opt.isOption("build-info") || opt.setDescription("Show extended info about version/build"))
        {
            if (hasHelpOption) return 0;

            if (!ignoreInfos)
            {
                std::string indent;
                printNameVersion(indent);
                printBuildDateTime(indent);
                printCommitHash(indent);
                return 1;
            }
        }
        else if (opt.isOption("!build-info-x") || opt.setDescription("Show extended info about version/build"))
        {
            if (hasHelpOption) return 0;

            if (!ignoreInfos)
            {
                std::string indent = "  ";
                printNameVersion(indent);
                printBuildDateTime(indent);
                printCommitHash(indent);
                return 1;
            }
        }
        else if (opt.isOption("where") || opt.setDescription("Show where the executable file is"))
        {
            if (hasHelpOption) return 0;

            LOG_MSG_OPT<<exeFullName<<"\n";
            return 0;
        }
        /*
        else if (opt.isOption("dont-warn") || opt.isOption('W') || opt.setParam("WARN_TYPE") || opt.setDescription("Disable warnings of WARN_TYPE"))
        {
            if (!opt.hasArg())
            {
                LOG_ERR_OPT<<"warning type(s) not taken (--dont-warn)\n";
                return -1;
            }
             
            std::vector<std::string> warnList;
            splitToVector(opt.optArg, warnList, ',' );
            for( auto &warn : warnList )
                logOptions.disabledWarnings.insert(warn);
        }
        */
        
        else if ( opt.isOption("no-ext") || opt.isOption('E') 
               || opt.setDescription("Remove file extention from autogenerated C-lang var name"))
        {
            if (hasHelpOption) return 0;

            dontKeepExt = true;
        }
        else if (opt.isOption("input-bin") || opt.isOption("bin") || opt.isOption('b') || opt.setDescription("Input is binary"))
        {
            if (hasHelpOption) return 0;

            binInput = true;
        }
        else if (opt.isOption("input-text") || opt.isOption("text") || opt.isOption('t') || opt.setDescription("Input is text"))
        {
            if (hasHelpOption) return 0;

            binInput = false;
        }
        else if ( opt.isOption("output-string") || opt.isOption("string") || opt.isOption('s') 
               || opt.setDescription("Generate output as C-string"))
        {
            if (hasHelpOption) return 0;

            outputAsString = true;
        }
        else if ( opt.isOption("output-array") || opt.isOption("array") || opt.isOption('a') 
               || opt.setDescription("Generate output as byte array"))
        {
            if (hasHelpOption) return 0;

            outputAsString = false;
        }
        else if ( opt.isOption("size") // || opt.isOption('a') 
               || opt.setDescription("Add resource size variable"))
        {
            if (hasHelpOption) return 0;

            addResourceFileSize = true;
        }
        else if (opt.isOption("crlf") || opt.setDescription("Use CRLF as line separator"))
        {
            if (hasHelpOption) return 0;

            outputLineSep = "\r\n";
        }
        else if (opt.isOption("cr") || opt.setDescription("Use CR as line separator"))
        {
            if (hasHelpOption) return 0;

            outputLineSep = "\r";
        }
        else if (opt.isOption("lf") || opt.setDescription("Use LF as line separator"))
        {
            if (hasHelpOption) return 0;

            outputLineSep = "\n";
        }
        else if (opt.isOption("lfcr") || opt.setDescription("Use LFCR as line separator"))
        {
            if (hasHelpOption) return 0;

            outputLineSep = "\n\r";
        }
        else if ( opt.isOption("linefeed-auto") || opt.isOption("auto-linefeed") || opt.isOption("auto") || opt.isOption('A') 
               || opt.setDescription("Use line separator from source file"))
        {
            if (hasHelpOption) return 0;

            outputLineSep = "auto";
        }
        else if (opt.isOption("no-end-linefeed") || opt.isOption('L') || opt.setDescription("Do not add linefeed at the end of text"))
        {
            if (hasHelpOption) return 0;

            disableEndLinefeed = true;
        }
        else if (opt.setParam("NAME", "") || opt.isOption("name") || opt.isOption('N') || opt.setDescription("Set C-array/string name"))
        {
            if (hasHelpOption) return 0;

            if (!opt.hasArg())
            {
                LOG_ERR_OPT<<"name option - value not taken (--name)\n";
                return -1;
            }
            cname = opt.optArg;
        }
        else if ( opt.setParam("?FILENAME", "") || opt.isOption("filename") || opt.isOption('F') 
               || opt.setDescription("Add resource file name variable. Parameter value is optional"))
        {
            if (hasHelpOption) return 0;

            if (opt.hasArg())
               resourceFileName = opt.optArg;
            addResourceFileName = true;
        }
        else if ( opt.setParam("?MIMETYPE", "") || opt.isOption("mime-type") || opt.isOption("mime") || opt.isOption('M') 
               || opt.setDescription("Set mime type as taken. If no mime type explicitly taken, it will be autodetected"))
        {
            if (hasHelpOption) return 0;

            if (opt.hasArg())
               mimeType = opt.optArg;
            addMimeType = true;
        }
        else if (opt.setParam("?DATETIME", "") || opt.isOption("last-modified") || opt.isOption('I') || opt.setDescription("Set last-modified"))
        {
            if (hasHelpOption) return 0;

            if (opt.hasArg())
               lastModified = opt.optArg;
            addLastModified = true;
        }
        else if ( opt.setParam(true) || opt.setInitial(false) || opt.isOption("base64") || opt.isOption('B') 
               || opt.setDescription("Encode input file into base64"))
        {
            if (hasHelpOption) return 0;

            if (!opt.getParamValue( base64, errMsg ))
            {
                LOG_ERR_OPT<<errMsg<<"\n";
                return -1;
            }
        }
        //else if (opt.setParam("NAME", "") || opt.isOption("name") || opt.isOption('N') || opt.setDescription("Set C-array/string name"))
        else if ( opt.setParam( "KeySize[,Seed[,Inc]]" /* "OPTIONS" */  /* "KeySize[,Seed[,Inc]]" */, "") || opt.isOption("xor") || opt.isOption('O') 
               || opt.setDescription("Encrypt input file with simple XOR encryption.\nKeySize = 0/1/2/4/NoEncryption/Byte/Short/Int(Unsigned)/Random(Rnd)\nSeed, Inc - start value and optional increment, Random(Rnd) for random value"))
        {
            if (hasHelpOption) return 0;

            // if (!opt.getParamValue( base64, errMsg ))
            // {
            //     LOG_ERR_OPT<<errMsg<<"\n";
            //     return -1;
            // }

            std::string xorOptionsStr;

            if (!opt.hasArg() || opt.optArg.empty())
            {
                LOG_ERR_OPT<<"Missing '--xor' option argument"<<"\n";
                return -1;
            }

            xorOptionsStr = opt.optArg;

            std::vector<std::string> xorOptionsVec;
            splitToVector( xorOptionsStr, xorOptionsVec, ',' );

            if (xorOptionsVec.size()<1)
            {
                LOG_ERR_OPT<<"XOR encryption option - invalid option format (--xor=KeySize[,Seed[,Inc]])"<<"\n";
                return -1;
            }

            // _2c::EKeySize ks = _2c::enum_deserialize(xorOptionsVec[0], _2c::EKeySize::Unknown);
            std::mt19937 rng = _2c::xorEncryptionGetRandomGenerator();

            _2c::EKeySize ksz = _2c::xorEncryptionKeySizeFromString(xorOptionsVec[0], rng);
            if (ksz==_2c::EKeySize::Unknown)
            {
                LOG_ERR_OPT<<"XOR encryption option - invalid KeySize value (--xor=KeySize[,Seed[,Inc]])"<<"\n";
                return -1;
            }

            std::size_t keySize = _2c::xorEncryptionKeySize(ksz);
            if (keySize==0)
            {
                // No XOR encryption, reset 
                xorEncKeySize  = _2c::EKeySize::NoEncryption;
                xorEncSeed     = 0;
                xorEncInc      = 0;
                outputAsString = true;
            }
            else
            {
                if (xorOptionsVec.size()<2)
                {
                    LOG_ERR_OPT<<"XOR encryption option - invalid option format (--xor=KeySize[,Seed[,Inc]])"<<"\n";
                    return -1;
                }

                if (xorOptionsVec.size()<3)
                    xorOptionsVec.push_back("0");

                bool bValid = false;
                xorEncSeed = _2c::xorEncryptionSeedFromString(ksz, xorOptionsVec[1], rng, &bValid);
                if (!bValid)
                {
                    LOG_ERR_OPT<<"XOR encryption option - invalid seed value (--xor=KeySize[,Seed[,Inc]])"<<"\n";
                    return -1;
                }

                bValid = false;
                xorEncInc = _2c::xorEncryptionSeedFromString(ksz, xorOptionsVec[2], rng, &bValid);
                if (!bValid)
                {
                    LOG_ERR_OPT<<"XOR encryption option - invalid key increment value (--xor=KeySize[,Seed[,Inc]])"<<"\n";
                    return -1;
                }

                xorEncKeySize = ksz;

                outputAsString = false;
            }

        }
        else if ( opt.setParam("LEN", 0, 0, 100) || opt.setInitial(0) || opt.isOption("base64-len") 
               || opt.setDescription("Set base64 encoded line max len"))
        {
            if (hasHelpOption) return 0;

            if (!opt.getParamValue( base64LineLen, errMsg ))
            {
                LOG_ERR_OPT<<errMsg<<"\n";
                return -1;
            }
        }
        else if ( opt.setParam(true) || opt.setInitial(true) || opt.isOption("base64-filling") 
               || opt.setDescription("Add base64 filling chars at end"))
        {
            if (hasHelpOption) return 0;

            if (!opt.getParamValue( base64Filling, errMsg ))
            {
                LOG_ERR_OPT<<errMsg<<"\n";
                return -1;
            }
        }
        else if (opt.setParam("CLASSNAME","") || opt.isOption("class") || opt.setDescription("* Generate class CLASSNAME for resource"))
        {
            if (hasHelpOption) return 0;

            if (opt.hasArg())
               className = opt.optArg;
            generateClass = true;
        }
        else if (opt.isOption("header") || opt.setDescription("* Generate header file (.h) instead of .c"))
        {
            if (hasHelpOption) return 0;

            generateHeader = true;
        }
        else if (opt.isOption("class-namespace") || opt.setDescription("* Generate class into namespace(s)"))
        {
            if (hasHelpOption) return 0;

            //generateHeader = true;
        }
        else if (opt.isOption("static") || opt.isOption('S') || opt.setDescription("Generate static data"))
        {
            if (hasHelpOption) return 0;

            staticArray = true;
        }
        else if (opt.isOption("non-const") || opt.isOption('C') || opt.setDescription("Generate non-const data"))
        {
            if (hasHelpOption) return 0;

            nonConstArray = true;
        }
        else if ( opt.setParam("SIZE", 16, 16, 160) || opt.setInitial(16) || opt.isOption("line-size") || opt.isOption('l') 
               || opt.setDescription("Set max line size or number of array items per line"))
        {
            if (hasHelpOption) return 0;

            if (!opt.getParamValue( intVal, errMsg ))
            {
                LOG_ERR_OPT<<errMsg<<"\n";
                return -1;
            }

            lineSize = (size_t)intVal;

        }
        else if (opt.isOption("input-encoding") || opt.isOption("input-enc") || opt.setDescription("Set input encoding"))
        {
            if (hasHelpOption) return 0;

            if (!opt.hasArg())
            {
                LOG_ERR_OPT<<"input-encoding option - value not taken (--input-encoding=ENC)\n";
                return -1;
            }

            inputEnc = opt.optArg;
        }
        else if (opt.isOption("target-encoding") || opt.isOption("target-enc") || opt.setDescription("Set target encoding"))
        {
            if (hasHelpOption) return 0;

            if (!opt.hasArg())
            {
                LOG_ERR_OPT<<"target-encoding option - value not taken (--target-encoding=ENC)\n";
                return -1;
            }

            outputEnc = opt.optArg;
        }
        else if (opt.isOption("dec") || opt.isOption('D') || opt.setDescription("Use decimal format for array items (default is hex)"))
        {
            if (hasHelpOption) return 0;

            decFormat = true;
        }
        else if (opt.isOption("hex") || opt.isOption('X') || opt.setDescription("Use hexadecimal format for array items (default)"))
        {
            if (hasHelpOption) return 0;

            decFormat = false;
        }
        else if ( opt.isOption("compress-ws") || opt.isOption('R') 
               || opt.setDescription("Compress whitespaces - reduce multiple WS (\\s, \\t) to single"))
        {
            if (hasHelpOption) return 0;

            compressWhitespaces = true;
        }
        else if (opt.isOption("trim") || opt.isOption("rtrim") || opt.isOption('T') || opt.setDescription("Trim lines at right"))
        {
            if (hasHelpOption) return 0;

            trimLines = true;
        }
        else if (opt.isOption("list-encodings") || opt.setDescription("List of supported encodings"))
        {
            if (hasHelpOption) return 0;

            EncodingsApi* pEncApi = getEncodingsApi();
            size_t encIdx = 0;
            std::string name, info;

            std::cout<<"Supported encodings:\n";

            while(pEncApi->getEncodingInfo(encIdx++, name, info))
            {
                std::cout<<name;
                if (name.size()<18)
                {
                    std::cout<<std::string( 18-name.size(), ' ' );
                }
                std::cout<<" - "<<info<<"\n";
            }

            return 1;

        }
        else if (opt.setParam("CLR", 0, "no/none/file|" 
                                        "ansi/term|" 
                                        #if defined(WIN32) || defined(_WIN32)
                                        "win32/win/windows/cmd/console"
                                        #endif
                             )
              || opt.setInitial(-1) || opt.isOption("console-colors") 
              || opt.setDescription("Force set console output coloring")
              /* ", can be:\nno, none, file - disable coloring\nansi, term - set ansi terminal coloring\nwin32, win, windows, cmd, console - windows console specific coloring method" */
              )
        {
            if (hasHelpOption) return 0;

            umba::term::ConsoleType res;
            auto mapper = [](int i) -> umba::term::ConsoleType
                          {
                              switch(i)
                              {
                                  case 0 : return umba::term::ConsoleType::file;
                                  case 1 : return umba::term::ConsoleType::ansi_terminal;
                                  case 2 : return umba::term::ConsoleType::windows_console;
                                  default: return umba::term::ConsoleType::file;
                              };
                          };
            if (!opt.getParamValue( res, errMsg, mapper ) )
            {
                LOG_ERR_OPT<<errMsg<<"\n";
                return -1;
            }

            coutWriter.forceSetConsoleType(res);
            cerrWriter.forceSetConsoleType(res);
        }
        /*
        else if (opt.isOption("!wiki") || opt.setDescription("Help output in Wiki format"))
        {
            printHelpStyle = PrintHelpStyle::wiki;
        }
        else if (opt.isOption("!bash") || opt.setDescription("Help output in 'bash complete' format"))
        {
            printHelpStyle = PrintHelpStyle::bash_complete;
        }
        else if (opt.isOption("!clink") || opt.setDescription("Help output in 'clink complete' format"))
        {
            printHelpStyle = PrintHelpStyle::clink_complete;
        }
        */
        else if (opt.isOption("autocomplete-install") || opt.setDescription("Install autocompletion to bash/clink(cmd)"))
        {
            if (hasHelpOption) return 0;

            //return autocomplete(opt, true);
            return autocompletionInstaller( pCol, opt, pCol->getPrintHelpStyle(), true, [&]( bool bErr ) -> decltype(auto) { return bErr ? LOG_ERR_OPT : LOG_MSG_OPT; } );
        }
        else if (opt.isOption("autocomplete-uninstall") || opt.setDescription("Remove autocompletion from bash/clink(cmd)"))
        {
            if (hasHelpOption) return 0;

            //return autocomplete(opt, false);
            return autocompletionInstaller( pCol, opt, pCol->getPrintHelpStyle(), false, [&]( bool bErr ) -> decltype(auto) { return bErr ? LOG_ERR_OPT : LOG_MSG_OPT; } );
        }
        else if (opt.isHelpStyleOption())
        {
            // Job is done in isHelpStyleOption
        }
        else if (opt.isHelpOption()) // if (opt.infoIgnore() || opt.isOption("help") || opt.isOption('h') || opt.isOption('?') || opt.setDescription(""))
        {
            if (!ignoreInfos)
            {
                if (pCol && !pCol->isNormalPrintHelpStyle())
                    quet = true;
                //printNameVersion();
                if (!quet)
                {
                    printBuildDateTime();
                    printCommitHash();
                    std::cout<<"\n";
                //printHelp();
                }

                if (pCol && pCol->isNormalPrintHelpStyle() && argsNeedHelp.empty())
                    std::cout<<"Usage: 2c [OPTIONS] input_file [output_file]\n\nOptions:\n\n"<<opt.getHelpOptionsString();
                
                if (pCol) // argsNeedHelp
                    std::cout<<pCol->makeText( 64, &argsNeedHelp );

                return 1;
            }
            return 0; // simple skip then parse builtins
        }
        else
        {
            LOG_ERR_OPT<<"unknown option: "<<opt.argOrg<<"\n";
            return -1;
        }

        return 0;

    } // if (opt.isOption())
    else if (opt.isResponseFile())
    {
        if (hasHelpOption) return 0;

        std::vector<std::string> opts;
        if (!readOptionsFile(opt.name, opts))
        {
            LOG_ERR_OPT<<"failed to read response file '"<<opt.name<<"'\n";
            return -1;
        }

        for ( const auto & optLine : opts )
        {
            int paRes = parseArg( optLine, pCol, false, true );
            if (paRes)
            {
               return paRes;
            }
        }

        #if 0
        std::ifstream optFile(opt.name.c_str());
        if (!optFile)
        {
            LOG_ERR_OPT<<"failed to read response file '"<<opt.name<<"'\n";
            return -1;
        }
        else
        {
            std::string optLine;
            while( std::getline( optFile, optLine) )
            {
                trim(optLine);
                if (optLine.empty())
                    continue;

                if (isComment( optLine ))
                    continue;

                int paRes = parseArg( optLine, pCol, false, true );
                if (paRes)
                {
                   return paRes;
                }
            }
        }
        #endif
        return 0;
    
    }

    
    if (inputFilename.empty())
        inputFilename = a;
    else
        outputFilename = a;

    return 0;

}


class CommandLineOptionCollectorImpl : public CommandLineOptionCollectorImplBase
{
protected:
    virtual void onOptionDup( const std::string &opt ) override
    {
        LOG_ERR_OPT<<"Duplicated option key - '"<<opt<<"'\n";
        throw std::runtime_error("Duplicated option key");
    }

};

#endif


int main(int argc, char* argv[])
{
    #if 0
    using std::cin;
    using std::cout;
    using std::cerr;

    //cout<<textAddIndent( prepareTextParaMakeString( "Very long long para with multiple something stranges repeated multiple times. Very long long para with multiple something stranges repeated multiple times. Very long long para with multiple something stranges repeated multiple times. Very long long para with multiple something stranges repeated multiple times. Very long long para with multiple something stranges repeated multiple times", 48 ), "    ");
    //return 0;

    CommandLineOptionCollectorImpl commandLineOptionCollector;

    detectProgLocation( exeFullName, progBinPath, progRootPath );

    std::vector<std::string> args = prepareArgs( argc, &argv[0] );
    hasHelpOption = argsFilterForHelpOption( args, argsNeedHelp /* , { "wiki", "bash", "clink" } */ );

    for( const auto & a : args)
    {
        CommandLineOption opt(a);
     
        if (!opt.isOption())
            continue;

        // if (opt.isOption("no-builtin-options"))
        //     disableBuiltins = true;
        // else if (opt.isOption("no-builtin-includes"))
        //     disableBuiltinIncludes = true;
        // else 
        //if (opt.isOption("quet") || opt.isOption('q') || opt.isOption("version") || opt.isOption('v') || opt.isOption("build-info") || opt.isOption("build-info-x"))
        if (opt.isSomeKindOfQuet() && !hasHelpOption)
            quet = true;
        else if (opt.isWhereOption())
        {
            LOG_MSG_OPT<<exeFullName<<"\n";
            return 0;
        }
    }
    #endif


    auto argsParser = umba::command_line::makeArgsParser( ArgParser()
                                                        , CommandLineOptionCollector()
                                                        , argc, argv
                                                        , umba::program_location::getProgramLocation
                                                            ( argc, argv
                                                            , false // useUserFolder = false
                                                            //, "" // overrideExeName
                                                            )
                                                        );

    // Force set CLI arguments while running under debugger
    if (umba::isDebuggerPresent())
    {
        argsParser.args.clear();
        argsParser.args.push_back("@../umba-tr.rsp");
        argsParser.args.push_back("../test-tr.json");

        //argsParser.args.push_back("@..\\make_sources_brief.rsp");
        // argsParser.args.push_back(umba::string_plus::make_string(""));
        // argsParser.args.push_back(umba::string_plus::make_string(""));
        // argsParser.args.push_back(umba::string_plus::make_string(""));
    }

    //programLocationInfo = argsParser.programLocationInfo;

    // try
    // {
        // Job completed - may be, --where option found
        if (argsParser.mustExit)
            return 0;
       
        // if (!argsParser.quet)
        // {
        //     printNameVersion();
        // }
       
        if (!argsParser.parseStdBuiltins())
            return 1;
        if (argsParser.mustExit)
            return 0;
       
        if (!argsParser.parse())
            return 1;
        if (argsParser.mustExit)
            return 0;
    // }
    // catch(const std::exception &e)
    // {
    //     LOG_ERR_OPT << e.what() << "\n";
    //     return -1;
    // }
    // catch(const std::exception &e)
    // {
    //     LOG_ERR_OPT << "command line arguments parsing error" << "\n";
    //     return -1;
    // }


    //if (hasHelpOption)

    if (!quet  /* && !hasHelpOption */ )
    {
        //printNameVersion();
        //LOG_MSG_OPT<<"\n";
        umba::cli_tool_helpers::printNameVersion(umbaLogStreamMsg);
    }

    #if 0
    if (!disableBuiltins)
    {
        std::string rdlcOptionsFileName = progConfPath + std::string("\\rdlc.options");
        std::ifstream optFile(rdlcOptionsFileName.c_str());
        if (!!optFile)
        {
            std::string optLine;
            while( std::getline( optFile, optLine) )
            {
                trim(optLine);
                if (optLine.empty())
                    continue;

                if (isComment( optLine ))
                    continue;

                int paRes = parseArg( optLine, 0 /* &commandLineOptionCollector */ , false, true );
                if (paRes)
                {
                   LOG_ERR_OPT<<"Error in options file '"<<rdlcOptionsFileName<<"'\n";
                   return paRes<0 ? 1 : 0;
                }
            }

            optFile.close();
        }

        rdlcOptionsFileName = progConfPath + std::string("\\rdlc.user");
        optFile.open( rdlcOptionsFileName.c_str() );
        if (!!optFile)
        {
            std::string optLine;
            while( std::getline( optFile, optLine) )
            {
                trim(optLine);
                if (optLine.empty())
                    continue;

                if (isComment( optLine ))
                    continue;

                int paRes = parseArg( optLine, 0 /* &commandLineOptionCollector */ , false, true );
                if (paRes)
                {
                   LOG_ERR_OPT<<"Error in options file '"<<rdlcOptionsFileName<<"'\n";
                   return paRes<0 ? 1 : 0;
                }
            }

            optFile.close();
        }
    }

    for( const auto & a : args)
    {
        int paRes = parseArg( a, &commandLineOptionCollector, false, false );
        if (paRes)
           return paRes<0 ? 1 : 0;
    }

    #endif


    if (inputFilename.empty())
    {
        LOG_ERR_OPT<<"No input file taken\n";
        return 1;
    }

    if (outputFilename.empty())
    {
        //cerr<<"No output file name taken\n";
        //return 1;
        outputFilename = umba::filename::getPathFile( inputFilename );
        //defaultOutputUsed = true;
        std::string ext = umba::filename::getFileExtention( inputFilename );
        if (!ext.empty())
            outputFilename += std::string("_") + ext;
    }

    std::string curOutputExt = umba::filename::getFileExtention( outputFilename );
    // toLower(curOutputExt); //NOTE: !!! А оно надо, toLower?

    if (curOutputExt.empty() || curOutputExt==".")
    {
        outputFilename = umba::filename::appendExtention( outputFilename, std::string("c") );
    }

    if (cname.empty())
    {
        if (!dontKeepExt)
           cname = makeCppName(umba::filename::getFileName(inputFilename));
        else
           cname = makeCppName(umba::filename::getName(inputFilename));
    }

    if (!cname.empty())
    {
        if (cname[0]>='0' && cname[0]<='9')
            cname = std::string("_") + cname;
    }

    if (addResourceFileName)
    {
        if (resourceFileName.empty())
        {
            resourceFileName = umba::filename::getFileName(inputFilename);
        }
    }

    if (addMimeType)
    {
        if (mimeType.empty())
        {
            //NOTE: !!! А оно надо, toLower?
            //std::string srcExt = toLower(umba::filename::getFileExtention(inputFilename));
            // std::string srcExt = umba::filename::getFileExtention(inputFilename);
            std::string srcExt = marty_cpp::toLower(umba::filename::getFileExtention(inputFilename));
            if (!srcExt.empty() && srcExt[0]!='.')
               srcExt = std::string(".") + srcExt;

            const std::map< std::string, std::string >& mimes = getMimeTypes();
            std::map< std::string, std::string >::const_iterator mit = mimes.find(srcExt);
            if (mit==mimes.end())
               mimeType = "application/octet-stream";
            else
               mimeType = mit->second;
        }
    }

    std::ifstream input( inputFilename, std::ios_base::in | std::ios_base::binary );
    if (!input)
    {
        LOG_ERR_OPT<<"Failed to open input file '"<<inputFilename<<"'\n";
        return 2;
    }

    std::ofstream os( outputFilename );
    if (!os)
    {
        LOG_ERR_OPT<<"Failed to open output file '"<<outputFilename<<"'\n";
        return 2;
    }


    if (!quet)
        LOG_MSG_OPT<<"Processing "<<inputFilename<<"\n";

    std::string dataReaded;
    std::string data;

    {
        {
            std::vector<char> fileDataVec;
            umba::filesys::readFile(static_cast<std::istream&>(input), fileDataVec);
            dataReaded = std::string(fileDataVec.data(), fileDataVec.size());
        }
        // readBinaryFile( input, dataReaded );

        if (!binInput)
        {
            if (outputEnc.empty())
                outputEnc = "UTF-8";

            encoding::EncodingsApi* pEncApi = encoding::getEncodingsApi();
           
            if (inputEnc.empty())
            {
                size_t bomSize = 0;
                inputEnc = pEncApi->detect( dataReaded, bomSize, std::string() /* httpHint */ , std::string() /* metaHint */  );
            }
           
            if (inputEnc.empty())
            {
                LOG_ERR_OPT<<"Failed to detect input encoding\n";
                return 2;
            }
           
            if (!pEncApi->isEqualEncodingNames(inputEnc, outputEnc))
            {
            }
           
            UINT cpSrc = pEncApi->getCodePageByName( inputEnc );
            if (!cpSrc)
            {
                LOG_ERR_OPT<<"Unknown or unsupported input file encoding - "<<inputEnc<<"\n";
                return 2;
            }
           
            UINT cpDst = pEncApi->getCodePageByName( outputEnc );
            if (!cpDst)
            {
                LOG_ERR_OPT<<"Unknown or unsupported output encoding - "<<outputEnc<<"\n";
                return 2;
            }
           
            dataReaded = pEncApi->convert(dataReaded, cpSrc, cpDst );
        }
    }

    //std::vector< std::string > inputLines;
    char splitBy = 0;
    bool detectOutputSep = false;

    if (binInput)
    {
        data = dataReaded;

        //splitBy
        //std::string sep = textDetectLinefeed(data);
        //splitBy = sep.back();
        splitBy = textDetectLinefeed(data).back();

        // есть данные, нет построчного разбиения входного текста
    }
    else // text input
    {
        if (compressWhitespaces)
        {
            dataReaded = textCompress(dataReaded, " \t");
            trimLines = true;
        }

        if (trimLines)
        {
            dataReaded = marty_cpp::stripTextTrailingSpaces(dataReaded);
        }

        std::vector< std::string > lines;
        std::string inputSep;
        textSplitToLines(dataReaded, lines, inputSep);
        if (trimLines)
        {
            //marty_cpp::stripLineTrailingSpaces(lines);
            //rtrim(lines);
        }
        if (outputLineSep.empty() || outputLineSep=="auto")
            outputLineSep = inputSep;

        splitBy = outputLineSep.back();

        size_t li = 0, lsz = lines.size();
        for( ; li!=lsz; ++li )
        {
            auto l = lines[li];
            std::string resLine = l;
            if ((li+1)!=lsz)
            {
                resLine += outputLineSep;
            }
            else if (!disableEndLinefeed)
            {
                resLine += outputLineSep;
            }
            // 
            //inputLines.push_back( resLine );
            data += resLine;
        }

        // есть данные, есть построчное разбиение входного текста (как артефакт компрессии, трима и замены перевода строк)
        // строки - локально, но можно вынести за скобки

    } // if (!binInput)

    size_t orgDataSize = 0;

    if (base64)
    {
        orgDataSize = data.size();
        //std::string
        data = base64_encode( (unsigned char const*)data.data(), (unsigned int)data.size() 
                            , base64Filling
                            , (unsigned int)base64LineLen
                            , getBase64StandartChars()
                            );
        splitBy = '\n'; // base64 always use CRLF

        //inputLines.clear();

        /*
        if (outputAsString)
        {
            {
                std::vector< std::string > lines;
                splitToVector( data, lines, splitBy );
                trim(lines, "\r\n");
                if (trimLines)
                   rtrim(lines);

                data.clear();
               
                size_t li = 0, lsz = lines.size();
                for( ; li!=lsz; ++li )
                {
                    auto l = lines[li];
                    std::string resLine = l;
                    if ((li+1)!=lsz)
                    {
                        resLine += "\r\n"; // outputLineSep;
                    }
                    else if (!disableEndLinefeed)
                    {
                        resLine += "\r\n"; // outputLineSep;
                    }
                    // 
                    inputLines.push_back( resLine );
                    data += resLine;
                }

            }
        }
        */
    }

    // есть только данные, лежат в data

    if (lineSize==0)
    {
        if (outputAsString)
            lineSize = 100;
        else
            lineSize = 16;
    }


    size_t inputDataPos = 0, inputDataSize = data.size();
    std::string curFormattedLine;
    std::vector< std::string > formattedLines;
    
    size_t numOfLineItems = 0;

    std::string indent = "    ";

    size_t dataSize = 0;


    if (xorEncKeySize!=_2c::EKeySize::Unknown)
    {
        _2c::xorEncrypt(data.begin(), data.end(), xorEncKeySize, xorEncSeed, xorEncInc);
    }

    
    if (!outputAsString)
    {
        // форматируем в виде массива байт

        dataSize = inputDataSize;

        for( ; inputDataPos!=inputDataSize; ++inputDataPos)
        {
            std::string chStr = cppHelpersFormatUnsigned( (std::uint8_t)data[inputDataPos], decFormat?10:16, true, decFormat?3:2, decFormat?' ':'0' );
            if (!decFormat)
               chStr = std::string("0x") + chStr;

            if ((numOfLineItems+1)==lineSize) // текущая строка после добавления элемента достигнет лимита
            {

                curFormattedLine.append(chStr);
                formattedLines.push_back(curFormattedLine);
                curFormattedLine.clear();
                numOfLineItems = 0;
            }
            else
            {
                curFormattedLine.append(chStr + std::string(", "));
                ++numOfLineItems;
            }
        }

        if (!curFormattedLine.empty())
            formattedLines.push_back(curFormattedLine);

        if (!formattedLines.empty())
        {
            // Удаляем лишние пробелы и запятые в конце последней строки
            //rtrim(formattedLines[formattedLines.size()-1], " ,");
            std::string &lastLine = formattedLines.back();
            auto i=lastLine.size();
            for(; i!=0; --i)
            {
                char lastChar = lastLine[i-1];
                if (lastChar==' ' || lastChar==',')
                    continue;
                break;
            }
            lastLine.erase(i, lastLine.npos);
        }

    }
    else // форматируем как строку
    {
        dataSize = 0;

        for( ; inputDataPos!=inputDataSize; ++inputDataPos)
        {
            char ch = data[inputDataPos];
            ++dataSize;

            std::string chStr = marty_cpp::cEscapeString( std::string(1,ch) );

            size_t len = indent.size() + 2 + curFormattedLine.size() + chStr.size();

            if (len>lineSize  /* && ch!=splitBy */   /* '\r' && l[i]!='\n' */ )
            {
                // Строка получается слишком длинной, обрываем её, не дожидаясь появления в тексте перевода строки
                formattedLines.push_back(curFormattedLine);
                curFormattedLine = chStr;
            }
            else if ( ch==splitBy )
            {
                // Обрываем строку по символу перевода строки, его добавляем в обрываемую строку
                curFormattedLine.append(chStr);
                formattedLines.push_back(curFormattedLine);
                curFormattedLine.clear();
            }
            else
            {
                // Просто добавляем к текущей строке
                curFormattedLine.append(chStr);
            }
        }

        if (!curFormattedLine.empty())
           formattedLines.push_back(curFormattedLine);

        ++dataSize; // terminating  zero

    }

    // отформатировали

    std::string staticConst;

    std::string prolog = outputAsString ? "char" : "unsigned char", epilog;

    if (!nonConstArray)
       staticConst = std::string("const ") + staticConst;
    if (staticArray)
       staticConst = std::string("static ") + staticConst;

    prolog = staticConst + prolog;

    prolog += std::string(" ") + cname + std::string("[") + cppHelpersFormatUnsigned( (unsigned)(dataSize), 10, false, 0 ) + std::string("] = ") ;


    os<<prolog;

    if (outputAsString && formattedLines.size()==1 && (prolog.size()+3+formattedLines[0].size())<=lineSize )
    {
        os<<"\""<<formattedLines[0]<<"\";\n";
    }
    else if (outputAsString)
    {
         bool bFirst = true;
         for( auto line : formattedLines )
         {
             if (!bFirst)
                os<<"";
             else
                bFirst = false;

             os<<"\n"<<indent<<"\""<<line<<"\"";
         }

         if (formattedLines.empty())
             os<<"\"\"";

         os<<"\n"<<indent<<";\n";
    }
    else
    {
         os<<"{";

         bool bFirst = true;
         for( auto line : formattedLines )
         {
             os<<"\n"<<indent;
             if (!bFirst)
             {
                os<<", ";
             }
             else
             {
                os<<"  ";
                bFirst = false;
             }

             os<<line;
         }

         os<<"\n"<<indent<<"};\n\n";
    }


    if (xorEncKeySize!=_2c::EKeySize::Unknown)
    {
        os<<"\n"<<staticConst<<"unsigned "<<cname<<"_xor_size = "<<(unsigned)xorEncKeySize<<";\n";
        os<<"\n"<<staticConst<<"unsigned "<<cname<<"_xor_seed = "<<(unsigned)xorEncSeed<<";\n";
        os<<"\n"<<staticConst<<"unsigned "<<cname<<"_xor_inc  = "<<(unsigned)xorEncInc<<";\n";
    }

    if (base64)
    {
        os<<"\n";
        os<<staticConst<<"char* ";
        os<<cname<<"_orgsize = \""<<orgDataSize<<"\";\n";
    }

    if (addResourceFileSize)
    {
        unsigned rcSize = dataSize;

        os<<"\n";
        if (outputAsString)
            --rcSize;

        os<<staticConst<<"unsigned ";
        os<<cname<<"_size = "<<cppHelpersFormatUnsigned( (unsigned)(rcSize), 10, false, 0 )<<";";

        if (outputAsString)
            os<<" /* Not including terminating zero */";

        os<<"\n";
    }

    if (addResourceFileName)
    {
        os<<"\n";
        os<<staticConst<<"char* ";
        os<<cname<<"_filename = \""<<resourceFileName<<"\";\n";
    }

    if (addMimeType)
    {
        os<<"\n";
        os<<staticConst<<"char* ";
        os<<cname<<"_mime_type = \""<<mimeType<<"\";\n";
    }

    if (addLastModified)
    {
        if (lastModified.empty())
        {
            struct stat t_stat;
            stat(inputFilename.c_str(), &t_stat);
            struct tm * timeinfo = gmtime(&t_stat.st_mtime); // or st_mtim? st_ctime or localtime() depending on what you want

            // asctime - Www Mmm dd hh:mm:ss yyyy\n
            // http    - <day-name>, <day> <month> <year> <hour>:<minute>:<second> GMT
            // https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Last-Modified
            // https://en.cppreference.com/w/cpp/chrono/c/strftime
           
            char buf[256];
            std::size_t sz = strftime( buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", timeinfo );
            buf[sz] = 0;

            lastModified = buf;
        }

        os<<"\n";
        os<<staticConst<<"char* ";
        os<<cname<<"_last_modified = \""<<lastModified<<"\";\n";
    }
    
            
            
    //if (!quet)
    //    LOG_MSG_OPT<<"Processing "<<inputFilename<<"\n";


    return 0;
   }

