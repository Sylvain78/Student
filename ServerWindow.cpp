#ifndef _SERVERWINDOW_CPP_
#define _SERVERWINDOW_CPP_

#include <iostream>

#include "App.h"
#include "Constantes.h"
#include "ServerWindow.h"

#include <LayoutBuilder.h>

const BRect rect(150,150,150,150);

ServerWindow::ServerWindow() 
	:	BWindow(rect, "Serveur",B_TITLED_WINDOW_LOOK,B_FLOATING_SUBSET_WINDOW_FEEL, B_NOT_RESIZABLE | B_AUTO_UPDATE_SIZE_LIMITS)
{
	fHost = new BTextControl("host", "host / IP", new BMessage(kServerHost));
	fPort = new BTextControl("port", "5757", new BMessage(kServerPort));
	
	BRadioButton* localButton = new BRadioButton("local", "Local", new BMessage(kLocalServer));
	BRadioButton* distantButton = new BRadioButton("distant", "Distant", new BMessage(kDistantServer));
	BButton *button = new BButton("Connect", new BMessage(kConnect));
	
	button->MakeDefault(true);
	localButton->SetValue(1);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL)
	.SetInsets(B_USE_WINDOW_SPACING)
		.AddGroup(B_HORIZONTAL, B_USE_DEFAULT_SPACING, 1.0)
			.Add(localButton)
			.Add(distantButton)
			.AddGlue()
		.End()
		.AddGroup(B_HORIZONTAL, B_USE_DEFAULT_SPACING)
			.Add(fHost)
			.Add(fPort)
		.End()
		.AddGroup(B_HORIZONTAL, B_USE_DEFAULT_SPACING)
			.Add(button)
			.AddGlue()
		.End()
	.End();
	ResizeToPreferred();
	Show();
	PostMessage(new BMessage(kLocalServer));
}



void ServerWindow::MessageReceived(BMessage* message) {
	switch (message->what) {
		
		 case(kLocalServer) :
		 {
		 	((BRadioButton*)FindView("local"))->SetValue(1);
		 	fHost->SetEnabled(false);
		 	fPort->MakeFocus(true);
		 	break;
		 }
		 case(kDistantServer) :
		 {
		 	((BRadioButton*)FindView("distant"))->SetValue(1);
		 	fHost->SetEnabled(true);
		 	fHost->MakeFocus(true);
		 	break;
		 }
		 case(kConnect) :
		 {
		 	const char* portText = fPort->Text();
		 	char *endPtr;
		 	uint16 port = strtol(fPort->Text(), &endPtr,10);
		 	if (errno == ERANGE || *endPtr != '\0' || portText == endPtr) {
    			std::string buf("Invalid number : ");
				buf.append(fPort->Text());
    			BAlert* alert = new BAlert("", buf.c_str(), "Ok", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
    			alert->Go();
    		} else {
    			message->AddString("host", fHost->Text());
			 	message->AddUInt16("port", port);
			 	fTarget->MessageReceived(message);
			 	PostMessage(B_QUIT_REQUESTED);
    		}
    	}
		default :
			BWindow::MessageReceived(message);
		}
}

void ServerWindow::SetTarget(SessionView* target) {
	fTarget = target;
	target->Window()->AddToSubset(this);
}

bool ServerWindow::QuitRequested() {
	return true;
	}
#endif	// _SERVERWINDOW_CPP_
