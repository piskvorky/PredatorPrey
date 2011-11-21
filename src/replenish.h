

#ifndef _replenish_h_
#define _replenish_h_

#include "ieexec.h"

class CAgent;

class Replenish : public IEExec {
public:
	Replenish();
	void init   ( IEOwner * owner );
	bool start  ();
	bool update ();
	bool finish ();

	IEOwner * getOwner ();
	const char * getName();
	enum ReplenishStates {
		Go
	};


private:
	std::string m_name;
	CAgent * m_owner;
};

#endif
