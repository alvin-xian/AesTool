#include "hex.h"

std::string String2Hex(const unsigned char *src, size_t len)
{
    std::string dest = "";
    for (size_t i = 0; i < len; ++i)
    {
        char  tmp[3] = {0};
        sprintf(tmp, "%02X", src[i]);
        dest += tmp;
    }
    return dest;
}

std::string Hex2String(const std::string &hex)
{
    std::string dest = "";
    if (hex.length() % 2 != 0)
    {
        return dest;
    }

    int len = hex.length();
    std::string newString;
    for(int i=0; i< len; i+=2)
    {
        std::string byte = hex.substr(i,2);
        char chr = (char) (int)strtol(byte.c_str(), NULL, 16);
        newString.push_back(chr);
    }
    return newString;
}
