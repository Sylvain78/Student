#include "DemonstrationWindow.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "DemonstrationWindow"

DemonstrationWindow::DemonstrationWindow() :
BWindow(BRect(100,100,500,500),"Démonstrations",B_TITLED_WINDOW,B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS)
{
	fMenuBar = new BMenuBar("menubar");

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
//TODO
}

void DemonstrationWindow::ActiveButtons(bool activate) {
	
}

bool DemonstrationWindow::QuitRequested() 
{
	//TODO
	return true;
}
