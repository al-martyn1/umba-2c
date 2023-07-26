#pragma once

#include <string>
#include <vector>
#include <map>

#include "umba/program_location.h"
#include "umba/enum_helpers.h"
#include "umba/flag_helpers.h"
#include "umba/regex_helpers.h"
#include "umba/filename.h"

#include <time.h>

#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/src_normalization.h"

#include "cppHelpers.h"
#include "textUtils.h"


#include "2c_common/_2c_xor_encrypt.h"


//----------------------------------------------------------------------------
#if defined(min)

    #undef min

#endif

#if defined(max)

    #undef max

#endif

//----------------------------------------------------------------------------
#if defined(DEBUG) || defined(_DEBUG)

    // При отладке удобнее разглядывать std::map

    template<typename StringType>
    using StringStringMap = std::map<StringType,StringType>;

    template<typename StringType>
    using StringSet = std::set<StringType>;

#else

    // В релизе std::unordered_map быстрее
    template<typename StringType>
    using StringStringMap = std::unordered_map<StringType,StringType>;

    template<typename StringType>
    using StringSet = std::unordered_set<StringType>;

    template<typename StringType>
    using StringSet = std::unordered_set<StringType>;

#endif



//----------------------------------------------------------------------------
struct XorEncryptionConfig
{
    _2c::EKeySize                            xorEncKeySize        = _2c::EKeySize::Unknown; //!<
    unsigned                                 xorEncSeed           = 0                     ; //!< 
    unsigned                                 xorEncInc            = 0                     ; //!< 

    void clear()
    {
        xorEncKeySize        = _2c::EKeySize::Unknown; //!<
        xorEncSeed           = 0                     ; //!< 
        xorEncInc            = 0                     ; //!< 
    }


    bool needXorEncrypt() const
    {
        return xorEncKeySize!=_2c::EKeySize::Unknown;
    }

    std::string xorEncrypt(std::string data) const
    {
        if (!needXorEncrypt())
        {
            return data;
        }

        _2c::xorEncrypt(data.begin(), data.end(), xorEncKeySize, xorEncSeed, xorEncInc);

        return data;
    }

    bool parseCommandLineOptionValue(const std::string &xorOptionsStr, std::string &errMsg)
    {
        std::vector<std::string> xorOptionsVec;
        // splitToVector( xorOptionsStr, xorOptionsVec, ',' );
        xorOptionsVec = marty_cpp::splitToLinesSimple(xorOptionsStr, false /* addEmptyLineAfterLastLf */, ',');

        if (xorOptionsVec.size()<1)
        {
            errMsg = "XOR encryption option - invalid option format";
            return false;
        }

        std::mt19937 rng = _2c::xorEncryptionGetRandomGenerator();

        _2c::EKeySize ksz = _2c::xorEncryptionKeySizeFromString(xorOptionsVec[0], rng);
        if (ksz==_2c::EKeySize::Unknown)
        {
            errMsg = "XOR encryption option - invalid KeySize value";
            return false;
        }

        //  
        // while(xorOptionsVec.size()<3)
        // {
        //     xorOptionsVec.emplace_back("Random");
        // }

        std::size_t keySize = _2c::xorEncryptionKeySize(ksz);
        if (keySize==0)
        {
            clear();
            return true;
        }
        else
        {
            if (xorOptionsVec.size()<2)
            {
                errMsg = "XOR encryption option - invalid option format";
                return false;
            }

            if (xorOptionsVec.size()<3)
                xorOptionsVec.push_back("0");

            bool bValid = false;
            xorEncSeed = _2c::xorEncryptionSeedFromString(ksz, xorOptionsVec[1], rng, &bValid);
            if (!bValid)
            {
                errMsg = "XOR encryption option - invalid seed value";
                return false;
            }

            bValid = false;
            xorEncInc = _2c::xorEncryptionSeedFromString(ksz, xorOptionsVec[2], rng, &bValid);
            if (!bValid)
            {
                errMsg = "XOR encryption option - invalid key increment value";
                return false;
            }

            xorEncKeySize = ksz;

            // outputAsString = false;

            return true;    
        }
    
    }


};



