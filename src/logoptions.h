#pragma once

struct LogOptions
{
    std::set<std::string>                     disabledWarnings;

    bool allowWarn(std::string warnType) const
    {
        if (disabledWarnings.find("all")!=disabledWarnings.end())
            return false; // найден all, запрещаем все варнинги
        return disabledWarnings.find(warnType)==disabledWarnings.end();
    }

};
