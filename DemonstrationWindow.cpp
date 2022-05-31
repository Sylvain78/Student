#include "DemonstrationWindow.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "DemonstrationWindow"

DemonstrationWindow::DemonstrationWindow() :
BWindow(BRect(100,100,500,500),"Démonstrations",B_TITLED_WINDOW,B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS)
{
	fMenuBar = new BMenuBar("menubar");
	
	fModeBox = new BBox("mode");
	fSpeedBox = new BBox("speed");
	fCompileBox = new BBox("compile");
	
	fDemonstration = new LatexListScrollView();
	
	BRadioButton* mode_prop = new BRadioButton("Prop", new BMessage('Prop'));
	BRadioButton* mode_first_order = new BRadioButton(B_TRANSLATE("First order"), new BMessage('Frst'));
	BRadioButton* speed_fast = new BRadioButton("Fast", new BMessage('Fast'));
	BRadioButton* speed_paranoid = new BRadioButton("Paranoid", new BMessage('Prnd'));
	BRadioButton* compile = new BRadioButton("Compile", new BMessage('Cmpl'));
	BRadioButton* interprete = new BRadioButton("Interprete", new BMessage('Intp'));
	
	BGroupLayout *modeBoxLayout = BLayoutBuilder::Group<>(B_HORIZONTAL)
	.SetInsets(2,0,2,0)
	.Add(mode_prop)
	.Add(mode_first_order);
	
	BGroupLayout *speedBoxLayout = BLayoutBuilder::Group<>(B_HORIZONTAL)
	.SetInsets(2,0,2,0)
	.Add(speed_fast)
	.Add(speed_paranoid);
	
	BGroupLayout *compileBoxLayout = BLayoutBuilder::Group<>(B_HORIZONTAL)
	.SetInsets(2,0,2,0)
	.Add(compile)
	.Add(interprete);
	
	fModeBox->SetLabel(new BStringView("Order label", "Order"));
	fModeBox->AddChild(modeBoxLayout->View());
	
	fSpeedBox->SetLabel(new BStringView("Speed label", "Speed"));
	fSpeedBox->AddChild(speedBoxLayout->View());

	fCompileBox->SetLabel(new BStringView("Compile label", "Compile"));
	fCompileBox->AddChild(compileBoxLayout->View());
	
	fTabView_Demonstration = new BTabView("Onglet Démonstrations");
	
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.AddGroup(B_HORIZONTAL)
		    .Add(fMenuBar)
			.Add(fTabView_Demonstration)
		.End();
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
