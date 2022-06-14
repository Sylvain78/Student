#include "Session.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Session"
 
Session::Session(const char* host, const uint16 port) :
	fHost(host),
	fPort(port) 
	{}
	
int Session::Connect() {
	BNetworkAddressResolver resolver(fHost, fPort);
	BNetworkAddress address;
	
	uint32 cookie = 0;
	bool success = false;
	const char *errorString;
	int errorCode;
	
	if (resolver.InitCheck() != B_OK) {
		errorString = B_TRANSLATE("Could not resolve server address");
		return B_ENTRY_NOT_FOUND;
	}

	int connection = socket(AF_INET, SOCK_STREAM, 0);
	if (connection < 0) {
		errorString = B_TRANSLATE("Could not create socket");
		errorCode = errno;
		return B_ERROR;
	}

	while (resolver.GetNextAddress(&cookie, address) == B_OK) {
		if (connect(connection,&address.SockAddr(),
				address.Length()) >= 0 ) {
			success = true;
			break;
		}
	}

	if (!success) {
		errorString = B_TRANSLATE("Could not connect to fHost/fPort TODO");
		close(connection);
		return B_ERROR;
	} else {
		fSocket = connection;
		return fSocket;
	}
}

bool Session::IsLocalServerLaunched() {
	return fLocalServerLaunched;
}

void Session::LaunchLocalServer(const uint16 port) {
	((App *)be_app)->LaunchLocalServer(port);
	fLocalServerLaunched = true;
}

status_t Session::Send(BString *text) {
	BString textProtocol = text->Append("\n\n");
	int sent = send(fSocket, &textProtocol, textProtocol.Length(), 0);
	if (sent == textProtocol.Length()) {
		return B_OK;
	} else {
		return B_ERROR;
	}
}
