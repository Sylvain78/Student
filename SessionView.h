#ifndef _SESSIONVIEW_H_
#define _SESSIONVIEW_H_

#include <Box.h>
#include <Button.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <RadioButton.h>
#include <StringView.h>
#include <View.h>

#include "LatexListScrollView.h"
#include "Session.h"

class SessionView : public BView {
	public:
		SessionView(const char* name);
		virtual	void MessageReceived(BMessage* message);
		
	private:
		LatexListScrollView* fDemonstration;
		BBox*         		 fModeBox;
		BBox*		 		 fSpeedBox;
		BBox*				 fCompileBox;
		BTextControl*		 fInputView;
		BString*			 fStatusText;
		BButton*			 fButton;

		Session*			 fSession;

};

#endif	// _SESSIONVIEW_H_
