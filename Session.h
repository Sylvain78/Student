#ifndef _SESSION_H_
#define _SESSION_H_

#include <cerrno>

#include <Catalog.h>
#include <NetworkAddressResolver.h>
#include <Socket.h>
#include <SupportDefs.h>

#include "App.h"

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
		status_t Send(BString *text);
		bool IsLocalServerLaunched();
		void LaunchLocalServer(const uint16 port);
	private:
		Flags  fFlags;
		const char*  fHost;
		const uint16 fPort;
		bool fLocalServerLaunched=false;
		int fSocket;
		
};	

#endif	// _SESSION_H_
