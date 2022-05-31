#ifndef _SESSIONVIEW_CPP_
#define _SESSIONVIEW_CPP_

#include "SessionView.h"


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "SessionView"

SessionView::SessionView(const char * name) : BView(name, B_SUPPORTS_LAYOUT),
	fModeBox(new BBox("mode")),
	fSpeedBox(new BBox("speed")),
	fCompileBox(new BBox("compile")),
	fDemonstration(new LatexListScrollView()) {
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
			
		.AddGroup(B_HORIZONTAL)
	    	.Add(fModeBox)
		.End()
		.AddGroup(B_HORIZONTAL)
			.Add(fSpeedBox)
		.End()
		.AddGroup(B_HORIZONTAL)
			.Add(fCompileBox)
		.End()
	.Add(fDemonstration)
	.End();
	
	
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
}

#endif	// _SESSIONVIEW_CPP_
