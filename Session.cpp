#include "Session.h"

int SessionConnect(const char* host, uint16 port) {
	BNetworkAddressResolver resolver(hostname, port);
	BNetworkAddress address;
	uint32 cookie = 0;
	bool success = false;

	if (resolver.InitCheck() != B_OK) {
		*errorString = B_TRANSLATE("Could not resolve server address");
		return B_ENTRY_NOT_FOUND;
	}

	int connection = socket(AF_INET, SOCK_STREAM, 0);
	if (connection < 0) {
		*errorString = B_TRANSLATE("Could not create socket");
		*errorCode = errno;
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
		*errorString = B_TRANSLATE("Could not connect to host/port TODO");
		close(connection);
		return B_ERROR;
	} else {
		return connection;
	}
}
