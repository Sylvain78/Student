#ifndef _SESSION_H_
#define _SESSION_H_

#include <cerrno>

#include <Catalog.h>
#include <NetworkAddressResolver.h>
#include <Socket.h>
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
		Session(const char* host, const uint16 port);
		int Connect();
	private:
		Flags  fFlags;
		const char*  fHost;
		const uint16 fPort;
		
};	

#endif	// _SESSION_H_
