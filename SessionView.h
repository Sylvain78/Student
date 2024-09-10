#ifndef _SESSIONVIEW_H_
#define _SESSIONVIEW_H_

#include <Box.h>
#include <Button.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <RadioButton.h>
#include <StringView.h>
#include <View.h>

class Session;

class SessionView : public BView {
	public:
		SessionView(const char* name);
		~SessionView();
		virtual void AttachedToWindow();
		virtual	void MessageReceived(BMessage* message);
		
	private:
		LatexListScrollView*	fDemonstration;
		BBox*			fModeBox;
		BBox*		 	fExpandNotationsBox;
		BBox*		 	fExpandCallsBox;
		BBox*			fCompileBox;
		BRadioButton* 		fMode_prop;
		BRadioButton* 		fMode_first_order;
		BRadioButton* 		fKeep_notations;
		BRadioButton* 		fExpand_notations;
		BRadioButton* 		fKeep_calls;
		BRadioButton* 		fExpand_calls;
		BRadioButton* 		fCompile;
		BRadioButton* 		fInterprete;
		BBox*			fDemonstrationBox;
		BTextView*		fInputView;
		BListView*		fOutputView;
		BString*		fStatusText;
		BButton*		fButton;

		Session*		fSession;

};

#endif	// _SESSIONVIEW_H_
