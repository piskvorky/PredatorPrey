
#ifndef _exhausted_h_
#define _exhausted_h_

#include "ieexec.h"

class CAgent;

class Exhausted : public IEExec {
public:
    Exhausted();
    void init   ( IEOwner * owner );
    bool update ();
    IEOwner * getOwner ();
    const char * getName();

private:
    std::string m_name;
    CAgent * m_owner;
};

#endif
