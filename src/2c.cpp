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

AppConfig   appConfig;

#include "arg_parser.h"



inline
int absDelta( int i1, int i2 )
{
    int res = i1 - i2;
    return res>0 ? res : -res;
}

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

    if (!argsParser.quet  /* && !hasHelpOption */ )
    {
        //printNameVersion();
        //LOG_MSG_OPT<<"\n";
        umba::cli_tool_helpers::printNameVersion(umbaLogStreamMsg);
    }


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


    appConfig = appConfig.adjusted(inputFilename);


    //[X]
    // if (appConfig.cname.empty())
    // {
    //     if (!appConfig.dontKeepExt)
    //         appConfig.cname = makeCppName(umba::filename::getFileName(inputFilename));
    //     else
    //         appConfig.cname = makeCppName(umba::filename::getName(inputFilename));
    // }
    //  
    // if (!appConfig.cname.empty())
    // {
    //     if (appConfig.cname[0]>='0' && appConfig.cname[0]<='9')
    //         appConfig.cname = std::string("_") + appConfig.cname;
    // }



    // [X]
    // if (appConfig.addResourceFileName)
    // {
    //     if (appConfig.resourceFileName.empty())
    //     {
    //         appConfig.resourceFileName = umba::filename::getFileName(inputFilename);
    //     }
    // }

    // [X]
    // if (appConfig.addMimeType)
    // {
    //     if (appConfig.mimeType.empty())
    //     {
    //         //NOTE: !!! А оно надо, toLower?
    //         //std::string srcExt = toLower(umba::filename::getFileExtention(inputFilename));
    //         // std::string srcExt = umba::filename::getFileExtention(inputFilename);
    //         std::string srcExt = marty_cpp::toLower(umba::filename::getFileExtention(inputFilename));
    //         if (!srcExt.empty() && srcExt[0]!='.')
    //            srcExt = std::string(".") + srcExt;
    //  
    //         const std::map< std::string, std::string >& mimes = getMimeTypes();
    //         std::map< std::string, std::string >::const_iterator mit = mimes.find(srcExt);
    //         if (mit==mimes.end())
    //             appConfig.mimeType = "application/octet-stream";
    //         else
    //             appConfig.mimeType = mit->second;
    //     }
    // }

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


    if (!argsParser.quet)
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

        if (!appConfig.binInput)
        {
            if (appConfig.outputEnc.empty())
                appConfig.outputEnc = "UTF-8";

            encoding::EncodingsApi* pEncApi = encoding::getEncodingsApi();
           
            if (appConfig.inputEnc.empty())
            {
                size_t bomSize = 0;
                appConfig.inputEnc = pEncApi->detect( dataReaded, bomSize, std::string() /* httpHint */ , std::string() /* metaHint */  );
            }
           
            if (appConfig.inputEnc.empty())
            {
                LOG_ERR_OPT<<"Failed to detect input encoding\n";
                return 2;
            }
           
            if (!pEncApi->isEqualEncodingNames(appConfig.inputEnc, appConfig.outputEnc))
            {
            }
           
            UINT cpSrc = pEncApi->getCodePageByName(appConfig.inputEnc );
            if (!cpSrc)
            {
                LOG_ERR_OPT<<"Unknown or unsupported input file encoding - "<< appConfig.inputEnc<<"\n";
                return 2;
            }
           
            UINT cpDst = pEncApi->getCodePageByName(appConfig.outputEnc );
            if (!cpDst)
            {
                LOG_ERR_OPT<<"Unknown or unsupported output encoding - "<< appConfig.outputEnc<<"\n";
                return 2;
            }
           
            dataReaded = pEncApi->convert(dataReaded, cpSrc, cpDst );
        }
    }

    //std::vector< std::string > inputLines;
    char splitBy = 0;
    bool detectOutputSep = false;

    if (appConfig.binInput)
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
        if (appConfig.compressWhitespaces)
        {
            dataReaded = textCompress(dataReaded, " \t");
            appConfig.trimLines = true;
        }

        if (appConfig.trimLines)
        {
            dataReaded = marty_cpp::stripTextTrailingSpaces(dataReaded);
        }

        std::vector< std::string > lines;
        std::string inputSep;
        textSplitToLines(dataReaded, lines, inputSep);
        if (appConfig.trimLines)
        {
            //marty_cpp::stripLineTrailingSpaces(lines);
            //rtrim(lines);
        }
        if (appConfig.outputLineSep.empty() || appConfig.outputLineSep=="auto")
            appConfig.outputLineSep = inputSep;

        splitBy = appConfig.outputLineSep.back();

        size_t li = 0, lsz = lines.size();
        for( ; li!=lsz; ++li )
        {
            auto l = lines[li];
            std::string resLine = l;
            if ((li+1)!=lsz)
            {
                resLine += appConfig.outputLineSep;
            }
            else if (!appConfig.disableEndLinefeed)
            {
                resLine += appConfig.outputLineSep;
            }
            // 
            //inputLines.push_back( resLine );
            data += resLine;
        }

        // есть данные, есть построчное разбиение входного текста (как артефакт компрессии, трима и замены перевода строк)
        // строки - локально, но можно вынести за скобки

    } // if (!binInput)

    size_t orgDataSize = 0;

    if (appConfig.base64)
    {
        orgDataSize = data.size();
        //std::string
        data = base64_encode( (unsigned char const*)data.data(), (unsigned int)data.size() 
                            , appConfig.base64Filling
                            , (unsigned int)appConfig.base64LineLen
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

    if (appConfig.lineSize==0)
    {
        if (appConfig.outputAsString)
            appConfig.lineSize = 100;
        else
            appConfig.lineSize = 16;
    }


    size_t inputDataPos = 0, inputDataSize = data.size();
    std::string curFormattedLine;
    std::vector< std::string > formattedLines;
    
    size_t numOfLineItems = 0;

    std::string indent = "    ";

    size_t dataSize = 0;


    if (appConfig.xorEncKeySize!=_2c::EKeySize::Unknown)
    {
        _2c::xorEncrypt(data.begin(), data.end(), appConfig.xorEncKeySize, appConfig.xorEncSeed, appConfig.xorEncInc);
    }

    
    if (!appConfig.outputAsString)
    {
        // форматируем в виде массива байт

        dataSize = inputDataSize;

        for( ; inputDataPos!=inputDataSize; ++inputDataPos)
        {
            std::string chStr = cppHelpersFormatUnsigned( (std::uint8_t)data[inputDataPos], appConfig.decFormat?10:16, true, appConfig.decFormat?3:2, appConfig.decFormat?' ':'0' );
            if (!appConfig.decFormat)
               chStr = std::string("0x") + chStr;

            if ((numOfLineItems+1)== appConfig.lineSize) // текущая строка после добавления элемента достигнет лимита
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

            if (len> appConfig.lineSize  /* && ch!=splitBy */   /* '\r' && l[i]!='\n' */ )
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

    std::string prolog = appConfig.outputAsString ? "char" : "unsigned char", epilog;

    if (!appConfig.nonConstArray)
       staticConst = std::string("const ") + staticConst;
    if (appConfig.staticArray)
       staticConst = std::string("static ") + staticConst;

    prolog = staticConst + prolog;

    prolog += std::string(" ") + appConfig.cname + std::string("[") + cppHelpersFormatUnsigned( (unsigned)(dataSize), 10, false, 0 ) + std::string("] = ") ;


    os<<prolog;

    if (appConfig.outputAsString && formattedLines.size()==1 && (prolog.size()+3+formattedLines[0].size())<= appConfig.lineSize )
    {
        os<<"\""<<formattedLines[0]<<"\";\n";
    }
    else if (appConfig.outputAsString)
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


    if (appConfig.xorEncKeySize!=_2c::EKeySize::Unknown)
    {
        os<<"\n"<<staticConst<<"unsigned "<< appConfig.cname<<"_xor_size = "<<(unsigned)appConfig.xorEncKeySize<<";\n";
        os<<"\n"<<staticConst<<"unsigned "<< appConfig.cname<<"_xor_seed = "<<(unsigned)appConfig.xorEncSeed<<";\n";
        os<<"\n"<<staticConst<<"unsigned "<< appConfig.cname<<"_xor_inc  = "<<(unsigned)appConfig.xorEncInc<<";\n";
    }

    if (appConfig.base64)
    {
        os<<"\n";
        os<<staticConst<<"char* ";
        os<< appConfig.cname<<"_orgsize = \""<<orgDataSize<<"\";\n";
    }

    if (appConfig.addResourceFileSize)
    {
        unsigned rcSize = dataSize;

        os<<"\n";
        if (appConfig.outputAsString)
            --rcSize;

        os<<staticConst<<"unsigned ";
        os<< appConfig.cname<<"_size = "<<cppHelpersFormatUnsigned( (unsigned)(rcSize), 10, false, 0 )<<";";

        if (appConfig.outputAsString)
            os<<" /* Not including terminating zero */";

        os<<"\n";
    }

    if (appConfig.addResourceFileName)
    {
        os<<"\n";
        os<<staticConst<<"char* ";
        os<< appConfig.cname<<"_filename = \""<< appConfig.resourceFileName<<"\";\n";
    }

    if (appConfig.addMimeType)
    {
        os<<"\n";
        os<<staticConst<<"char* ";
        os<< appConfig.cname<<"_mime_type = \""<< appConfig.mimeType<<"\";\n";
    }

    // [X]
    if (appConfig.addLastModified)
    {
        // if (appConfig.lastModified.empty())
        // {
        //     struct stat t_stat;
        //     stat(inputFilename.c_str(), &t_stat);
        //     struct tm * timeinfo = gmtime(&t_stat.st_mtime); // or st_mtim? st_ctime or localtime() depending on what you want
        //  
        //     // asctime - Www Mmm dd hh:mm:ss yyyy\n
        //     // http    - <day-name>, <day> <month> <year> <hour>:<minute>:<second> GMT
        //     // https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Last-Modified
        //     // https://en.cppreference.com/w/cpp/chrono/c/strftime
        //    
        //     char buf[256];
        //     std::size_t sz = strftime( buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", timeinfo );
        //     buf[sz] = 0;
        //  
        //     appConfig.lastModified = buf;
        // }

        os<<"\n";
        os<<staticConst<<"char* ";
        os<< appConfig.cname<<"_last_modified = \""<< appConfig.lastModified<<"\";\n";
    }

    
    return 0;
}

