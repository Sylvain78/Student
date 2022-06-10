#include "DemonstrationWindow.h"
#include<iostream>
#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "DemonstrationWindow"

DemonstrationWindow::DemonstrationWindow() :
	BWindow(BRect(100,100,500,500),"Démonstrations",B_TITLED_WINDOW,B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS),
	fServerWindow(NULL)
{
	fMenuBar = new BMenuBar("menubar");
	//Serveur
	BMenu* menu = new BMenu("Serveur");
	BMenuItem* item = new BMenuItem("New", new BMessage(kServerSettings), 'P');
	menu->AddItem(item);
	fMenuBar->AddItem(menu);

	fTabView_Demonstration = new BTabView("Onglet Démonstrations");
	

	BTextView* fOutputView = new BTextView(BRect(), "OutputView", BRect(), B_FOLLOW_ALL, B_FRAME_EVENTS | B_WILL_DRAW);
	BScrollView* fScrollOutputView = new BScrollView("ScrollOutput",fOutputView, B_FOLLOW_ALL, 0, true, true);
	fScrollOutputView->SetViewColor(255,255,255);
	 
	fOutputView->MakeEditable(false);
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		    .Add(fMenuBar)
			.Add(fTabView_Demonstration)
			.Add(fScrollOutputView)
	.End();
		
	fTabView_Demonstration->AddTab(new SessionView("Session"));
	fTabView_Demonstration->AddTab(new SessionView("+"));
	fTabView_Demonstration->AddTab(new SessionView("++"));
}

void DemonstrationWindow::MessageReceived(BMessage* message) 
{
	switch(message->what) {
		case (kServerSettings):
			{
				SessionView* sessionView = (SessionView*)fTabView_Demonstration->ViewForTab(fTabView_Demonstration->Selection());
				
				if (!fServerWindow)
					fServerWindow = new ServerWindow();
				
				fServerWindow->SetTarget(sessionView);
				fServerWindow->Activate();
				break;
			}
		case (kServerHost) :
		 {
		 	SessionView* view = (SessionView*)fTabView_Demonstration->ViewForTab(fTabView_Demonstration->Selection());
		 	break;
		 }
		 case(kServerPort) :
		 {
		 	SessionView* view = (SessionView*)fTabView_Demonstration->ViewForTab(fTabView_Demonstration->Selection());
		 	break;
		 }
		 case (kConnect) :
		 {
		 	const char* host = message->GetString("host", "");
		 	uint16 port = message->GetUInt16("port",0);
		 	std::cout << "connect to " << host << ":" << port << std::endl;
		 	
		 	BMessage* statusMessage = new BMessage(kStatusChange);
		 	char* newStatus = (char *)malloc(strlen("connecting to ") +strlen(host)+1+5);
		 	sprintf(newStatus, "Connecting to %s:%d", host, port);
		 	std::cout << newStatus << std::endl;

		 	statusMessage->AddString("status", newStatus);
		 	
		 	PostMessage(statusMessage);
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
	//TODO
	return true;
}

void DemonstrationWindow::SetServerWindow(ServerWindow* window) {
	fServerWindow = window;
}
