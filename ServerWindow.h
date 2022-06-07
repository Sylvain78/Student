#ifndef _SERVERWINDOW_H_
#define _SERVERWINDOW_H_
#include <TextControl.h>
#include <Window.h>

class ServerWindow : public BWindow {

public:
		ServerWindow(BLooper* target);
		
		void MessageReceived(BMessage* message);
	private:
		BLooper* fTarget;
		BTextControl *fHost;
		BTextControl* fPort;
};

#endif	// _SERVERWINDOW_H_
