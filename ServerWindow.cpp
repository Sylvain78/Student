#ifndef _SERVERWINDOW_CPP_
#define _SERVERWINDOW_CPP_

#include <iostream>

#include "App.h"
#include "Constantes.h"
#include "ServerWindow.h"

#include <LayoutBuilder.h>

const BRect rect(150,150,150,150);

ServerWindow::ServerWindow(BWindow* target) 
	:	BWindow(rect, "Serveur",B_TITLED_WINDOW_LOOK,B_FLOATING_SUBSET_WINDOW_FEEL, B_NOT_RESIZABLE | B_AUTO_UPDATE_SIZE_LIMITS),
	fTarget(target)
{
	fHost = new BTextControl("host", "host / IP", new BMessage(kServerHost));
	fPort = new BTextControl("port", "5757", new BMessage(kServerPort));
	
	fHost->SetTarget(target);
	fPort->SetTarget(target);
	
	status_t status = this->AddToSubset(target);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
	.SetInsets(B_USE_WINDOW_SPACING)
		.AddGroup(B_HORIZONTAL, B_USE_DEFAULT_SPACING, 1.0)
			.Add(new BRadioButton("local", new BMessage(kLocalServer)))
			.Add(new BRadioButton("distant", new BMessage(kDistantServer)))
			.AddGlue()
		.End()
		.AddGroup(B_HORIZONTAL, B_USE_DEFAULT_SPACING)
			.Add(fHost)
			.Add(fPort)
		.End()
	.End();
	ResizeToPreferred();
	Show();
}



void ServerWindow::MessageReceived(BMessage* message) {
	switch (message->what) {
		
		 case(kLocalServer) :
		 {
		 	fHost->SetEnabled(false);
		 	break;
		 }
		 case(kDistantServer) :
		 {
		 	fHost->SetEnabled(true);
		 	break;
		 }
		default :
			BWindow::MessageReceived(message);
		}
	
}

#endif	// _SERVERWINDOW_CPP_
