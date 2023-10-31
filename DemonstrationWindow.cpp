#include "DemonstrationWindow.h"
#include<iostream>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "DemonstrationWindow"

DemonstrationWindow::DemonstrationWindow() :
	BWindow(BRect(100,100,800,700),"Démonstrations",B_TITLED_WINDOW,B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS),
	fServerWindow(NULL)
{
	fMenuBar = new BMenuBar("menubar");
	//Serveur
	BMenu* menu = new BMenu("Serveur");
	BMenuItem* item = new BMenuItem("New", new BMessage(kServerSettings), 'P');
	menu->AddItem(item);
	fMenuBar->AddItem(menu);

	fTabView_Demonstration = new BTabView("Onglet Démonstrations");

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		    .Add(fMenuBar)
			.Add(fTabView_Demonstration)
	.End();
	
	fTabView_Demonstration->AddTab(new SessionView("Session"));
	fTabView_Demonstration->AddTab(new SessionView("+"));
}

void DemonstrationWindow::MessageReceived(BMessage* message) 
{
	SessionView* sessionView = (SessionView*)fTabView_Demonstration->ViewForTab(fTabView_Demonstration->Selection());
	switch(message->what) {
		case (kServerSettings): {
			bool init = false;
			
			if (!fServerWindow) {
				init = true;
				fServerWindow = new ServerWindow();
				fServerWindow->AddToSubset(this);
			}

			fServerWindow->SetTarget(sessionView);
			fServerWindow->Show();
			if(init) {
				fServerWindow->Show();
			} else {
				fServerWindow->Activate();
			}
			fServerWindow->PostMessage(new BMessage(kLocalServer));
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
