/*! \file
    \brief Утилита umba-2c - конвертер файлов в С-массивы
 */

#define UMBA_2RCFS

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
#include "umba/scanners.h"


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
//#include "mimes.h"
#include "base64.h"
#include "encoding/encoding.h"

#include "2c_common/_2c_xor_encrypt.h"

std::string outputFilename;

#include "log.h"
#include "app_config.h"
#include "2c.h"

AppConfig   appConfig;

#include "arg_parser.h"

std::string curFile;
unsigned    lineNo = 0;


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

        argsParser.args.push_back("@..\\test_2rcfs_01.rsp");
        argsParser.args.push_back("--filename-xor=rnd,rnd,rnd");
        argsParser.args.push_back("--input-bin");
        argsParser.args.push_back("--output-string");
        // argsParser.args.push_back("");
        argsParser.args.push_back("--scan=..\\test_data");
        argsParser.args.push_back("../test_res\\2rcfs_01.c");

        // argsParser.args.push_back("--overwrite");
        // argsParser.args.push_back("--scan=..\\test_data");
        // argsParser.args.push_back("--include-files=*.dat^,*.json^,*.txt^");
        // argsParser.args.push_back("../test_res\\2rcfs_01.c");

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

    appConfig = appConfig.adjusted(std::string());


    if (appConfig.includeFilesMaskList.empty() && appConfig.excludeFilesMaskList.empty())
    {
        if (!appConfig.allFiles)
        {
            LOG_ERR_OPT << "no --exclude-files nor --include-files mask are taken, requires --all to confirm processing all files"
                        << "\n";
            return 1;
        }
    }

    // appConfig.scanPaths = inputs;
    std::vector<std::string> foundFiles, excludedFiles;
    std::set<std::string>    foundExtentions;
    //umba::scanners::scanFolders( appConfig, argsParser.quet ? umbaLogStreamNul : umbaLogStreamMsg, foundFiles, excludedFiles, foundExtentions);
    umba::scanners::scanFolders( appConfig, umbaLogStreamNul, foundFiles, excludedFiles, foundExtentions);

    unsigned errCount = 0;

    std::vector< std::basic_regex<char> > binFilesMaskListRegexes  = umba::scanners::fromSimpleMaskToRegexVector(appConfig.binaryFilesMaskList, true /* useAnchoring */, true /* allowRawRegexes */);
    std::vector< std::basic_regex<char> > textFilesMaskListRegexes = umba::scanners::fromSimpleMaskToRegexVector(appConfig.textFilesMaskList  , true /* useAnchoring */, true /* allowRawRegexes */);

    for(auto foundFile : foundFiles)
    {
        std::size_t binMatchIndex  = 0;
        std::size_t textMatchIndex = 0;
        auto normalizedFoundFile = umba::filename::normalizePathSeparators(foundFile,'/');
        bool binMatched  = umba::regex_helpers::regexMatch(normalizedFoundFile, binFilesMaskListRegexes , &binMatchIndex  );
        bool textMatched = umba::regex_helpers::regexMatch(normalizedFoundFile, textFilesMaskListRegexes, &textMatchIndex );

        if (binMatched && textMatched)
        {
            ++errCount;

            LOG_ERR_OPT << umba::formatMessage("'$(fileName)' mathed both bin and text masks. Bin mask matched: '$(binMask)', text mask matched: '$(textMask)'")
                                              .arg("fileName", normalizedFoundFile)
                                              .arg("binMask" , appConfig.binaryFilesMaskList[binMatchIndex ])
                                              .arg("textMask", appConfig.textFilesMaskList  [textMatchIndex])
                                              .toString()
                                              << "\n";
        }
    }

    if (errCount)
    {
        LOG_ERR_OPT << "some error occurs, exiting\n";
        return 1;
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


    os << "/*! \\file\n"
          "    \\brief Generated by umba-2rcfs\n\n"

          "    CUSTOM_UMBA_RCFS_RC_FILENAME(filename, filenameSize) - user defined macro to convert filename array to std::string\n"
          "    CUSTOM_UMBA_RCFS_RC_FILENAME_XORED(filename, filenameSize, filenameXorSize, filenameXorSeed, filenameXorInc) - user defined macro to convert XORed filename array to std::string\n"
          "\n"
          "    CUSTOM_UMBA_RCFS_RC_FILENAME and\n"
          "    CUSTOM_UMBA_RCFS_RC_FILENAME_XORED\n"
          "      are defined in the 2c_common/_2c_xor_encrypt.h if the DISABLE_CUSTOM_UMBA_RCFS_RC_FILENAME_DEFS macro not defined\n"
          "\n"
          "\n"
          "    CUSTOM_UMBA_RCFS_REGISTER_RESOURSE(rcFilename, data, dataSize)\n"
          "    CUSTOM_UMBA_RCFS_REGISTER_RESOURSE_XORED(rcFilename, data, dataSize, xorSize, xorSeed, xorInc)\n"
          "      define this macros before processing this file"
          "\n"
          "\n"
          "*/\n";

    os<<"\n"<<"/*--------------------------------------------------------*/"<<"\n\n";

    for(const auto &fileName : foundFiles)
    {
        curFile = fileName;

        std::string relFileName = umba::filename::makeRelPath( appConfig.scanPaths, fileName );
        if (!argsParser.quet)
        {
            // LOG_MSG_OPT << "Relative name   : '" << relFileName << "'" << "\n";
        }

        auto normalizedRelName = umba::filename::normalizePathSeparators(relFileName,'/');

        auto normalizedFileName = umba::filename::normalizePathSeparators(fileName,'/');
        bool binMatched  = umba::regex_helpers::regexMatch(normalizedFileName, binFilesMaskListRegexes , 0 );
        bool textMatched = umba::regex_helpers::regexMatch(normalizedFileName, textFilesMaskListRegexes, 0 );

        bool binInput = appConfig.binInput;

        std::string binInputMatch ="by default";

        if (binMatched)
        {
            binInput = true;
            binInputMatch = "bin";
        }

        if (textMatched)
        {
            binInput = false;
            binInputMatch = "text";
        }


        bool outputAsString = appConfig.outputAsString;

        if (binInput)
            outputAsString = false;

        if (appConfig.needXorEncryptData())
            outputAsString = false;
        
        if (!argsParser.quet)
        {
            LOG_MSG_OPT << umba::formatMessage("Processing '$(fileName)' - as $(bintext), input match: $(inputMatch), output: $(outputFormat)")
                                              .arg("fileName"     , fileName)
                                              .arg("bintext"      , binInput?"binary":"text")
                                              .arg("inputMatch"   , binInputMatch)
                                              .arg("outputFormat" , outputAsString?"as string":"as array")
                                              .toString()
                        << "\n";
        }
     

        std::string dataReaded;
        try
        {
            dataReaded = readFile(fileName, appConfig, binInput);
        }
        catch(const std::exception &e)
        {
            LOG_ERR_OPT << e.what() << "\n";
            return 2;
        }

        std::string data;
        //bool detectOutputSep = false;
    
        if (binInput)
        {
            data = dataReaded;
        }
        else // text input
        {
            dataReaded = compressTrimText(dataReaded, appConfig, false /* binInput */);
            data       = appConfig.normalizeLinefeeds(dataReaded, false /* binInput */, appConfig.outputLineFeed);
        }

        if (outputAsString)
            appConfig.lineSize = appConfig.getLineSizeText();
        else
            appConfig.lineSize = appConfig.getLineSizeBin();
        

        std::vector< std::string > formattedLines;
        std::string indent = "    ";
       
        size_t dataSize  = data.size(); 
        size_t arraySize = dataSize;
       
        data = appConfig.xorEncryptData(data);

        if (!outputAsString)
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

        auto cname = makeCppName(normalizedRelName);

        std::string staticConst = appConfig.getStaticConst();
        auto values = umba::formatMessage("").arg("scv"             , staticConst.empty() ? std::string() : staticConst+" ")
                                             .arg("arrayType"       , appConfig.getArrayTypeName())
                                             .arg("arraySize"       , arraySize)
                                             .arg("cname"           , cname)
                                             .arg("xorSize"         , (unsigned)appConfig.dataXorEncryptionConfig.xorEncKeySize)
                                             .arg("xorSeed"         , (unsigned)appConfig.dataXorEncryptionConfig.xorEncSeed)
                                             .arg("xorInc"          , (unsigned)appConfig.dataXorEncryptionConfig.xorEncInc)
                                             .arg("filenameXorSize" , (unsigned)appConfig.filenameXorEncryptionConfig.xorEncKeySize)
                                             .arg("filenameXorSeed" , (unsigned)appConfig.filenameXorEncryptionConfig.xorEncSeed)
                                             .arg("filenameXorInc"  , (unsigned)appConfig.filenameXorEncryptionConfig.xorEncInc)
                                             .arg("dataSize"        , dataSize)
                                             .arg("rcFilename"      , normalizedRelName)
                                             .arg("rcFilenameSize"  , normalizedRelName.size())
                                             .arg("nitzComment"     , appConfig.outputAsString ? " /* Not including terminating zero */" : "")
    
                                             .values();
    

        // std::string fileNameFormatted = ( appConfig.needXorEncryptFilename()
        //                                 ? umba::formatMessage("CUSTOM_UMBA_RCFS_RC_FILENAME_XORED(&$(cname)_filename[0], $(rcFilenameSize), $(filenameXorSize), $(filenameXorSeed), $(filenameXorInc))").values(values).toString()
        //                                 : umba::formatMessage("CUSTOM_UMBA_RCFS_RC_FILENAME(&$(cname)_filename[0], $(rcFilenameSize))").values(values).toString()
        //                                 );
        //  
        // os << ( appConfig.needXorEncryptData()
        //       ? umba::formatMessage("CUSTOM_UMBA_RCFS_REGISTER_RESOURSE_XORED($(rcFilename), &$(cname)[0], $(cname)_size, $(xorSize), $(xorSeed), $(xorInc))").values(values).arg("rcFilename",fileNameFormatted).toString()
        //       : umba::formatMessage("CUSTOM_UMBA_RCFS_REGISTER_RESOURSE($(rcFilename), &$(cname)[0], $(cname)_size)").values(values).arg("rcFilename",fileNameFormatted).toString()
        //       ) << "\n";


        if (appConfig.needXorEncryptFilename())
        {
            os << umba::formatMessage("\n$(scv)char* $(cname)_filename[$(rcFilenameSize)] = " ).values(values).toString();
            auto xoredNormalizedRelName = appConfig.xorEncryptFilename(normalizedRelName);
            auto appConfigCopy = appConfig;
            appConfigCopy.lineSize = 4; // 096;
            auto filenameFormattedLines = formatToLinesAsBinary(xoredNormalizedRelName, appConfigCopy);
            bool bFirst = true;
            for( auto line : filenameFormattedLines )
            {
                if (bFirst)
                    os << "{ ";
                else
                    os << ", ";

                os << "" << line << "";

                bFirst = false;
            }

            os << " };\n";

        }
        else
        {
            os << umba::formatMessage("\n$(scv)char* $(cname)_filename = \"$(rcFilename)\";\n" ).values(values).toString();
        }

        os << umba::formatMessage("\n$(scv)unsigned $(cname)_size  = $(dataSize);$(nitzComment)\n" ).values(values).toString();
        os << umba::formatMessage("\n$(scv)$(arrayType) $(cname)[$(arraySize)] =").values(values).toString();
    

        bool bFirst = true;
    
        if (outputAsString && formattedLines.size()==1 /* && (prolog.size()+3+formattedLines[0].size())<= appConfig.lineSize */  )
        {
            os<<"\""<<formattedLines[0]<<"\";\n";
        }
        else if (outputAsString)
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

        std::string fileNameFormatted = ( appConfig.needXorEncryptFilename()
                                        ? umba::formatMessage("CUSTOM_UMBA_RCFS_RC_FILENAME_XORED(&$(cname)_filename[0], $(rcFilenameSize), $(filenameXorSize), $(filenameXorSeed), $(filenameXorInc))").values(values).toString()
                                        : umba::formatMessage("CUSTOM_UMBA_RCFS_RC_FILENAME(&$(cname)_filename[0], $(rcFilenameSize))").values(values).toString()
                                        );

        os << ( appConfig.needXorEncryptData()
              ? umba::formatMessage("CUSTOM_UMBA_RCFS_REGISTER_RESOURSE_XORED($(rcFilename), &$(cname)[0], $(cname)_size, $(xorSize), $(xorSeed), $(xorInc))").values(values).arg("rcFilename",fileNameFormatted).toString()
              : umba::formatMessage("CUSTOM_UMBA_RCFS_REGISTER_RESOURSE($(rcFilename), &$(cname)[0], $(cname)_size)").values(values).arg("rcFilename",fileNameFormatted).toString()
              ) << "\n";

        os<<"\n"<<"/*--------------------------------------------------------*/"<<"\n\n";
    
        /*
        if (appConfig.xorEncKeySize!=_2c::EKeySize::Unknown)
        {
            os << umba::formatMessage("\n$(scv)unsigned $(cname)_xor_size = $(xorSize);\n").values(values).toString();
            os << umba::formatMessage("\n$(scv)unsigned $(cname)_xor_seed = $(xorSeed);\n").values(values).toString();
            os << umba::formatMessage("\n$(scv)unsigned $(cname)_xor_inc  = $(xorInc);\n" ).values(values).toString();
        }
        */
    
    




    }

    return 0;
}

