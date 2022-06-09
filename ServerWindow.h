#ifndef _SERVERWINDOW_H_
#define _SERVERWINDOW_H_

#include <TextControl.h>
#include <Window.h>

#include "SessionView.h"

class DemonstrationWindow;

class ServerWindow : public BWindow {

public:
		ServerWindow();

		void MessageReceived(BMessage* message);
		bool QuitRequested();
		void SetTarget(SessionView *target);

	private:
		SessionView* fTarget;
		BTextControl *fHost;
		BTextControl* fPort;
};

#endif	// _SERVERWINDOW_H_
