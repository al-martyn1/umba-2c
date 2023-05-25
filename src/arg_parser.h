#pragma once

#include <stack>

//#include "app_config.h"
#include "umba/cmd_line.h"


#if defined(WIN32) || defined(_WIN32)
    #include <shellapi.h>
#endif

// AppConfig    appConfig;


struct ArgParser
{

std::stack<std::string> optFiles;


std::string makeAbsPath( std::string p )
{
    std::string basePath;

    if (optFiles.empty())
        basePath = umba::filesys::getCurrentDirectory<std::string>();
    else
        basePath = umba::filename::getPath(optFiles.top());


    return umba::filename::makeAbsPath( p, basePath );

}



// 0 - ok, 1 normal stop, -1 - error
template<typename ArgsParser>
int operator()( const std::string                               &a           //!< строка - текущий аргумент
              , umba::command_line::CommandLineOption           &opt         //!< Объект-опция, содержит разобранный аргумент и умеет отвечать на некоторые вопросы
              , ArgsParser                                      &argsParser  //!< Класс, который нас вызывает, содержит некоторый контекст
              , umba::command_line::ICommandLineOptionCollector *pCol        //!< Коллектор опций - собирает инфу по всем опциям и готов вывести справку
              , bool fBuiltin
              , bool ignoreInfos
              )
{
    //using namespace marty::clang::helpers;

    std::string dppof = "Don't parse predefined options from ";

    if (opt.isOption())
    {
        std::string errMsg;
        std::string strVal;
        int intVal;
        //unsigned uintVal;
        std::size_t szVal;
        bool boolVal;

        if (opt.name.empty())
        {
            LOG_ERR_OPT<<"invalid (empty) option name\n";
            return -1;
        }

       if (opt.isOption("quet") || opt.isOption('q') || opt.setDescription("Operate quetly"))  // . Short alias for '--verbose=quet'
        {
            argsParser.quet = true;
            //appConfig.setOptQuet(true);
        }

        #if defined(WIN32) || defined(_WIN32)
        else if (opt.isOption("home") || opt.setDescription("Open homepage"))
        {
            if (argsParser.hasHelpOption) return 0;
            ShellExecuteA( 0, "open", appHomeUrl, 0, 0, SW_SHOW );
            return 1;
        }
        #endif

        else if ( opt.isBuiltinsDisableOptionMain  () 
               || opt.setDescription( dppof + "main distribution options file '" + argsParser.getBuiltinsOptFileName(umba::program_location::BuiltinOptionsLocationFlag::appGlobal   ) + "'"))
        { } // simple skip - обработка уже сделана

        else if ( opt.isBuiltinsDisableOptionCustom() 
               || opt.setDescription( dppof + "custom global options file '"     + argsParser.getBuiltinsOptFileName(umba::program_location::BuiltinOptionsLocationFlag::customGlobal) + "'"))
        { } // simple skip - обработка уже сделана

        else if ( opt.isBuiltinsDisableOptionUser  () 
               || opt.setDescription( dppof + "user local options file '"        + argsParser.getBuiltinsOptFileName(umba::program_location::BuiltinOptionsLocationFlag::userLocal   ) + "'"))
        { } // simple skip - обработка уже сделана

        else if (opt.isOption("version") || opt.isOption('v') || opt.setDescription("Show version info"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!ignoreInfos)
            {
                umba::cli_tool_helpers::printOnlyVersion(umbaLogStreamMsg);
                return 1;
            }
        }

        else if (opt.isOption("where") || opt.setDescription("Show where the executable file is"))
        {
            if (argsParser.hasHelpOption) return 0;

            LOG_MSG_OPT << argsParser.programLocationInfo.exeFullName << "\n";
            return 0;
        }

        else if (opt.setParam("CLR", 0, "no/none/file|" 
                                        "ansi/term|" 
                                        #if defined(WIN32) || defined(_WIN32)
                                        "win32/win/windows/cmd/console"
                                        #endif
                             )
              || opt.setInitial(-1) || opt.isOption("color") 
              || opt.setDescription("Force set console output coloring")
              /* ", can be:\nno, none, file - disable coloring\nansi, term - set ansi terminal coloring\nwin32, win, windows, cmd, console - windows console specific coloring method" */
              )
        {
            if (argsParser.hasHelpOption) return 0;

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

        else if ( opt.setParam("?MODE",true)
               || opt.isOption("overwrite") || opt.isOption('Y') 
               // || opt.setParam("VAL",true)
               || opt.setDescription("Allow overwrite existing file."))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR_OPT<<errMsg<<"\n";
                return -1;
            }
            
            appConfig.bOverwrite = boolVal;
            return 0;
        }

        #if 0
        else if ( opt.setParam("LINEFEED",umba::command_line::OptionType::optString)
               || opt.isOption("linefeed") || opt.isOption("LF") || opt.isOption('L')
               // || opt.setParam("VAL",true)
               || opt.setDescription("Output linefeed. LINEFEED is one of: CR/LF/CRLF/LFCR/DETECT. "
                                     #if defined(WIN32) || defined(_WIN32)
                                     "Default is CRLF."
                                     #else
                                     "Default is LF."
                                     #endif
                                    )
                )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR_OPT<<errMsg<<"\n";
                return -1;
            }
            
