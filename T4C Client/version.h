#ifndef VERSION_H_11042000
#define VERSION_H_11042000

namespace Version{
    const char   *GetVersionText();
    unsigned long GetVersion();
    const char   *GetBuild();
    char         *
       GetSubVersion();
};

#endif // VERSION_H_11042000