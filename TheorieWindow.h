#ifndef THEORIEWINDOW_H
#define THEORIEWINDOW_H

#include <Application.h>
#include <Button.h>
#include <ClassInfo.h>
#include <Directory.h>
#include <File.h>
#include <LayoutBuilder.h>
#include <ListView.h>
#include <MenuBar.h>
#include <Message.h>
#include <Path.h>
#include <Roster.h>
#include <StringView.h>
#include <TabView.h>
#include <TextControl.h>
#include <Window.h>

#include "Constantes.h"
#include "LatexUtils.h"
#include "LatexListItem.h"
#include "LatexListScrollView.h"
#include "Schema.h"
#include "Theorie.h"

class TheorieWindow : public BWindow {
	
	typedef void (*for_each_hook)(Theorie* th);
    
	public :
		TheorieWindow();
		virtual void MessageReceived(BMessage* message);
		bool QuitRequested();
	
	private :
		BMenuBar* fMenuBar;
		BTextControl* fNomTheorie;

		//Axiomes
		BTextControl* fNomAxiome;
		BTextControl* fAxiome;
		BButton* fAjoutAxiomeButton;
		BButton* fSupprimeAxiomeButton;

		//Schémas
		BTextControl* fNomSchema;
		BTextControl* fAriteSchema;
		BButton* fAjoutSchemaButton;
		BButton* fSupprimeSchemaButton;
		
		LatexListScrollView* fListeTheories_ListView;
		LatexListScrollView* fSignatures_ListView;
		
		BView*     fLangageView;
		BListView* fAxiomes_ListView;
		BListView* fSchemas_ListView;
		
		BGroupView* fLangage_GroupView;
		BGroupView* fAxiomes_GroupView;
		BGroupView* fSchemas_GroupView;
		
		BTabView* fTabView_Theorie;
		
		void ActiveButtons(bool activate);
	
};
#endif
