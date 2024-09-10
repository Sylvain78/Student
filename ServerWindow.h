#ifndef _SERVERWINDOW_H_
#define _SERVERWINDOW_H_

#include <TextControl.h>
#include <Window.h>

#include "SessionView.h"

class DemonstrationWindow;

class ServerWindow : public BWindow {

public:
		ServerWindow(BString &host, int &port, bool &local);

		void MessageReceived(BMessage* message);
		bool QuitRequested();
		void SetTarget(SessionView *target);

	private:
		SessionView* fTarget;
		BRadioButton *fLocalButton, *fDistantButton;
		BTextControl *fHost, *fPort;
		BString &fHostDemonstration;
		int &fPortDemonstration;
		bool &fLocalDemonstration;
};

#endif	// _SERVERWINDOW_H_
