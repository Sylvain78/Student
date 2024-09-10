#ifndef DEMONSTRATIONWINDOW_H
#define DEMONSTRATIONWINDOW_H

#include <Box.h>
#include <Button.h>
#include <Catalog.h>
#include <File.h>
#include <LayoutBuilder.h>
#include <ListView.h>
#include <MenuBar.h>
#include <Message.h>
#include <Path.h>
#include <RadioButton.h>
#include <Roster.h>
#include <String.h>
#include <StringView.h>
#include <TabView.h>
#include <TextControl.h>
#include <TextView.h>
#include <Window.h>

#include "Constantes.h"
#include "LatexListScrollView.h"
#include "LatexUtils.h"
#include "ServerWindow.h"
#include "SessionView.h"

#include "Constante.h"
#include "Relation.h"
#include "Theorie.h"

class DemonstrationWindow : public BWindow {
	
	typedef void (*for_each_hook)(Theorie* th);
    
	public :
		DemonstrationWindow();
		~DemonstrationWindow();
		virtual void MessageReceived(BMessage* message);
		bool QuitRequested();
		
		void SetServerWindow(ServerWindow* window);
		
	private :
		BString *fHost;
		int fPort;
		bool fLocal;
		ServerWindow*	fServerWindow;
		BMenuBar*		fMenuBar;
		BTabView*		fTabView_Demonstration;
		
		void ActiveButtons(bool activate);
};
#endif
 
