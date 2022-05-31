/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
 #include "App.h"
#include "LWindow.h"

const BRect rect(150,150,500,400);

LWindow::LWindow(void) 
	:	BWindow(rect, "Saisie Latex",B_TITLED_WINDOW,B_ASYNCHRONOUS_CONTROLS)
{
	LView *view = NULL;
	BString chaine = BString("\\[\\oint_{\\partial A}\\vec H\\;\\cdot\\mathrm{d}\\vec s=\\iint_A\\vec J\\;\\cdot\\mathrm{d}\\vec A+\\frac{\\mathrm{d}}{\\mathrm{d} t}\\iint_A\\vec D\\;\\cdot\\mathrm{d}\\vec A\\]");
	
	BGridView *grille = new BGridView(2,26);
	grille->GetLayout()->SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER,B_ALIGN_BOTTOM));

	for (int i=0; i < 26;i++) {
		BString s;
		s.Append((char)('a'+i), 1);
//		BBitmap* image = ((App*)be_app)->latexToPNG(s);
		view = new LView(s);

		grille->GridLayout()->AddView(view,i,1);
	}
	
	for (int i=0; i < 26;i++) {
		BString s;
		s.Append((char)('A'+i), 1);
//		BBitmap* image = ((App*)be_app)->latexToPNG(s);
		view = new LView(s);
		
		grille->GridLayout()->AddView(view,i,0);
	}
	view = new LView(chaine);

	BTextControl *saisie = new BTextControl("label","text", new BMessage(kLatexPreview));

	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.AddGroup(B_VERTICAL)
			.SetInsets(5, 5, 5, 5)
			.Add(grille)
			.Add(view)
			.AddGroup(B_HORIZONTAL)
			.Add(saisie);
	float height1, width1, height2, width2;
	view->GetPreferredSize(&width1,&height1);
	saisie->GetPreferredSize(&width2,&height2);
	ResizeTo(10+std::max(width1, width2),20+height1+height2);	
	Show();
}



void LWindow::MessageReceived(BMessage* message) {
	switch (message->what) {
		case(kLatexPreview): {
			BTextControl *textControl = NULL;
			message->FindPointer("source", (void **)&textControl);
			
			BString texte = BString(textControl->Text());
			BWindow *preview = new BWindow(BRect(50,50,200,200), "preview", B_FLOATING_WINDOW, B_ASYNCHRONOUS_CONTROLS);
			
			LView *view = new LView(texte);
			preview->AddChild(view);
			float width;
			float height;
			view->GetPreferredSize(&width, &height);
			preview->ResizeTo(width,height);
			preview->Show();
			break;
		}
		default :
			BWindow::MessageReceived(message);
		}
	
}
