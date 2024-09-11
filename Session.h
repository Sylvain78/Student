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
	First_Order,
	Fast,
	Paranoid,
	Compile,
	Interprete
};


typedef struct {
	BString *text;
	int socket;
} send_data_params;


class Session {
	public:
		Session(const char* host, const uint16 port, SessionView *output);
		int Connect();
		status_t Send(BString *text);
		bool IsLocalServerLaunched();
		void LaunchLocalServer(const uint16 port);
		BListView* GetOutput();

	private:
		Flags  fFlags;
		const char*  fHost;
		const uint16 fPort;
		bool fLocalServerLaunched=false;
		int fSocket;
		SessionView *fOutput;
		static status_t _Send(void *send_data_params);
		static status_t Receive(void *data);
};	

#endif	// _SESSION_H_
