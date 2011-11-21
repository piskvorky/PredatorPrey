

#ifndef _Wander_H_
#define _Wander_H_

#include "ieexec.h"

class CAgent;

class Wander : public IEExec {

public:
    Wander();
    void init   ( IEOwner * owner );
    bool start  ();
    bool update ();
    bool finish ();

    IEOwner * getOwner ();

    const char * getName();

    enum WanderStates
    {
        Go
    };

    void newDirection ();

private:
    std::string m_name;
    CAgent * m_owner;

    TVirtTime m_dirTick;
    TVirtTime m_tickChangeDir;
};

#endif
