

#ifndef _eat_h_
#define _eat_h_

#include "ieexec.h"

class CAgent;

class Eat : public IEExec {
public:
	Eat();
	void init   ( IEOwner * owner );
	bool start  ();
	bool update ();
	bool finish ();

	IEOwner * getOwner ();
	const char * getName();
	enum EatStates {
		Go
	};


private:
	std::string m_name;
	CAgent * m_owner;
    TVirtTime m_tickChangeDir;
};

#endif
