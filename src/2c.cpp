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
#include "umba/format_message.h"


#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/src_normalization.h"

#include "2c_common/_2c_xor_encrypt.h"


#if defined(WIN32) || defined(_WIN32)
    #include <windows.h>
#endif

#include "app_config.h"
#include "cppHelpers.h"



umba::StdStreamCharWriter coutWriter(std::cout);
umba::StdStreamCharWriter cerrWriter(std::cerr);
umba::NulCharWriter       nulWriter;

umba::SimpleFormatter umbaLogStreamErr(&cerrWriter);
umba::SimpleFormatter umbaLogStreamMsg(&cerrWriter);
umba::SimpleFormatter umbaLogStreamNul(&nulWriter);

bool umbaLogGccFormat   = false; // true;
bool umbaLogSourceInfo  = false;

#include "app_ver_config.h"
#include "mimes.h"
#include "base64.h"
#include "encoding/encoding.h"

#include "2c_common/_2c_xor_encrypt.h"

std::string inputFilename;
std::string outputFilename;

#include "log.h"
#include "app_config.h"
#include "2c.h"

AppConfig   appConfig;

#include "arg_parser.h"



//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{

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

        
        argsParser.args.push_back("-q");
        argsParser.args.push_back("--text");
        argsParser.args.push_back("--string");
        argsParser.args.push_back("--crlf");
        argsParser.args.push_back("--static");
        argsParser.args.push_back("--compress-ws");
        argsParser.args.push_back("--rtrim");
        argsParser.args.push_back("--size");
        argsParser.args.push_back("--filename");
        argsParser.args.push_back("--header");
        argsParser.args.push_back("../test_data\\translations.json");
        argsParser.args.push_back("../test_res\\translations.json_u.h");

        // argsParser.args.push_back("--input-bin");
        // argsParser.args.push_back("../test_data/enum_gen_cpp_templates.txt");
        
        //argsParser.args.push_back("@../umba-tr.rsp");
        //argsParser.args.push_back("../test-tr.json");

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

    if (!argsParser.quet  /* && !hasHelpOption */ )
    {
        //printNameVersion();
        //LOG_MSG_OPT<<"\n";
        umba::cli_tool_helpers::printNameVersion(umbaLogStreamMsg);
    }

    appConfig = appConfig.adjusted(inputFilename);

    if (inputFilename.empty())
    {
        LOG_ERR_OPT<<"No input file taken\n";
        return 1;
    }

    outputFilename = appConfig.getAdustedOutputFilename(inputFilename, outputFilename);

    std::string dataReaded;
    try
    {
        dataReaded = readFile(inputFilename, appConfig);
    }
    catch(const std::exception &e)
    {
        LOG_ERR_OPT << e.what() << "\n";
        return 2;
    }

    if (!appConfig.bOverwrite && umba::filesys::isFileReadable(outputFilename))
    {
        LOG_ERR_OPT << umba::formatMessage("output file '$(filename)' already exist."
                                          ).arg("filename", outputFilename).toString()
                    << "\n"
                    ;
        LOG_ERR_OPT << "Use --overwrite options to force overwrite output file" << "\n";
        return 2;
    }

    std::ofstream os( outputFilename );
    if (!os)
    {
        LOG_ERR_OPT<<"Failed to open output file '"<<outputFilename<<"'\n";
        return 2;
    }


    if (!argsParser.quet)
        LOG_MSG_OPT<<"Processing "<<inputFilename<<"\n";

    
    std::string data;
    bool detectOutputSep = false;

    if (appConfig.binInput)
    {
        data = dataReaded;
    }
    else // text input
    {
        dataReaded = compressTrimText(dataReaded, appConfig);
        data       = appConfig.normalizeLinefeeds(dataReaded);
    }

    size_t orgDataSize = orgDataSize = data.size();

    if (appConfig.base64)
    {
        data = base64_encode( (unsigned char const*)data.data(), (unsigned int)data.size() 
                            , appConfig.base64Filling
                            , (unsigned int)appConfig.base64LineLen
                            , getBase64StandartChars()
                            );
    }

    // есть только данные, лежат в data

    if (appConfig.lineSize==0)
    {
        if (appConfig.outputAsString)
            appConfig.lineSize = 100;
        else
            appConfig.lineSize = 16;
    }


    std::vector< std::string > formattedLines;
    std::string indent = "    ";

    size_t dataSize  = data.size(); 
    size_t arraySize = dataSize;

    data = appConfig.xorEncrypt(data);
    
    if (!appConfig.outputAsString)
    {
        // форматируем в виде массива байт
        formattedLines = formatToLinesAsBinary(data, appConfig);
    }
    else
    {
        // форматируем как строку
        formattedLines = formatToLinesAsEscapedStrings(data, appConfig);
        ++arraySize;
    }

    // отформатировали

    std::string staticConst = appConfig.getStaticConst();
    auto values = umba::formatMessage("").arg("scv"           , staticConst.empty() ? std::string() : staticConst+" ")
                                         .arg("arrayType"     , appConfig.getArrayTypeName())
                                         .arg("arraySize"     , arraySize)
                                         .arg("cname"         , appConfig.cname)
                                         .arg("xorSize"       , (unsigned)appConfig.xorEncKeySize)
                                         .arg("xorSeed"       , (unsigned)appConfig.xorEncSeed)
                                         .arg("xorInc"        , (unsigned)appConfig.xorEncInc)
                                         .arg("orgSize"       , orgDataSize)
                                         .arg("dataSize"      , dataSize)
                                         .arg("rcFilename"    , appConfig.resourceFileName)
                                         .arg("mimeType"      , appConfig.mimeType)
                                         .arg("lastModified"  , appConfig.lastModified)
                                         .arg("nitzComment", appConfig.outputAsString ? " /* Not including terminating zero */" : "")

                                         .values();

    os << umba::formatMessage("$(scv)$(arrayType) $(cname)[$(arraySize)] =").values(values).toString();

    bool bFirst = true;

    if (appConfig.outputAsString && formattedLines.size()==1 /* && (prolog.size()+3+formattedLines[0].size())<= appConfig.lineSize */  )
    {
        os<<"\""<<formattedLines[0]<<"\";\n";
    }
    else if (appConfig.outputAsString)
    {
         for( auto line : formattedLines )
         {
             os << umba::formatMessage("\n$(indent)$(first)\"$(line)\"").arg("indent", indent).arg("first", bFirst?"":"").arg("line", line).toString();
             bFirst = false;
         }

         if (formattedLines.empty())
             os<<"\"\"";

         os << "\n" << indent<<";\n";
    }
    else
    {
         for( auto line : formattedLines )
         {
             os << umba::formatMessage("\n$(indent)$(first)$(line)").arg("indent", indent).arg("first", bFirst?"{ ":", ").arg("line", line).toString();
             bFirst = false;
         }

         os<<"\n"<<indent<<"};\n\n";
    }


    if (appConfig.xorEncKeySize!=_2c::EKeySize::Unknown)
    {
        os << umba::formatMessage("\n$(scv)unsigned $(cname)_xor_size = $(xorSize);\n").values(values).toString();
        os << umba::formatMessage("\n$(scv)unsigned $(cname)_xor_seed = $(xorSeed);\n").values(values).toString();
        os << umba::formatMessage("\n$(scv)unsigned $(cname)_xor_inc  = $(xorInc);\n" ).values(values).toString();
    }

    if (appConfig.base64)
    {
        os << umba::formatMessage("\n$(scv)unsigned $(cname)_orgsize = $(orgSize);\n" ).values(values).toString();
    }

    if (appConfig.addResourceFileSize)
    {
        os << umba::formatMessage("\n$(scv)unsigned $(cname)_size = $(dataSize);$(nitzComment)\n" ).values(values).toString();
    }

    if (appConfig.addResourceFileName)
    {
        os << umba::formatMessage("\n$(scv)char* $(cname)_filename = \"$(rcFilename)\";\n" ).values(values).toString();
    }

    if (appConfig.addMimeType)
    {
        os << umba::formatMessage("\n$(scv)char* $(cname)_mime_type = \"$(mimeType)\";\n" ).values(values).toString();
    }

    if (appConfig.addLastModified)
    {
        os << umba::formatMessage("\n$(scv)char* $(cname)_last_modified = \"$(lastModified)\";\n" ).values(values).toString();
    }

    
    return 0;
}

