#ifndef _SERVERWINDOW_H_
#define _SERVERWINDOW_H_
#include <TextControl.h>
#include <Window.h>

class DemonstrationWindow;

class ServerWindow : public BWindow {

public:
		ServerWindow(DemonstrationWindow* target);
		
		void MessageReceived(BMessage* message);
		bool QuitRequested();
	private:
		DemonstrationWindow* fTarget;
		BTextControl *fHost;
		BTextControl* fPort;
};

#endif	// _SERVERWINDOW_H_