struct AppConfig
{
    // umba-2c & umba-2rcfs commons
    //bool                                     quet       = false;
    bool                                     bOverwrite           = false  ; //!< Overwrite output file, if exist
    bool                                     disableEndLinefeed   = false  ; //!< Do not add linefeed at the end of text
    bool                                     dontKeepExt          = false  ; //!< Remove file extention from autogenerated C-lang var name
    bool                                     binInput             = false  ; //<! Input is binary
    //std::string                              outputLineSep                 ; //<! Output line separator (for data)

    marty_cpp::ELinefeedType                 outputLineFeed       = marty_cpp::ELinefeedType::detect;

    bool                                     outputAsString       = false  ; //!< Generate output as C-string, else - array of HEXs
    size_t                                   lineSize             = 0      ; //!< Set max line size or number of array items per line
    size_t                                   lineSizeBin          = 0      ; //!< Set max number of array items per line for binary (2RCFS)
    size_t                                   lineSizeText         = 0      ; //!< Set max line size for text (2RCFS)
    bool                                     staticArray          = true   ; //!< Generate static data
    bool                                     nonConstArray        = false  ; //!< Generate non-const data
    bool                                     decFormat            = false  ; //!< Use decimal format for array items (default is hex)
    bool                                     compressWhitespaces  = false  ; //!< Compress whitespaces - reduce multiple WS (\\s, \\t) to single
    bool                                     trimLines            = false  ; //!< Trim lines at right - remove trailing spaces

    std::string                              cname                         ; //!< C-array/string name, override auto-generated from file name

    std::string                              inputEnc                      ; //!< Set input encoding (for text data)
    std::string                              outputEnc            = "UTF-8"; //!< Set target encoding

    bool                                     addMimeType          = false  ; //!< Set mime type
    std::string                              mimeType             = ""     ; //!< Set mime type as taken. If no mime type explicitly taken, it will be autodetected
    StringStringMap<std::string>             mimeTypesMap;

    bool                                     addLastModified      = false  ; //!< Set last-modified
    std::string                              lastModified         = ""     ; //!< Set last-modified as taken. If not explicitly taken, it will be generated by strftime("%a, %d %b %Y %H:%M:%S GMT")

    bool                                     addResourceFileName  = false  ; //!< Add resource file name variable
    std::string                              resourceFileName              ; //!< Add resource file name. Parameter value is optional, autogenerated on filename, if not explicitly taken

    bool                                     addResourceFileSize  = false  ; //!< Add resource size variable

    bool                                     generateClass        = false  ; //!< Generate class CLASSNAME for resource
    std::string                              className                     ; //!< class name

    std::vector< std::string >               classNamespaces               ; //!< 

    bool                                     generateHeader       = false  ; //!< Generate header file (.h) instead of .c

    bool                                     base64               = false  ; //!< Encode input file into base64
    int                                      base64LineLen        = 0      ; //!< Set base64 encoded line max len
    bool                                     base64Filling        = false  ; //!< Add base64 filling chars at end

    XorEncryptionConfig                      dataXorEncryptionConfig       ; //!< Data Xor encryption options
    XorEncryptionConfig                      filenameXorEncryptionConfig   ; //!< Filename Xor encryption options - for 2RCFS



    // Только для umba-2rcfs
    bool                                     allFiles = false;
    // bool                                     scanMode = false; // хз откуда взялось
    std::vector<std::string>                 scanPaths;
    std::vector<std::string>                 includeFilesMaskList;
    std::vector<std::string>                 excludeFilesMaskList;

    // Если файлы не подходят ни под одну маску, будет использоваться значение значение по умолчанию из binInput
    // Если файл подходит под обе маски, то это ошибка
    // Надо сделать эту проверку до реальной работы, даже до создания выходного файла
    std::vector<std::string>                 binaryFilesMaskList;
    std::vector<std::string>                 textFilesMaskList  ;

