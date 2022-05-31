#include "DemonstrationWindow.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "DemonstrationWindow"

DemonstrationWindow::DemonstrationWindow() :
BWindow(BRect(100,100,500,500),"Démonstrations",B_TITLED_WINDOW,B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS)
{
	fMenuBar = new BMenuBar("menubar");

	fTabView_Demonstration = new BTabView("Onglet Démonstrations");
	
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.AddGroup(B_HORIZONTAL)
		    .Add(fMenuBar)
			.Add(fTabView_Demonstration)
		.End();
		
	fTabView_Demonstration->AddTab(new SessionView("+"));
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