            ELinefeedType tmp = marty_cpp::enum_deserialize( strVal, ELinefeedType::invalid );
            if (tmp==ELinefeedType::invalid)
            {
                LOG_ERR_OPT<<"Invalid linefeed option value: "<<strVal<<"\n";
                return -1;
            }

            outputLinefeed = tmp;

            return 0;
        }
        #endif

        else if ( opt.isOption("gcc")
               || opt.setDescription("GCC messages format instead of MSVC format")
                )
        {
            if (argsParser.hasHelpOption) return 0;
            
            umbaLogGccFormat = true;

            return 0;
        }

        else if ( opt.isOption("no-ext") || opt.isOption('E') 
               || opt.setDescription("Remove file extention from autogenerated C-lang var name"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.dontKeepExt = true;
        }
        else if (opt.isOption("input-bin") || opt.isOption("bin") || opt.isOption('b') || opt.setDescription("Input is binary"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.binInput = true;
        }
        else if (opt.isOption("input-text") || opt.isOption("text") || opt.isOption('t') || opt.setDescription("Input is text"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.binInput = false;
        }
        else if ( opt.isOption("output-string") || opt.isOption("string") || opt.isOption('s') 
               || opt.setDescription("Generate output as C-string"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.outputAsString = true;
        }
        else if ( opt.isOption("output-array") || opt.isOption("array") || opt.isOption('a') 
               || opt.setDescription("Generate output as byte array"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.outputAsString = false;
        }
        else if ( opt.isOption("size") // || opt.isOption('a') 
               || opt.setDescription("Add resource size variable"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.addResourceFileSize = true;
        }
        else if (opt.isOption("crlf") || opt.setDescription("Use CRLF as line separator"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.outputLineSep = "\r\n";
        }
        else if (opt.isOption("cr") || opt.setDescription("Use CR as line separator"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.outputLineSep = "\r";
        }
        else if (opt.isOption("lf") || opt.setDescription("Use LF as line separator"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.outputLineSep = "\n";
        }
        else if (opt.isOption("lfcr") || opt.setDescription("Use LFCR as line separator"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.outputLineSep = "\n\r";
        }
        else if ( opt.isOption("linefeed-auto") || opt.isOption("auto-linefeed") || opt.isOption("auto") || opt.isOption('A') 
               || opt.setDescription("Use line separator from source file"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.outputLineSep = "auto";
        }
        else if (opt.isOption("no-end-linefeed") || opt.isOption('L') || opt.setDescription("Do not add linefeed at the end of text"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.disableEndLinefeed = true;
        }
        else if (opt.setParam("NAME", "") || opt.isOption("name") || opt.isOption('N') || opt.setDescription("Set C-array/string name"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.hasArg())
            {
                LOG_ERR_OPT<<"name option - value not taken (--name)\n";
                return -1;
            }
            appConfig.cname = opt.optArg;
        }
        else if ( opt.setParam("?FILENAME", "") || opt.isOption("filename") || opt.isOption('F') 
               || opt.setDescription("Add resource file name variable. Parameter value is optional"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (opt.hasArg())
               appConfig.resourceFileName = opt.optArg;
            appConfig.addResourceFileName = true;
        }
        else if ( opt.setParam("?MIMETYPE", "") || opt.isOption("mime-type") || opt.isOption("mime") || opt.isOption('M') 
               || opt.setDescription("Set mime type as taken. If no mime type explicitly taken, it will be autodetected"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (opt.hasArg())
               appConfig.mimeType = opt.optArg;
            appConfig.addMimeType = true;
        }
        else if ( opt.setParam("?DATETIME", "") || opt.isOption("last-modified") || opt.isOption('I')
               || opt.setDescription( "Set last-modified. If not explicitly taken, it will be generated by strftime(\"%a, %d %b %Y %H:%M:%S GMT\").\n"
                                      "Autogenerated datetime is in HTTP/Header/Last-Modified compatible format"
                                    )
                )
        {
            if (argsParser.hasHelpOption) return 0;

            if (opt.hasArg())
               appConfig.lastModified = opt.optArg;
            appConfig.addLastModified = true;
        }
        else if ( opt.setParam(true) || opt.setInitial(false) || opt.isOption("base64") || opt.isOption('B') 
               || opt.setDescription("Encode input file into base64"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(appConfig.base64, errMsg ))
            {
                LOG_ERR_OPT<<errMsg<<"\n";
                return -1;
            }
        }
        //else if (opt.setParam("NAME", "") || opt.isOption("name") || opt.isOption('N') || opt.setDescription("Set C-array/string name"))
        else if ( opt.setParam( "KeySize[,Seed[,Inc]]" /* "OPTIONS" */  /* "KeySize[,Seed[,Inc]]" */, "") || opt.isOption("xor") || opt.isOption('O') 
               || opt.setDescription("Encrypt input file with simple XOR encryption.\nKeySize = 0/1/2/4/NoEncryption/Byte/Short/Int(Unsigned)/Random(Rnd)\nSeed, Inc - start value and optional increment, Random(Rnd) for random value"))
        {
            if (argsParser.hasHelpOption) return 0;

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
            // splitToVector( xorOptionsStr, xorOptionsVec, ',' );
            xorOptionsVec = marty_cpp::splitToLinesSimple(xorOptionsStr, false /* addEmptyLineAfterLastLf */, ',');

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
                appConfig.xorEncKeySize  = _2c::EKeySize::NoEncryption;
                appConfig.xorEncSeed     = 0;
                appConfig.xorEncInc      = 0;
                //appConfig.outputAsString = true; //
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
                appConfig.xorEncSeed = _2c::xorEncryptionSeedFromString(ksz, xorOptionsVec[1], rng, &bValid);
                if (!bValid)
                {
                    LOG_ERR_OPT<<"XOR encryption option - invalid seed value (--xor=KeySize[,Seed[,Inc]])"<<"\n";
                    return -1;
                }

                bValid = false;
                appConfig.xorEncInc = _2c::xorEncryptionSeedFromString(ksz, xorOptionsVec[2], rng, &bValid);
                if (!bValid)
                {
                    LOG_ERR_OPT<<"XOR encryption option - invalid key increment value (--xor=KeySize[,Seed[,Inc]])"<<"\n";
                    return -1;
                }

                appConfig.xorEncKeySize = ksz;

                appConfig.outputAsString = false;
            }

        }
        else if ( opt.setParam("LEN", 0, 0, 100) || opt.setInitial(0) || opt.isOption("base64-len") 
               || opt.setDescription("Set base64 encoded line max len"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(appConfig.base64LineLen, errMsg ))
            {
                LOG_ERR_OPT<<errMsg<<"\n";
                return -1;
            }
        }
        else if ( opt.setParam(true) || opt.setInitial(true) || opt.isOption("base64-filling") 
               || opt.setDescription("Add base64 filling chars at end"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(appConfig.base64Filling, errMsg ))
            {
                LOG_ERR_OPT<<errMsg<<"\n";
                return -1;
            }
        }
        else if (opt.setParam("CLASSNAME","") || opt.isOption("class") || opt.setDescription("* Generate class CLASSNAME for resource"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (opt.hasArg())
                appConfig.className = opt.optArg;
            appConfig.generateClass = true;
        }
        else if (opt.isOption("header") || opt.setDescription("* Generate header file (.h) instead of .c"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.generateHeader = true;
        }
        else if (opt.isOption("class-namespace") || opt.isOption("namespace") || opt.setDescription("* Generate class into namespace(s)"))
        {
            if (argsParser.hasHelpOption) return 0;

            //generateHeader = true;
        }
        else if (opt.isOption("static") || opt.isOption('S') || opt.setDescription("Generate static data"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.staticArray = true;
        }
        else if (opt.isOption("non-const") || opt.isOption('C') || opt.setDescription("Generate non-const data"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.nonConstArray = true;
        }
        else if ( opt.setParam("SIZE", 16, 16, 160) || opt.setInitial(16) || opt.isOption("line-size") || opt.isOption('l') 
               || opt.setDescription("Set max line size or number of array items per line"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue( intVal, errMsg ))
            {
                LOG_ERR_OPT<<errMsg<<"\n";
                return -1;
            }

            appConfig.lineSize = (size_t)intVal;

        }
        else if (opt.isOption("input-encoding") || opt.isOption("input-enc") || opt.setDescription("Set input encoding"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.hasArg())
            {
                LOG_ERR_OPT<<"input-encoding option - value not taken (--input-encoding=ENC)\n";
                return -1;
            }

            appConfig.inputEnc = opt.optArg;
        }
        else if (opt.isOption("target-encoding") || opt.isOption("target-enc") || opt.setDescription("Set target encoding"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.hasArg())
            {
                LOG_ERR_OPT<<"target-encoding option - value not taken (--target-encoding=ENC)\n";
                return -1;
            }

            appConfig.outputEnc = opt.optArg;
        }
        else if (opt.isOption("dec") || opt.isOption('D') || opt.setDescription("Use decimal format for array items (default is hex)"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.decFormat = true;
        }
        else if (opt.isOption("hex") || opt.isOption('X') || opt.setDescription("Use hexadecimal format for array items (default)"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.decFormat = false;
        }
        else if ( opt.isOption("compress-ws") || opt.isOption('R') 
               || opt.setDescription("Compress whitespaces - reduce multiple WS (\\s, \\t) to single"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.compressWhitespaces = true;
        }
        else if (opt.isOption("trim") || opt.isOption("rtrim") || opt.isOption('T') || opt.setDescription("Trim lines at right"))
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.trimLines = true;
        }
        else if (opt.isOption("list-encodings") || opt.setDescription("List of supported encodings"))
        {
            if (argsParser.hasHelpOption) return 0;

            encoding::EncodingsApi* pEncApi = encoding::getEncodingsApi();
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

        else if ( opt.isOption("autocomplete-install") 
               || opt.setDescription("Install autocompletion to bash"
                                     #if defined(WIN32) || defined(_WIN32)
                                         "/clink(cmd)"
                                     #endif
                                    )
               )
        {
            if (argsParser.hasHelpOption) return 0;

            //return autocomplete(opt, true);
            return umba::command_line::autocompletionInstaller( pCol, opt, pCol->getPrintHelpStyle(), true, [&]( bool bErr ) -> decltype(auto) { return bErr ? LOG_ERR_OPT : LOG_MSG_OPT; } );
        }
        else if ( opt.isOption("autocomplete-uninstall") 
               || opt.setDescription("Remove autocompletion from bash"
                                     #if defined(WIN32) || defined(_WIN32)
                                         "/clink(cmd)"
                                     #endif
                                    )
                )
        {
            if (argsParser.hasHelpOption) return 0;

            //return autocomplete(opt, false);
            return umba::command_line::autocompletionInstaller( pCol, opt, pCol->getPrintHelpStyle(), false, [&]( bool bErr ) -> decltype(auto) { return bErr ? LOG_ERR_OPT : LOG_MSG_OPT; } );
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
                    argsParser.quet = true;
                //printNameVersion();
                if (!argsParser.quet)
                {
                    umba::cli_tool_helpers::printNameVersion(umbaLogStreamMsg);
                    umba::cli_tool_helpers::printBuildDateTime(umbaLogStreamMsg);
                    umba::cli_tool_helpers::printCommitHash(umbaLogStreamMsg);
                    std::cout<<"\n";
                //printHelp();
                }

                if (pCol && pCol->isNormalPrintHelpStyle() && argsParser.argsNeedHelp.empty())
                {
                    auto helpText = opt.getHelpOptionsString();
                    std::cout << "Usage: " << argsParser.programLocationInfo.exeName
                              << " [OPTIONS] [output_file]\n"
                              << "  If output_file not taken, STDOUT used\n"
                              << "\nOptions:\n\n"
                              << helpText;
                              //<< " [OPTIONS] input_file [output_file]\n\nOptions:\n\n"<<helpText;

                }
                
                if (pCol) // argsNeedHelp
                    std::cout<<pCol->makeText( 78, &argsParser.argsNeedHelp );

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
        std::string optFileName = makeAbsPath(opt.name);

        optFiles.push(optFileName);

        auto parseRes = argsParser.parseOptionsFile( optFileName );

        optFiles.pop();

        if (!parseRes)
            return -1;

        if (argsParser.mustExit)
            return 1;

        return 0;
    
    }

    if (inputFilename.empty())
    {
        inputFilename = makeAbsPath(a);
    }
    else
    {
        outputFilename = makeAbsPath(a);
    }

    return 0;

}

}; // struct ArgParser



class CommandLineOptionCollector : public umba::command_line::CommandLineOptionCollectorImplBase
{
protected:
    virtual void onOptionDup( const std::string &opt ) override
    {
        LOG_ERR_OPT<<"Duplicated option key - '"<<opt<<"'\n";
        throw std::runtime_error("Duplicated option key");
    }

};



