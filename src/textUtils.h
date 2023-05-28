#pragma once

#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/src_normalization.h"
#include <string>

// Используем старый код, лень переделывать

std::string prepareFileExtForMimeType(std::string ext)
{
    while(!ext.empty() && ext.front()=='.')
    {
        ext.erase(ext.begin());
    }

    return marty_cpp::toLower(ext);
}


inline
std::string textCompress( const std::string &text, const std::string &compressChars )
{
    std::string res;

    std::string::size_type i = 0, sz = text.size();

    char prevWs = 0;

    for(; i!=sz; ++i)
    {
        char ch = text[i];
        std::string::size_type chPos = compressChars.find(ch);
        //if (ch==' ' || ch=='\t')
        if (chPos!=compressChars.npos)
        {
            if (ch==prevWs)
            {
                // simple skip
            }
            else
            {
                prevWs = ch;
                res.append(1,ch);
            }
        }
        else
        {
            prevWs = 0;
            res.append(1,ch);
        }
    }

    return res;

}


inline
std::string textDetectLinefeed( const std::string &text )
{
    // CRLF most used
    std::string::size_type pos = text.find('\n');
    if (pos == text.npos)
    {
        pos = text.find('\r');
        if (pos != text.npos)
            return "\r";
        // no linefeeds found at all
        return "\r\n";
    }

    if (pos==0)
    {
        if ((pos+1) != text.size() && text[pos+1]=='\r')
            return "\n\r"; // MAC style
        return "\n";
    }

    if (text[pos-1]=='\r')
        return "\r\n";

    if ((pos+1) != text.size() && text[pos+1]=='\r')
        return "\n\r"; // MAC style

    return "\n";
}

inline
std::string detectLinefeed( const std::string &text )
{
    return textDetectLinefeed(text);
}



inline
void textSplitToLines( const std::string &text, std::vector<std::string> &lines, std::string &detectedLinefeed )
{
    detectedLinefeed = textDetectLinefeed(text);
    if (detectedLinefeed.empty())
        throw std::runtime_error("BEBEBEBEBE");

    // splitToVector( text, lines, detectedLinefeed.back() );
    //rtrim(lines, "\r\n");

    lines = marty_cpp::splitToLinesSimple(text, true /* addEmptyLineAfterLastLf */, detectedLinefeed.back());

    for(auto &l : lines)
    {
        l = marty_cpp::simple_rtrim(l, [](char ch) { return ch=='\r' || ch=='\n'; });
    }

}

