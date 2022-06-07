#include "DemonstrationWindow.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "DemonstrationWindow"

DemonstrationWindow::DemonstrationWindow() :
BWindow(BRect(100,100,500,500),"Démonstrations",B_TITLED_WINDOW,B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS)
{
	fMenuBar = new BMenuBar("menubar");
	//Serveur
	BMenu* menu = new BMenu("Serveur");
	BMenuItem* item = new BMenuItem("New", new BMessage(kServerSettings), 'P');
	menu->AddItem(item);
	fMenuBar->AddItem(menu);

	fTabView_Demonstration = new BTabView("Onglet Démonstrations");
	fInputView  = new BTextControl("InputView",0,0,0);
	fButton = new BButton("Envoi", "Envoi", new BMessage(kEnvoi));
	fInputView->MakeFocus(true);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		    .Add(fMenuBar)
			.Add(fTabView_Demonstration)
		.AddGroup(B_HORIZONTAL)
			.SetInsets(5,5,5,5)
			.Add(fInputView)
			.Add(fButton)
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
				ServerWindow* serverWindow = new ServerWindow(this);
				serverWindow->Show();
				break;
			}
		case(kServerHost) :
		 {
		 	SessionView* view = (SessionView*)fTabView_Demonstration->ViewForTab(fTabView_Demonstration->Selection());
		 	break;
		 }
		 case(kServerPort) :
		 {
		 	SessionView* view = (SessionView*)fTabView_Demonstration->ViewForTab(fTabView_Demonstration->Selection());
		 	break;
		 }
	}
}

void DemonstrationWindow::ActiveButtons(bool activate) {
	
}

bool DemonstrationWindow::QuitRequested() 
{
	//TODO
	return true;
}
