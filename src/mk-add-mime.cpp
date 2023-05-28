/*! \file
    \brief Генерируем --add-mime-type по уже изготовленной мапе mime-типов
 */

#include "textUtils.h"

#include "mimes.h"
#include <string>
#include <map>
#include <set>
#include <vector>
#include <iostream>



int main(int argc, char* argv[])
{
    std::map<std::string, std::set<std::string> >  extByMimeType;

    {
        const std::map< std::string, std::string > &mimes = getMimeTypes();
        std::map< std::string, std::string >::const_iterator mit = mimes.begin();
        for(; mit!=mimes.end(); ++mit)
        {
            std::string ext       = prepareFileExtForMimeType(mit->first);
            std::string mimeType  = mit->second;
            if (ext.empty())
                continue;
            extByMimeType[mimeType].insert(ext);
        }
    }

    std::map<std::string, std::set<std::string> >::const_iterator it = extByMimeType.begin();
    for(; it!=extByMimeType.end(); ++it)
    {
        std::string mimeType = it->first;
        std::set<std::string>::const_iterator extIt = it->second.begin();
        unsigned extCnt = 0;
        for(; extIt!=it->second.end(); ++extIt)
        {
            //if (extIt->empty())
            if (!extCnt)
               std::cout << "--add-mime-type=";
            else
               std::cout << ",";

            std::cout << *extIt;

            ++extCnt;
        }

        if (extCnt)
        {
            std::cout << ":" << mimeType << "\n";
        }

    }

    return 0;
}


