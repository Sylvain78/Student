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

class Session;

class SessionView : public BView {
	public:
		SessionView(const char* name);
		virtual void AttachedToWindow();
		virtual	void MessageReceived(BMessage* message);
		
	private:
		LatexListScrollView* fDemonstration;
		BBox*         		 fModeBox;
		BBox*		 		 fSpeedBox;
		BBox*				 fCompileBox;
		BRadioButton* 		 fMode_prop;
		BRadioButton* 		 fMode_first_order;
		BRadioButton* 		 fSpeed_fast;
		BRadioButton* 		 fSpeed_paranoid;
		BRadioButton* 		 fCompile;
		BRadioButton* 		 fInterprete;
		BBox*				 fDemonstrationBox;
		BTextView*			 fInputView;
		BTextView*			 fOutputView;
		BString*			 fStatusText;
		BButton*			 fButton;

		Session*			 fSession;

};

#endif	// _SESSIONVIEW_H_
