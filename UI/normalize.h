#include <string>

#ifndef NORMALIZE
#define NORMALIZE
inline string lower(string s)
{
    for (char &c : s)
    {
        if ('A' <= c && c <= 'Z')
            c = c - 'A' + 'a';
    }
    return s;
}

inline string upper(string s)
{
    for (char &c : s)
    {
        if ('a' <= c && c <= 'z')
            c = c - 'a' + 'A';
    }
    return s;
}

inline string slugify(string s)
{
    int i = 0;
    for (char &c : s)
    {
        if ('A' <= c && c <= 'Z')
        {
            c = c - 'A' + 'a';
            s[i++] = c;
        }
        else if (('a' <= c && c <= 'z') || ('0' <= c && c <= '9'))
        {
            s[i++] = c;
        }
        else
        {
            continue;
        }
    }
    s = s.substr(0, i);
    return s;
}

#endif 