    std::vector<std::string>                 removeLinefeedMaskList;

    size_t getLineSizeBin() const
    {
        return lineSizeBin ? lineSizeBin : 16;
    }

    size_t getLineSizeText() const
    {
        return lineSizeText ? lineSizeText : 96;
    }

    size_t getLineSize() const
    {
        return lineSize;
    }

    void adjust_lineSize()
    {
        if (lineSize)
            return;

        if (outputAsString)
            lineSize = 100;
        else
            lineSize = 16;
    }

    bool needXorEncryptData() const
    {
        return dataXorEncryptionConfig.needXorEncrypt();
    }

    std::string xorEncryptData(std::string data) const
    {
        return dataXorEncryptionConfig.xorEncrypt(data);
    }

    bool needXorEncryptFilename() const
    {
        return filenameXorEncryptionConfig.needXorEncrypt();
    }

    std::string xorEncryptFilename(std::string data) const
    {
        return filenameXorEncryptionConfig.xorEncrypt(data);
    }

    static
    std::string textAppend(std::string text, const std::string &strAppend)
    {
        if (!text.empty() && !strAppend.empty())
        {
            text.append(1, ' ');
        }
    
        text.append(strAppend);
    
        return text;
    }


    std::string getMimeTypeByExt(std::string ext) const
    {
        if (!mimeType.empty())
        {
            return mimeType;
        }

        ext = prepareFileExtForMimeType(ext);

        StringStringMap<std::string>::const_iterator it = mimeTypesMap.find(ext);
        if (it!=mimeTypesMap.end())
            return it->second;

        return std::string("application/octet-stream");
    }

    std::string getMimeTypeByFileName(const std::string &fn) const
    {
        return getMimeTypeByExt(umba::filename::getFileExtention(fn));
    }


    bool isOutputEncodingUtf8() const
    {
        auto tmpEnc = marty_cpp::toLower(outputEnc);
        if (tmpEnc=="utf-8" || tmpEnc=="utf8")
            return true;
        return false;
    }


    void adjust_mimeType(const std::string &inputFilename)
    {
        if (addMimeType)
        {
            if (mimeType.empty())
            {
                //NOTE: !!! А оно надо, toLower?
                //std::string srcExt = toLower(umba::filename::getFileExtention(inputFilename));
                // std::string srcExt = umba::filename::getFileExtention(inputFilename);
                // std::string srcExt = marty_cpp::toLower(umba::filename::getFileExtention(inputFilename));
                // if (!srcExt.empty() && srcExt[0]!='.')
                //    srcExt = std::string(".") + srcExt;
                //  
                // const std::map< std::string, std::string >& mimes = getMimeTypes();
                // std::map< std::string, std::string >::const_iterator mit = mimes.find(srcExt);
                // if (mit==mimes.end())
                //     mimeType = "application/octet-stream";
                // else
                //     mimeType = mit->second;

                mimeType = getMimeTypeByFileName(inputFilename);
            }
        }

    }

