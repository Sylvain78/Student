#ifndef SignatureWindow_H
#define SignatureWindow_H

#include <Application.h>
#include <Button.h>
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

#include "App.h"
#include "Constante.h"
#include "Constantes.h"
#include "LatexUtils.h"
#include "LatexListItem.h"
#include "LatexListScrollView.h"
#include "Operation.h"
#include "Relation.h"
#include "Signature.h"

class SignatureWindow : public BWindow {
	
	typedef void (*for_each_hook)(Signature* th);
    
	public :
		SignatureWindow();
		virtual void MessageReceived(BMessage* message);
		bool QuitRequested();
	
	private :
		BMenuBar* fMenuBar;
		BTextControl* fNomSignature;
		BTextControl* fNomConstante;
		BTextControl* fNomOperation;
		BTextControl* fAriteOperation;
		BTextControl* fNomRelation;
		BTextControl* fAriteRelation;
		BButton* 		fAjoutConstanteButton;
		BButton* 		fSupprimeConstanteButton;
		BButton* 		fAjoutRelationButton;
		BButton* 		fSupprimeRelationButton;
		BButton* 		fAjoutOperationButton;
		BButton* 		fSupprimeOperationButton;
		LatexListScrollView*	fListeSignatures_ListView;
		BListView* fConstantes_ListView;
		BListView* fOperations_ListView;
		BListView* fRelations_ListView;
		BGroupView* fConstantes_GroupView;
		BGroupView* fOperations_GroupView;
		BGroupView* fRelations_GroupView;
		BTabView* fTabView_Signature;
		
		void ActiveButtons(bool activate) const;
		void DesactivateLatexViews() const;
	
};
#endif
