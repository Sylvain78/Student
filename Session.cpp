#include <fstream>
#include <iostream>
#include <regex.h>
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

	if (status < 0	&& !strcmp(fHost, "localhost") && !fLocalServerLaunched) {
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

status_t Session::_Send(void *data) {
	send_data_params* send_params = (send_data_params *)(data);
	BString *text= send_params->text;
	int socket = send_params->socket;
	BString textProtocol = text->Trim().Append("\n\n");
	int size = textProtocol.Length();
	int sent;
	while (size>0) {
		sent = send(socket, textProtocol.String(), 64*1024,0);//TODO check return value /errno
		textProtocol.Remove(0,sent);
		size -= sent;
		if (sent <= 0)
			break;
	}
	if (sent < 0)
		return B_ERROR;
	else
		return B_OK;
}

status_t Session::Send(BString *text) {
	send_data_params *data =(send_data_params*)malloc(sizeof(send_data_params)); 
	data->text=text;
	data->socket=fSocket;
	thread_id send_thread = spawn_thread(&Session::_Send, "sender", B_NORMAL_PRIORITY, data);
	resume_thread(send_thread);
	return B_OK;

}

status_t Session::Receive(void *data) {
	Session *session = (Session *)data;
	BListView *output = session->GetOutput();
	char *answerSizeBuffer = (char *)malloc(4);
	uint32 answerSize32;
	char *answerBuffer;
	ssize_t received;
	Answer answer;

	GOOGLE_PROTOBUF_VERIFY_VERSION;

	while (true) {

		int total = 0;
		int sizeToRead = 4;
		while ((received = recv(session->fSocket, &answerSizeBuffer[total], sizeToRead-total, 0/*flags*/)) > 0) {
			total += received;
			if (total == 4)
				break;
		}
		answerSize32 = ntohl(* ((uint32 *)answerSizeBuffer));

		answerBuffer = (char *)realloc(answerBuffer, answerSize32);

		total = 0;
		while ((received = recv(session->fSocket, &answerBuffer[total], answerSize32-total, 0/*flags*/)) > 0) {
			total += received;
			if (total == answerSize32)
				break;
		}

		if (received == 0) {
			BMessage *messageQuit = new BMessage(kStatusChange);
			messageQuit->AddString("status", "Disconnected");
			output->Parent()->MessageReceived(messageQuit);
			return B_OK;
		}  

		answer.ParseFromString(answerBuffer);
		switch (answer.t_case()) {
			case Answer::TCase::kOk :
				{
					rgb_color *bgColor = new rgb_color();
					*bgColor = tint_color(ui_color(B_SUCCESS_COLOR), B_LIGHTEN_1_TINT);
					output->LockLooper();
					switch(answer.ok().t_case()) {
						case Command::TCase::kProp : 
							{
								output->AddItem(new LatexListItem(new LView(BString(answer.ok().prop().GetDescriptor()->name().c_str()), LTEXT, bgColor)));
								break;
							}
						case Command::TCase::kFirstOrder: 
							{
								std::string command = answer.ok().first_order().GetDescriptor()->name();
								output->AddItem(new LatexListItem(new LView(BString(command.c_str()), LTEXT, bgColor)));
								break;
							}
						case Command::TCase::kInterpreted: 
							{
								output->AddItem(new LatexListItem(new LView(BString(answer.ok().interpreted().GetDescriptor()->name().c_str()), LTEXT, bgColor)));
								break;
							}
						case Command::TCase::kNotation:
							{
								output->AddItem(new LatexListItem(new LView(BString((
														answer.ok().notation().GetDescriptor()->name()
														+ std::string(" ")
														+ answer.ok().notation().name()).c_str()), LTEXT, bgColor)));
								break;
							}
						case Command::TCase::kTheorem:
							{
								output->AddItem(new LatexListItem(new LView(BString((answer.ok().theorem().GetDescriptor()->name() + std::string(" ") +
														answer.ok().theorem().name()).c_str()), LTEXT, bgColor)));
								break;
							}
						default:
							output->AddItem(new LatexListItem(new LView(BString(answer.ok().GetTypeName().c_str()), LTEXT, bgColor)));
					}
					output->UnlockLooper();
					break;
				}

			case Answer::TCase::kAnswer:
				{
					rgb_color *bgColor = new rgb_color();
					*bgColor = tint_color(ui_color(B_TOOL_TIP_BACKGROUND_COLOR), B_NO_TINT);
					output->LockLooper();
					output->AddItem(new LatexListItem(new LView(BString(answer.answer().c_str()), LTEXT, bgColor)));
					output->UnlockLooper();
					break;
				}
			case Answer::TCase::kError :
				{
					rgb_color *bgColor = new rgb_color();
					*bgColor = tint_color(ui_color(B_FAILURE_COLOR), B_LIGHTEN_1_TINT);
					output->LockLooper();
					output->AddItem(new LatexListItem(new LView(BString(answer.error().error_message().c_str()), LTEXT, bgColor)));
					output->UnlockLooper();
					break;
				}
		}
	};

	return B_OK;
}
