#include "Utils.h"

string& Utils::matchAndRepl(string& dest , const char* _pattern, const char* _repl)
{
    const char* base = dest.c_str();
    int offs;
    for (offs = 0; offs < dest.size(); offs++)
    {
        if (strncmp(_pattern, base + offs, strlen(_pattern)) == 0) break;
    }
    if (offs > 0 && offs < dest.size()) dest.replace(offs, strlen(_pattern), _repl);
    return dest;
}
