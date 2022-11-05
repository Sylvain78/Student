#include <fstream>
#include <iostream>
#include <stdio.h>

#include "Session.h"
#include "server_protocol.pb.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Session"

Session::Session(const char* host, const uint16 port, BListView *output) :
	fHost(host),
	fPort(port)
	{
		fOutput = output;
	}

int Session::Connect() {
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

BListView *Session::GetOutput() {
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
	BListView *output = session->GetOutput();
	uint32 answerSize;
	char *answerBuffer;
	ssize_t received;
	Answer answer;

	GOOGLE_PROTOBUF_VERIFY_VERSION;

	while (true) {
		received = recv(session->fSocket, &answerSize, 4, 0/*flags*/);
		uint32 answerSize32 = ntohl(answerSize);
		answerBuffer = (char *)realloc(answerBuffer, answerSize32);
		received = recv(session->fSocket, answerBuffer, answerSize32, 0/*flags*/);

		if (received == -1) {
			BMessage *messageQuit = new BMessage(kStatusChange);
			messageQuit->AddString("status", "Disconnected");
			output->Parent()->MessageReceived(messageQuit);
			return B_OK;
		}  
		
		answer.ParseFromString(answerBuffer);
		switch (answer.t_case()) {
			
			case Answer::TCase::kOk :
				rgb_color *bgColor = new rgb_color();
				*bgColor = tint_color(ui_color(B_SUCCESS_COLOR), B_LIGHTEN_1_TINT);
				output->LockLooper();
				switch(answer.ok().t_case()) {
					
					case Command::TCase::kProp : {
						output->AddItem(new LatexListItem(new LView(BString(answer.ok().prop().GetDescriptor()->name().c_str()), bgColor)));
						break;
					}
					case Command::TCase::kFirstOrder: {
						std::string command = answer.ok().first_order().GetDescriptor()->name();
						output->AddItem(new LatexListItem(new LView(BString(command.replace(command.find('_'), 1,"\\_").c_str()), bgColor)));
						break;
					}
					case Command::TCase::kInterpreted : {
						output->AddItem(new LatexListItem(new LView(BString(answer.ok().interpreted().GetDescriptor()->name().c_str()), bgColor)));
						break;
					}
					default:
					output->AddItem(new LatexListItem(new LView(BString(answer.ok().GetTypeName().c_str()), bgColor)));
				}
				output->UnlockLooper();
			break;
		}
		if (errno < 0) 
			perror("error in Receive.recv");
		output->LockLooper();
//			output->Insert(answerBuffer,received);
		output->UnlockLooper();
	};

	return B_OK;
}
