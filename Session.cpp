#include "Session.h"
#include <iostream>
#include <stdio.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Session"

Session::Session(const char* host, const uint16 port, BTextView *output) :
	fHost(host),
	fPort(port)
	{
		fOutput = output;
	}

int Session::Connect(BTextView *outputView) {
	struct hostent* host = gethostbyname(fHost);
	if (host == NULL) {
		perror("gethostbyname");
		return 1;
	}

	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(struct sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(fPort);
	serverAddr.sin_addr = *((struct in_addr*)host->h_addr);

	uint32 cookie = 0;
	bool success = false;
	const char *errorString;
	int errorCode;

	int connection = socket(AF_INET, SOCK_STREAM, 0);
	if (connection < 0) {
		errorString = B_TRANSLATE("Could not create socket");
		errorCode = errno;
		return B_ERROR;
	}

	status_t status = connect(connection,(struct sockaddr*)&serverAddr, sizeof(struct sockaddr_in));

	if (status < 0  && !strcmp(fHost, "localhost") && !fLocalServerLaunched) {
		status_t err_connect = errno;
		perror(NULL);
		close(connection);
		LaunchLocalServer(fPort);
		while ( connection = socket(AF_INET, SOCK_STREAM, 0),connect(connection, (struct sockaddr*)&serverAddr,
				sizeof(struct sockaddr_in)) < 0) {
					status_t err_connect = errno;
					perror(NULL);
					close(connection);

					snooze(100*1000);
		}
	} else {
		if(!strcmp(fHost, "localhost")) 
			fLocalServerLaunched = true;
	}


	fSocket = connection;

	thread_id receive_thread = spawn_thread(&Session::Receive, "receiver", B_NORMAL_PRIORITY, this);
	resume_thread(receive_thread);

	return fSocket;
}

bool Session::IsLocalServerLaunched() {
	return fLocalServerLaunched;
}

void Session::LaunchLocalServer(const uint16 port) {
	((App *)be_app)->LaunchLocalServer(port);
	fLocalServerLaunched = true;
}

BTextView *Session::GetOutput() {
	return fOutput;
}

status_t Session::Send(BString *text) {
	BString textProtocol = text->Append("\n\n");
	std::cout << "send : " << textProtocol.String()<< std::endl;
	int sent = send(fSocket, textProtocol.String(), textProtocol.Length(), 0);
	if (sent == textProtocol.Length()) {
		return B_OK;
	} else {
		return B_ERROR;
	}
}

status_t Session::Receive(void *data) {
	Session *session = (Session *)data;
	BTextView *output = session->GetOutput();
 	char buffer[1];
	ssize_t received;

	while (received = recv(session->fSocket, buffer, 1, 0/*flags*/) != NULL) {
		if (errno < 0) 
			perror("test");
		output->LockLooper();
			output->Insert(buffer,received);
		output->UnlockLooper();
	};

	return B_OK;
}
