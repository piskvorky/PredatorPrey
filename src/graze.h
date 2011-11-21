
#ifndef _graze_h_
#define _graze_h_

#include "ieexec.h"

class CPrey;

class Graze : public IEExec {
public:
    Graze();
    void init   ( IEOwner * owner );
    bool start  ();
    bool update ();
    bool finish ();
    IEOwner * getOwner ();
    const char * getName();
    enum GrazeStates { Go };

private:
    std::string m_name;
    CPrey * m_owner;
};

#endif
