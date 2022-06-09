#ifndef _SESSION_H_
#define _SESSION_H_

#include <SupportDefs.h>

enum Flags {
	Prop,
	First_order,
	Fast,
	Paranoid,
	Compile,
	Interprete
};

class Session {
	public:
	private:
		Flags  fFlags;
		char*  host;
		uint16 port;
		
};	

#endif	// _SESSION_H_
