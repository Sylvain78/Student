#include "MainWindow.h"
#include "SignatureWindow.h"

const BRect rect(100,100,500,400);

MainWindow::MainWindow(void)
	:	BWindow(rect, B_TRANSLATE_SYSTEM_NAME("Student"),B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	fMenuBar = new BMenuBar("menubar");
	
	BMenu *menu;
	BMenuItem* item;
	
	//Signatures
	menu = new BMenu("Signatures");
	item = new BMenuItem("New", new BMessage(kNewSignature), 'S');
	menu->AddItem(item);
	fMenuBar->AddItem(menu);
	
	//Théories
	menu = new BMenu("Théories");
	item = new BMenuItem("New", new BMessage(kNewTheory), 'N');
	menu->AddItem(item);
	fMenuBar->AddItem(menu);

	//Démonstrations
	menu = new BMenu("Démonstrations");
	item = new BMenuItem("New", new BMessage(kNouvelleDemonstration), 'D');
	menu->AddItem(item);
	fMenuBar->AddItem(menu);

	
	
	//Latex
	menu = new BMenu("LaTeX");
	item = new BMenuItem("Saisie", new BMessage(kLatex), 'L');
	menu->AddItem(item);
	fMenuBar->AddItem(menu);

	//A propos
	//TODO
	menu = new BMenu("A Propos");
	fMenuBar->AddItem(menu);
	menu->Invalidate();
	
	
	fButton = new BButton("Envoi", "Envoi", new BMessage(kEnvoi));
	fButton->SetTarget(be_app);
	fButton->MakeDefault(true);

	fInputView  = new BTextView("InputView");
	
	
	BRect viewFrame = Bounds();
	viewFrame.top = fMenuBar->Bounds().Height() + 1;
	viewFrame.right -=  B_V_SCROLL_BAR_WIDTH;
	viewFrame.left = 0;
	viewFrame.bottom -= B_H_SCROLL_BAR_HEIGHT;

	BRect textBounds = viewFrame;
	textBounds.OffsetTo(B_ORIGIN);
	textBounds.InsetBy(TEXT_INSET, TEXT_INSET);

	
	fOutputView = new BTextView(viewFrame, "OutputView", textBounds, B_FOLLOW_ALL, B_FRAME_EVENTS | B_WILL_DRAW);
	fScrollOutputView = new BScrollView("ScrollOutput",fOutputView, B_FOLLOW_ALL, 0, true, true);
	fScrollOutputView->SetViewColor(255,255,255);
	
	fOutputView->MakeEditable(false);


	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.AddGroup(B_VERTICAL)
			.SetInsets(5, 5, 5, 5)
			.Add(fScrollOutputView)
			.AddGroup(B_HORIZONTAL)
				.Add(fInputView)
				.Add(fButton)
				;
}

void
MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case (kLatex):
		{
			LWindow* lWindow = new LWindow();
			lWindow->Show();
			break;
		}
		
		case (kNewTheory):
		{
			TheorieWindow* theorieWindow = new TheorieWindow();
			theorieWindow->Show();
			break;
		}
		
		case (kNewSignature):
		{
			SignatureWindow* sigWindow = new SignatureWindow();
			sigWindow->Show();
			break;
		}
	
		case (kNouvelleDemonstration):
		{
			DemonstrationWindow* demonstrationWindow = new DemonstrationWindow();
			demonstrationWindow->Show();
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}


bool
MainWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

const char* MainWindow::GetInputText() {
	return fInputView->Text();
	}