    //! Requires full name with path
    void adjust_lastModified(const std::string &inputFilename)
    {
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
        }
    }

    void adjust_resourceFileName(const std::string &inputFilename)
    {
        if (addResourceFileName)
        {
            if (resourceFileName.empty())
            {
                resourceFileName = umba::filename::getFileName(inputFilename);
            }
        }
    }

    //! cname - C-array/string name, override auto-generated from file name
    void adjust_cname(const std::string &inputFilename)
    {
        if (cname.empty())
        {
            if (!dontKeepExt)
                cname = makeCppName(umba::filename::getFileName(inputFilename));
            else
                cname = makeCppName(umba::filename::getName(inputFilename));
        }
    
        if (!cname.empty())
        {
            // if (appConfig.cname[0]>='0' && appConfig.cname[0]<='9')
            //     appConfig.cname = std::string("_") + appConfig.cname;
            cname = marty_cpp::fixName(cname, true /* fixStartDigit */  /* , forceAllowedChars */ );
        }
    
    }

    void adjust_outputAsString()
    {
        if (needXorEncryptData())
        {
            outputAsString = false;
        }
    }


    //! Set target encoding
    void adjust_outputEnc()
    {
        if (outputEnc.empty())
            outputEnc = "UTF-8";
    }

    void adjust_trim()
    {
        if (compressWhitespaces)
            trimLines = true;
    }

    void adjust(const std::string &inputFilename)
    {
        adjust_mimeType(inputFilename);
        adjust_lastModified(inputFilename);
        adjust_resourceFileName(inputFilename);
        adjust_cname(inputFilename);
        adjust_outputAsString();
        adjust_outputEnc();
        adjust_trim();
        adjust_lineSize();
    }

    AppConfig adjusted(const std::string &inputFilename) const
    {
        AppConfig cfgCopy = *this;
        cfgCopy.adjust(inputFilename);
        return cfgCopy;
    }

    std::string getAdustedOutputFilename(const std::string &inputFilename, std::string outputFilename) const
    {
        if (!outputFilename.empty())
            return outputFilename;

        outputFilename = umba::filename::getPathFile( inputFilename );
        std::string ext = umba::filename::getFileExtention( inputFilename );
        if (!ext.empty())
            outputFilename += std::string("_") + ext;
        outputFilename = umba::filename::appendExtention( outputFilename, std::string("c") );

        return outputFilename;
    }

    static
    std::string normalizeLinefeeds(const std::string &text, bool binInput, marty_cpp::ELinefeedType outputLineFeed)
    {
        if (binInput)
        {
            return text;
        }

        switch(outputLineFeed)
        {
            case marty_cpp::ELinefeedType::lf  :
            case marty_cpp::ELinefeedType::cr  :
            case marty_cpp::ELinefeedType::lfcr:
            case marty_cpp::ELinefeedType::crlf:
            case marty_cpp::ELinefeedType::linefeedRemove:
            {
                std::string textLfNormalized   = marty_cpp::normalizeCrLfToLf(text);
                std::vector<std::string> lines = marty_cpp::splitToLinesSimple(textLfNormalized, true /* addEmptyLineAfterLastLf */, '\n' /* lfChar */ );
                return mergeLines(lines, outputLineFeed, false /* addTrailingNewLine */);
            }

            default: return text;
        }
    }

    std::string normalizeLinefeeds(const std::string &text) const
    {
        return normalizeLinefeeds(text, binInput, outputLineFeed);
        #if 0
        if (binInput)
        {
            return text;
        }

        switch(outputLineFeed)
        {
            case marty_cpp::ELinefeedType::lf  :
            case marty_cpp::ELinefeedType::cr  :
            case marty_cpp::ELinefeedType::lfcr:
            case marty_cpp::ELinefeedType::crlf:
            {
                std::string textLfNormalized   = marty_cpp::normalizeCrLfToLf(text);
                std::vector<std::string> lines = marty_cpp::splitToLinesSimple(textLfNormalized, true /* addEmptyLineAfterLastLf */, '\n' /* lfChar */ );
                return mergeLines(lines, outputLineFeed, false /* addTrailingNewLine */);
            }

            default: return text;
        }
        #endif
    }

    std::string getArrayTypeName() const
    {
        return outputAsString && !needXorEncryptData() ? "char" : "unsigned char";
    }

    std::string getArrayTypeName(bool oas) const
    {
        return oas && !needXorEncryptData() ? "char" : "unsigned char";
    }

    std::string getStaticConst() const
    {
        std::string staticConst;
        if (staticArray)
           staticConst = textAppend(staticConst, "static"); // std::string("static ") + staticConst;
        if (!nonConstArray)
           staticConst = textAppend(staticConst, "const"); // std::string("const ") + staticConst;
        return staticConst;
    }



}; // struct AppConfig




