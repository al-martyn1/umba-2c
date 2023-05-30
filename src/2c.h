#pragma once

#include "app_config.h"
#include "encoding/encoding.h"
#include "umba/format_message.h"
#include "textUtils.h"
#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/src_normalization.h"
#include "textUtils.h"

#include <string>
#include <exception>
#include <stdexcept>


//----------------------------------------------------------------------------
inline
std::string textAppend(std::string text, const std::string &strAppend)
{
    if (!text.empty() && !strAppend.empty())
    {
        text.append(1, ' ');
    }

    text.append(strAppend);

    return text;
}

//----------------------------------------------------------------------------
inline
std::string convertTextEncoding(const std::string &text, const AppConfig &appCfg, bool binInput)
{
    if (binInput)
    {
        return text;
    }

    encoding::EncodingsApi* pEncApi = encoding::getEncodingsApi();
   
    std::string inputEnc = appCfg.inputEnc;

    if (inputEnc.empty())
    {
        size_t bomSize = 0;
        inputEnc = pEncApi->detect( text, bomSize, std::string() /* httpHint */ , std::string() /* metaHint */  );
        if (inputEnc.empty())
        {
            throw std::runtime_error( umba::formatMessage("Failed to detect input encoding").toString());
        }
    }
   
    UINT cpSrc = pEncApi->getCodePageByName(inputEnc);
    if (!cpSrc)
    {
        throw std::runtime_error( umba::formatMessage("Unknown or unsupported input file encoding: '$(enc)'").arg("enc", inputEnc).toString());
        // LOG_ERR_OPT<<"Unknown or unsupported input file encoding - "<< appCfg.inputEnc<<"\n";
        // return 2;
    }
   
    UINT cpDst = pEncApi->getCodePageByName(appCfg.outputEnc);
    if (!cpDst)
    {
        throw std::runtime_error( umba::formatMessage("Unknown or unsupported output encoding: '$(enc)'").arg("enc", appCfg.outputEnc).toString());
    }
   
    // if (cpSrc==cpDst)
    //     return text;

    return pEncApi->convert(text, cpSrc, cpDst );
}

//----------------------------------------------------------------------------
inline
std::string convertTextEncoding(const std::string &text, const AppConfig &appCfg)
{
    return convertTextEncoding(text, appCfg, appCfg.binInput);
}

//----------------------------------------------------------------------------
inline
std::string readFile(const std::string &inputFilename, const AppConfig &appCfg, bool binInput)
{
    std::vector<char> fileDataVec;
    if (!umba::filesys::readFile(inputFilename, fileDataVec))
    {
        // LOG_ERR_OPT << umba::formatMessage("Failed to open input file '$(filename)'").arg("filename",inputFilename).toString() << "\n";
        // return 2;
        throw std::runtime_error( umba::formatMessage("Failed to open input file '$(filename)'").arg("filename", inputFilename).toString());
    }
    std::string
    dataReaded = std::string(fileDataVec.data(), fileDataVec.size());
    dataReaded = convertTextEncoding(dataReaded, appCfg, binInput);
    return dataReaded;
}

//----------------------------------------------------------------------------
inline
std::string readFile(const std::string &inputFilename, const AppConfig &appCfg)
{
    return readFile(inputFilename, appCfg, appCfg.binInput);
}

//----------------------------------------------------------------------------
inline
std::string compressTrimText(std::string text, const AppConfig &appCfg, bool binInput)
{
    if (binInput)
    {
        return text;
    }

    if (appCfg.compressWhitespaces)
    {
        text = textCompress(text, " \t");
    }

    if (appCfg.trimLines)
    {
        text = marty_cpp::stripTextTrailingSpaces(text);
    }

    return text;
}

//----------------------------------------------------------------------------
inline
std::string compressTrimText(std::string text, const AppConfig &appCfg)
{
    return compressTrimText(text, appCfg, appCfg.binInput);
}

//----------------------------------------------------------------------------
inline
std::vector<std::string> formatToLinesAsBinary(const std::string &text, const AppConfig &appCfg)
{
    std::vector<std::string> lines;
    size_t                   numOfLineItems = 0;
    std::string              curLine;

    auto appendLine = [&]()
    {
        numOfLineItems = 0;
        if (curLine.empty())
        {
            return;
        }

        lines.emplace_back(curLine);

        curLine.clear();
    };

    auto appendCharStr = [&](const std::string &chStr)
    {
        if (numOfLineItems==appCfg.lineSize)
        {
            appendLine();
            numOfLineItems = 0;
        }

        if (!curLine.empty())
        {
            curLine.append(", ");
        }

        curLine.append(chStr);
        ++numOfLineItems;
    };


    for(std::string::const_iterator it=text.begin(); it!=text.end(); ++it)
    {
        std::string chStr = cppHelpersFormatUnsigned( (std::uint8_t)*it, appCfg.decFormat?10:16, true /* upper */ , appCfg.decFormat?3:2, appCfg.decFormat?' ':'0' );
        if (!appCfg.decFormat)
        {
           chStr = std::string("0x") + chStr;
        }

        appendCharStr(chStr);
    }

    appendLine();

    return lines;
}

//----------------------------------------------------------------------------
inline
std::vector<std::string> formatToLinesAsEscapedStrings(const std::string &text, const AppConfig &appCfg)
{
    // std::string chStr = marty_cpp::cEscapeString( std::string(1,ch) );

    std::vector<std::string> lines;
    //size_t                   numOfLineItems = 0;
    std::string              curLine;
    char                     prevCh = 0;

    auto appendLine = [&]()
    {
        if (curLine.empty())
        {
            return;
        }

        lines.emplace_back(curLine);
        curLine.clear();
    };

    // Ќадо автоматически перенести данные на новую строку, если длина текущей дошла до лимита
    // Ќо если у нас CR на конце, то наверно за ним может идти LF, и тогда допустим единичный выход за размер
    // ѕо символу LF переводим всегда
    // LF CR как перевод строки - маловеро€тна€ ситуаци€
    auto appendCharStr = [&](const std::string &chStr, char prevCh, char ch)
    {
        if (curLine.size() && curLine.size()>=appCfg.lineSize && prevCh!='\r')
        {
            // if ((curLine.back()!='\r' /*  && curLine.back()!='\n' */ )  /* || bForce */ )
            {
                appendLine();
            }
        }
        else if (curLine.size() && prevCh=='\r' && ch!='\n')
        {
            appendLine();
        }

        curLine.append(chStr);

        if (ch=='\n')
        {
            appendLine();
        }

    };

    //  
    // enum State
    //     { stNormal
    //     , stWaitCr
    //     , stWaitLf
    //     };
    //  
    // State st = stNormal;

    for(std::string::const_iterator it=text.begin(); it!=text.end(); ++it)
    {
        char ch = *it;
        std::string chStr = marty_cpp::cEscapeString( std::string(1,ch) );

        appendCharStr(chStr, prevCh, ch);

        prevCh = ch;

        // switch(st)
        // {
        //     case stNormal:
        //     {
        //         if (ch=='\r')
        //         {
        //             st = stWaitLf;
        //             appendCharStr(chStr)
        //         }
        //         else if (ch=='\n')
        //     }
        //  
        //     case stWaitCr:
        //     {
        //     }
        //  
        //     case stWaitLf:
        //     {
        //     }
        //  
        //     default: throw std::runtime_error("Something goes wrong");
        // }

    }

    if (!curLine.empty())
    {
        lines.emplace_back(curLine);
    }

    return lines;
}




