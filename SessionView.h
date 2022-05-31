#ifndef _SESSIONVIEW_H_
#define _SESSIONVIEW_H_

#include <Box.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <RadioButton.h>
#include <StringView.h>
#include <View.h>

#include "LatexListScrollView.h"

class SessionView : public BView {
	public:
		SessionView(const char* name);
	private:
		LatexListScrollView* fDemonstration;
		BBox*         fModeBox;
		BBox*		  fSpeedBox;
		BBox*		  fCompileBox;
	
};

#endif	// _SESSIONVIEW_H_
