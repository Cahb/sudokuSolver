#include "parser.h"
#include <conio.h>
using namespace std;

string parser::extractKey(std::string toParse, std::string key)
{
    size_t found=-1;
    if(!key.empty() && toParse.size() > 4 && toParse[0] == '{' && toParse[toParse.size()-1]=='}')
    {
        found = toParse.find(key);
        if(found != string::npos)
        {
            found += key.size() + 3;
            toParse = toParse.substr(found, toParse.size()-found);
            found = toParse.find_first_of("\"");
            if(found != string::npos)
            {
                return toParse.substr(0, found);
            }
        }
    }
    return string("EMPTY");
}
