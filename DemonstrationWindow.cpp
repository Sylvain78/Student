#include "DemonstrationWindow.h"
#include<iostream>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "DemonstrationWindow"

DemonstrationWindow::DemonstrationWindow() :
	BWindow(BRect(100,100,800,700),"Démonstrations",B_TITLED_WINDOW,B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS),
	fHost(new BString("")),
	fPort(0),
	fServerWindow(NULL)
{
	fMenuBar = new BMenuBar("menubar");
	//Serveur
	BMenu* menu = new BMenu("Serveur");
	BMenuItem* item = new BMenuItem("New", new BMessage(kServerSettings), 'P');
	menu->AddItem(item);
	fMenuBar->AddItem(menu);

	fSessionView = new SessionView("Session");
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		    .Add(fMenuBar)
			.Add(fSessionView)
	.End();
}

DemonstrationWindow::~DemonstrationWindow() {
	}
	
void DemonstrationWindow::MessageReceived(BMessage* message) 
{
	switch(message->what) {
		case (kServerSettings): {
			bool init = false;
			
			if (!fServerWindow) {
				init = true;
			fServerWindow = new ServerWindow(*fHost, fPort, fLocal);
			fServerWindow->AddToSubset(this);
			}

			fServerWindow->SetTarget(fSessionView);
			fServerWindow->Show();
			if(init) {
				fServerWindow->Show();
			} else {
				fServerWindow->Activate();
			}
			if (fLocal)
				fServerWindow->PostMessage(new BMessage(kLocalServer));
			else
				fServerWindow->PostMessage(new BMessage(kDistantServer));
			break;
		}
		default:
			BWindow::MessageReceived(message);
	}
}

void DemonstrationWindow::ActiveButtons(bool activate) {
	
}

bool DemonstrationWindow::QuitRequested() 
{
	//TODO wait for receive thread
	return true;
}

void DemonstrationWindow::SetServerWindow(ServerWindow* window) {
	fServerWindow = window;
}
