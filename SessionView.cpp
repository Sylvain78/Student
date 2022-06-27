#ifndef _SESSIONVIEW_CPP_
#define _SESSIONVIEW_CPP_

#include <iostream>

#include "Session.h"
#include "SessionView.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "SessionView"


SessionView::SessionView(const char * name) : BView(name, B_SUPPORTS_LAYOUT),
	fDemonstration(new LatexListScrollView()),
	fModeBox(new BBox("mode")),
	fSpeedBox(new BBox("speed")),
	fCompileBox(new BBox("compile")),
	fSession(NULL),

	fStatusText(new BString("Not connected")) {

	fInputView  = new BTextControl("InputView",0,0,0);

	fButton = new BButton("Envoi", "Envoi", new BMessage(kEnvoi));

	BStringView* statusView = new BStringView("status",NULL);

	fInputView->MakeFocus(true);

	fOutputView = new BTextView(BRect(), "OutputView", BRect(), B_FOLLOW_ALL, B_FRAME_EVENTS | B_WILL_DRAW);

	BScrollView* fScrollOutputView = new BScrollView("ScrollOutput",fOutputView, B_FOLLOW_ALL, 0, true, true);
	fScrollOutputView->SetViewColor(255,255,255);
	fOutputView->MakeEditable(false);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddGroup(B_HORIZONTAL)
	    	.Add(fModeBox)
			.Add(fSpeedBox)
			.Add(fCompileBox)
		.End()
		.Add(fDemonstration)
		.AddGroup(B_HORIZONTAL)
			.SetInsets(5,5,5,5)
			.Add(fInputView)
			.Add(fButton)
		.End()
		.AddGroup(B_HORIZONTAL)
		//TODO font & color
			.Add(statusView, 0.1f)
		.End()
		.Add(fScrollOutputView)

	.End();


	BRadioButton* mode_prop = new BRadioButton("Prop", new BMessage('Prop'));
	BRadioButton* mode_first_order = new BRadioButton(B_TRANSLATE("First order"), new BMessage('Frst'));
	BRadioButton* speed_fast = new BRadioButton("Fast", new BMessage('Fast'));
	BRadioButton* speed_paranoid = new BRadioButton("Paranoid", new BMessage('Prnd'));
	BRadioButton* compile = new BRadioButton("Compile", new BMessage('Cmpl'));
	BRadioButton* interprete = new BRadioButton("Interprete", new BMessage('Intp'));

	BGroupLayout *modeBoxLayout = BLayoutBuilder::Group<>(B_HORIZONTAL)
	.SetInsets(2,0,2,0)
	.Add(mode_prop)
	.Add(mode_first_order);

	BGroupLayout *speedBoxLayout = BLayoutBuilder::Group<>(B_HORIZONTAL)
	.SetInsets(2,0,2,0)
	.Add(speed_fast)
	.Add(speed_paranoid);

	BGroupLayout *compileBoxLayout = BLayoutBuilder::Group<>(B_HORIZONTAL)
	.SetInsets(2,0,2,0)
	.Add(compile)
	.Add(interprete);

	fModeBox->SetLabel(new BStringView("Order label", "Order"));
	fModeBox->AddChild(modeBoxLayout->View());

	fSpeedBox->SetLabel(new BStringView("Speed label", "Speed"));
	fSpeedBox->AddChild(speedBoxLayout->View());

	fCompileBox->SetLabel(new BStringView("Compile label", "Compile"));
	fCompileBox->AddChild(compileBoxLayout->View());
}


void SessionView::MessageReceived(BMessage* message) {
	switch(message->what) {
		case (kConnect) : {
		 	const char* host = message->GetString("host", "");
		 	uint16 port = message->GetUInt16("port",0);
		 	std::cout << "connect to " << host << ":" << port << std::endl;

		 	BMessage* statusMessage = new BMessage(kStatusChange);
		 	char* newStatus = (char *)malloc(strlen("Connecting to ") +strlen(host)+1+5);
		 	sprintf(newStatus, "Connecting to %s:%d", host, port);
		 	std::cout << newStatus << std::endl;
			statusMessage->AddString("status", newStatus);
		 	MessageReceived(statusMessage);

			fSession = new Session(host, port, fOutputView);
			std::cout << "appel de Session::Connect" << std::endl;
			int connection = fSession->Connect(fOutputView);
			if (connection < 0) {
		 		newStatus = (char *)realloc(newStatus,strlen("Failed to connect to ") +strlen(host)+1+5);
		 		sprintf(newStatus, "Failed to connect to %s:%d", host, port);
		 		std::cout << newStatus << std::endl;
				statusMessage->ReplaceString("status", newStatus);
		 		MessageReceived(statusMessage);

		 		if (!(strcmp(host , "localhost")) && !fSession->IsLocalServerLaunched()) {

		 			BMessage* statusMessage = new BMessage(kStatusChange);
				 	char* newStatus = (char *)malloc(strlen("Launching proof server on port %d") +5);
				 	sprintf(newStatus, "Launching proof server on port %d", port);
				 	std::cout << newStatus << std::endl;
					statusMessage->AddString("status", newStatus);
				 	MessageReceived(statusMessage);

				 	
				 	fSession->LaunchLocalServer(port);
				 	BMessage* connectMessage = new BMessage(kConnect);
				 	connectMessage->AddString("host", host);
				 	connectMessage->AddUInt16("port", port);
				 	
				 	MessageReceived(connectMessage);
				 	
		 			}
		 	} else {
				newStatus = (char *)realloc(newStatus,strlen("Connected to ") +strlen(host)+1+5);
		 		sprintf(newStatus, "Connected to %s:%d", host, port);
		 		std::cout << newStatus << std::endl;
				statusMessage->ReplaceString("status", newStatus);
		 		MessageReceived(statusMessage);
		 	}
		 	break;
		}
		case (kStatusChange) : {
			//TODO : Get rid of fStatusText ?
			const char* newStatus = message->GetString("status");
			fStatusText->SetTo(newStatus);
			LockLooper();
		 		((BStringView*)FindView("status"))->SetText(fStatusText->String());
			UnlockLooper();
			break;
		}
		case(kEnvoi) : {
			fSession->Send(new BString(fInputView->Text()));
//			BString * receveid = fSession->Receive();
			break;
		}
	}
}

void SessionView::AttachedToWindow() {
	status_t status = fButton->SetTarget(this);
}

#endif	// _SESSIONVIEW_CPP_
