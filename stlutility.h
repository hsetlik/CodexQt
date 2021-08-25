#ifndef STLUTILITY_H
#define STLUTILITY_H

#include <regex>
#include <map>
#include <unordered_map>

struct stdu
{
public:
    static std::vector<std::string> matchesAsVector(std::string body, std::regex reg)
        {
            std::vector<std::string> strings;
            auto results = std::smatch{};
            for (std::sregex_iterator it = std::sregex_iterator(body.begin(), body.end(), reg);
                     it != std::sregex_iterator(); it++)
            {
                std::smatch match;
                match = *it;
                auto str = match.str(0);
                strings.push_back(str);
            }
            return strings;
        }
};

#endif // STLUTILITY_H
