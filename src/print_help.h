#pragma once

// https://stackoverflow.com/questions/11697820/how-to-use-date-and-time-predefined-macros-in-as-two-integers-then-stri
void printOnlyVersion()
{
    std::cout<<_2cVersion<<"\n";
}

void printNameVersion( const std::string &indent = "" )
{
    std::cout<<indent<<"2c file convertion utility version ";
    printOnlyVersion();
    //<<rdlcVersion<<"\n";
}

void printCommitHash( const std::string &indent = "" )
{
    if (_2cCommitHash.empty())
        return;
    std::cout<<indent<<"#"<<_2cCommitHash<<"\n";
}

void printBuildDateTime( const std::string &indent = "" )
{
    std::cout<<indent<<"Built at "<<_2cBuildDate<<" "<< _2cBuildTime<<"\n";
}


