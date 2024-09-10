#ifndef DEMONSTRATIONWINDOW_H
#define DEMONSTRATIONWINDOW_H

#include <Box.h>
#include <Button.h>
#include <File.h>
#include <Path.h>
#include <Window.h>
#include <MenuBar.h>
#include <Message.h>
#include <TabView.h>
#include <ListView.h>
#include <StringView.h>
#include <LayoutBuilder.h>
#include <TabView.h>
#include <TextControl.h>
#include <TextView.h>
#include <Roster.h>
#include <RadioButton.h>
#include <Catalog.h>


#include "Constante.h"
#include "Constantes.h"
#include "LatexUtils.h"
#include "LatexListScrollView.h"
#include "ServerWindow.h"
#include "SessionView.h"

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
		ServerWindow*	fServerWindow;
		BMenuBar*		fMenuBar;
		BTabView*		fTabView_Demonstration;
		
		void ActiveButtons(bool activate);
};
#endif
 
