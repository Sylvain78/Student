#ifndef _SESSIONVIEW_CPP_
#define _SESSIONVIEW_CPP_

#include "SessionView.h"

void SessionView::SessionView(const char * name) : BView(name),
	fModeBox("mode"),
	fSpeedBox("speed"),
	fCompileBox("compile") {
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
}

#endif	// _SESSIONVIEW_CPP_
