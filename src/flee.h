

#ifndef _flee_h_
#define _flee_h_

#include "ieexec.h"

class CAgent;

class Flee : public IEExec {

public:
    Flee();

    void init   ( IEOwner * owner );
    bool start  ();
    bool update ();
    bool finish ();

    IEOwner * getOwner ();

    const char * getName();

    enum FleeStates
    {
        Go
    };

private:
    std::string m_name;
    CAgent * m_owner;
	TVirtTime m_dirTick;
	TVirtTime m_minDirTick;
	void newDirection(TVirtTime minTick);

};

#endif
