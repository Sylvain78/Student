#ifndef _SESSION_H_
#define _SESSION_H_

#include <cerrno>
#include <netdb.h>

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
		Session(const char* host, const uint16 port, BTextView *output);
		int Connect(BTextView *outputView);
		status_t Send(BString *text);
		static status_t Receive(void *data);
		bool IsLocalServerLaunched();
		void LaunchLocalServer(const uint16 port);
		BTextView* GetOutput();

	private:
		Flags  fFlags;
		const char*  fHost;
		const uint16 fPort;
		bool fLocalServerLaunched=false;
		int fSocket;
		BTextView *fOutput;
		
};	

#endif	// _SESSION_H_
