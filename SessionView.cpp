#ifndef _SESSIONVIEW_CPP_
#define _SESSIONVIEW_CPP_

#include <iostream>

#include "Constantes.h"
#include "Session.h"
#include "SessionView.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "SessionView"


SessionView::SessionView(const char * name) : BView(name, B_SUPPORTS_LAYOUT),
	fDemonstration(new LatexListScrollView()),
	fModeBox(new BBox("mode")),
	fExpandNotationsBox(new BBox("notations")),
	fExpandCallsBox(new BBox("calls")),
	fCompileBox(new BBox("compile")),
	fDemonstrationBox(new BBox("demonstration")),
	fStatusText(new BString("Not connected")) ,
	fSession(NULL)
	{

	fInputView  = new BTextView("InputView");
	BScrollView* fScrollInputView = new BScrollView("ScrollInput",fInputView, B_WILL_DRAW | B_FRAME_EVENTS, false,
		true, B_FANCY_BORDER);
	fScrollInputView->SetViewColor(255,255,255);

	fButton = new BButton("Envoi", "Envoi", new BMessage(kEnvoi));

	BStringView* statusView = new BStringView("status",fStatusText->String());
	
	
	LatexListScrollView* fScrollOutputView = new LatexListScrollView();
	fScrollOutputView->SetViewColor(255,255,255);
	fOutputView = fScrollOutputView->GetView();

	BLayoutBuilder::Group<>(this, B_VERTICAL)
	.SetInsets(B_USE_WINDOW_INSETS)
	.AddSplit(B_VERTICAL)
		.AddGroup(B_VERTICAL)
			.AddGroup(B_HORIZONTAL)
	    		.Add(fModeBox)
				.Add(fExpandNotationsBox)
				.Add(fExpandCallsBox)
				.Add(fCompileBox)
			.End()
			.AddGroup(B_HORIZONTAL)
				.SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNLIMITED))
				.SetInsets(5,5,5,5)
				.Add(fScrollInputView)
				.Add(fButton)
				.Add(fDemonstrationBox)
			.End()
		.End()
		.AddGroup(B_VERTICAL)
			.Add(fScrollOutputView)
		.End()
	.End()
	//TODO font & color
	.Add(statusView, 0.1f);

	fMode_prop = new BRadioButton("Prop", new BMessage(kModeProp));
	fMode_first_order = new BRadioButton(B_TRANSLATE("First order"), new BMessage(kModeFirstOrder));
	fKeep_notations = new BRadioButton("Keep", new BMessage(kKeepNotations));
	fExpand_notations = new BRadioButton("Expand", new BMessage(kExpandNotations));
	fKeep_calls = new BRadioButton("Keep", new BMessage(kKeepCalls));
	fExpand_calls = new BRadioButton("Expand", new BMessage(kExpandCalls));
	fCompile = new BRadioButton("Compile", new BMessage(kCompile));
	fInterprete = new BRadioButton("Interprete", new BMessage(kInterprete));

	fMode_prop->SetEnabled(false);
	fMode_first_order->SetEnabled(false);
	fKeep_notations->SetEnabled(false);
	fExpand_notations->SetEnabled(false);
	fKeep_calls->SetEnabled(false);
	fExpand_calls->SetEnabled(false);
	fCompile->SetEnabled(false);
	fInterprete->SetEnabled(false);
	fButton->SetEnabled(false);
	
	BGroupLayout *modeBoxLayout = BLayoutBuilder::Group<>(B_HORIZONTAL)
	.SetInsets(2,0,2,0)
	.Add(fMode_prop)
	.Add(fMode_first_order);

	BGroupLayout *notationsBoxLayout = BLayoutBuilder::Group<>(B_HORIZONTAL)
	.SetInsets(2,0,2,0)
	.Add(fKeep_notations)
	.Add(fExpand_notations);

	BGroupLayout *callsBoxLayout = BLayoutBuilder::Group<>(B_HORIZONTAL)
	.SetInsets(2,0,2,0)
	.Add(fKeep_calls)
	.Add(fExpand_calls);

	BGroupLayout *compileBoxLayout = BLayoutBuilder::Group<>(B_HORIZONTAL)
	.SetInsets(2,0,2,0)
	.Add(fCompile)
	.Add(fInterprete);

	fModeBox->SetLabel(new BStringView("Order label", "Order"));
	fModeBox->AddChild(modeBoxLayout->View());

	fExpandNotationsBox->SetLabel(new BStringView("Expand notations label", "Expand notations"));
	fExpandNotationsBox->AddChild(notationsBoxLayout->View());

	fExpandCallsBox->SetLabel(new BStringView("Expand calls label", "Expand calls"));
	fExpandCallsBox->AddChild(callsBoxLayout->View());

	fCompileBox->SetLabel(new BStringView("Compile label", "Compile"));
	fCompileBox->AddChild(compileBoxLayout->View());
	
	fDemonstrationBox->SetLabel(new BStringView("Demonstration label", "Demonstration"));
	fDemonstrationBox->AddChild(fDemonstration);
}

SessionView::~SessionView(){
	delete fStatusText;
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
			int connection = fSession->Connect();
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
		 		LockLooper();
					fMode_prop->SetEnabled(true);
					fMode_first_order->SetEnabled(true);
					fKeep_notations->SetEnabled(true);
					fExpand_notations->SetEnabled(true);
					fKeep_calls->SetEnabled(true);
					fExpand_calls->SetEnabled(true);
					fCompile->SetEnabled(true);
					fInterprete->SetEnabled(true);
					fButton->SetEnabled(true);
					fInputView->MakeFocus(true);
				UnlockLooper();

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
			break;
		}
		case(kModeProp) : {
			fSession->Send(new BString("Prop"));
			break;
		}
		case(kModeFirstOrder) : {
			fSession->Send(new BString("First_order"));
			break;
		}
	}
}

void SessionView::AttachedToWindow() {
	fButton->SetTarget(this);
	fMode_prop->SetTarget(this);
	fMode_first_order->SetTarget(this);
	fKeep_notations->SetTarget(this);
	fExpand_notations->SetTarget(this);
	fCompile->SetTarget(this);
	fInterprete->SetTarget(this);

}

#endif	// _SESSIONVIEW_CPP_
