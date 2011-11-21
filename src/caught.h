
#ifndef _caught_h_
#define _caught_h_

#include "ieexec.h"

class CAgent;

class Caught : public IEExec {
public:
    Caught();
    void init   ( IEOwner * owner );
    bool update ();
    IEOwner * getOwner ();
    const char * getName();

private:
    std::string m_name;
    CAgent * m_owner;
};

#endif
