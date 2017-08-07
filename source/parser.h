#ifndef PARSER_H
#define PARSER_H
#pragma once

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

namespace parser
{
//std::string parseString(std::string &toParse);
std::string extractKey(std::string toParse, std::string key);
void saveToFile(FILE *f1, std::string &str);
}

#endif
