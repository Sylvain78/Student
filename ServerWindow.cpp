#ifndef _SERVERWINDOW_CPP_
#define _SERVERWINDOW_CPP_

#include <iostream>

#include "App.h"
#include "Constantes.h"
#include "ServerWindow.h"

#include <LayoutBuilder.h>

const BRect rect(150,150,500,200);

ServerWindow::ServerWindow(void) 
	:	BWindow(rect, "Serveur",B_TITLED_WINDOW,B_ASYNCHRONOUS_CONTROLS)
{
	fHost = new BTextControl("host", "host", new BMessage(kServerHost));
	fPort = new BTextControl("port", "port", new BMessage(kServerPort));
	
	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0)
			.SetInsets(5, 5, 5, 5)
			.Add(fHost)
			.Add(fPort)
			.End();
	Show();
}



void ServerWindow::MessageReceived(BMessage* message) {
	switch (message->what) {
		case(kServerHost) :
		 {
		 	std::cout << "host : " << fHost->Text() << std::endl;
		 	((App *)be_app)->GetSettings()->AddString("ServerHost", fHost->Text());
		 	break;
		 }
		 case(kServerPort) :
		 {
		 	std::cout << "host : " << fPort->Text() << std::endl;
		 	((App *)be_app)->GetSettings()->AddString("ServerPort", fPort->Text());
		 	break;
		 }
		default :
			BWindow::MessageReceived(message);
		}
	
}

#endif	// _SERVERWINDOW_CPP_
