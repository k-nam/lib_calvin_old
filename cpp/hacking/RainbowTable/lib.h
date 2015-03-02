#ifndef HACKING__LIB_H
#define HACKING__LIB_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

namespace hacking__lib
{
// read until newline, and split by tab
std::vector<std::string>
split(std::string const &line, char delim);

}
#endif