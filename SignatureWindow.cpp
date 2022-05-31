#include "SignatureWindow.h"
#include "LatexListScrollView.h"
#include<iostream>

SignatureWindow::SignatureWindow() : 
	BWindow(BRect(100, 100, 500, 500),"Signatures",B_TITLED_WINDOW,B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS)
{
	fMenuBar = new BMenuBar("menubar");
	
	//fListeSignatures = new BList();
	fListeSignatures_ListView = new LatexListScrollView();
	
	fTabView_Signature = new BTabView("SignaturesView");
	{
		((App *)be_app)->GetSignatures()->DoForEach(&addToLatexListScrollView, (void *)fListeSignatures_ListView);
	}
	fListeSignatures_ListView->Invalidate();
	fListeSignatures_ListView->ResizeToPreferred();//TODO Faire retailler la fenêtre
	fListeSignatures_ListView->SetDrawingMode(B_OP_ALPHA);
	fListeSignatures_ListView->GetView()->SetSelectionMessage(new BMessage(kSelectionSignature));
	
	
	fNomSignature = new BTextControl("Nom","", new BMessage(kNomSignatureChange));
	fNomSignature->SetTarget(this);
	fNomSignature->SetEnabled(false);
	
	//Constantes
	fConstantes_ListView = new BListView();
	fConstantes_ListView->SetDrawingMode(B_OP_ALPHA);
	fConstantes_ListView->SetSelectionMessage(new BMessage(kSelectionConstante));
	fConstantes_GroupView = new BGroupView();
		
	fNomConstante = new BTextControl("Nom","", new BMessage(kNomConstanteChange));
	fNomConstante->SetTarget(this);
//	fNomConstante->SetEnabled(false);
	
	fAjoutConstanteButton = new BButton("+", new BMessage(kNewConstante));
//	fAjoutConstanteButton->SetEnabled(false);
	fSupprimeConstanteButton = new BButton("-", new BMessage(kDeleteConstante));
//	fSupprimeConstanteButton->SetEnabled(false);

	//Opérations
	fOperations_ListView = new BListView();
	fOperations_ListView->SetDrawingMode(B_OP_ALPHA);
	fOperations_ListView->SetSelectionMessage(new BMessage(kSelectionOperation));
	fOperations_GroupView = new BGroupView();
		
	fNomOperation = new BTextControl("Nom","", new BMessage(kNomOperationChange));
	fNomOperation->SetTarget(this);
//	fNomOperation->SetEnabled(false);
	
	fAriteOperation = new BTextControl("Arité","", new BMessage(kAriteOperationChange));
	fAriteOperation->SetTarget(this);
//	fAriteOperation->SetEnabled(false);
	
	fAjoutOperationButton = new BButton("+", new BMessage(kNewOperation));
//	fAjoutOperationButton->SetEnabled(false);
	fSupprimeOperationButton = new BButton("-", new BMessage(kDeleteOperation));
//	fSupprimeOperationButton->SetEnabled(false);

	//Relations
	fRelations_ListView = new BListView();
	fRelations_ListView->SetDrawingMode(B_OP_ALPHA);
	fRelations_ListView->SetSelectionMessage(new BMessage(kSelectionRelation));
	fRelations_GroupView = new BGroupView();
		
	fNomRelation = new BTextControl("Nom","", new BMessage(kNomRelationChange));
	fNomRelation->SetTarget(this);
//	fNomRelation->SetEnabled(false);
	
	fAriteRelation = new BTextControl("Arité","", new BMessage(kAriteRelationChange));
	fAriteRelation->SetTarget(this);
//	fAriteRelation->SetEnabled(false);
	
	fAjoutRelationButton = new BButton("+", new BMessage(kNewRelation));
//	fAjoutRelationButton->SetEnabled(false);
	fSupprimeRelationButton = new BButton("-", new BMessage(kDeleteRelation));
//	fSupprimeRelationButton->SetEnabled(false);

	ActiveButtons(false);
	
	BLayoutBuilder::Group<>(fConstantes_GroupView)
    	.SetInsets(0, 0, 0, 0)
	    .Add(fConstantes_ListView)
    	.AddGroup(B_VERTICAL)
    		.AddGroup(B_HORIZONTAL)
	        	.Add(fNomConstante)
	        	.Add(fAjoutConstanteButton)
	        .End()
	        
    	.AddGroup(B_VERTICAL)
    	    .Add(fSupprimeConstanteButton)
    	;
	
	BLayoutBuilder::Group<>(fRelations_GroupView)
    	.SetInsets(0, 0, 0, 0)
	    .Add(fRelations_ListView)
    	.AddGroup(B_VERTICAL)
    		.AddGroup(B_HORIZONTAL)
	        	.Add(fNomRelation)
	        	.Add(fAriteRelation)
	        	.Add(fAjoutRelationButton)
	        .End()
	    .End()    
    	.AddGroup(B_VERTICAL)
    	    .Add(fSupprimeRelationButton)
		.End()
    	;

	BLayoutBuilder::Group<>(fOperations_GroupView)
    	.SetInsets(0, 0, 0, 0)
	    .Add(fOperations_ListView)
    	.AddGroup(B_VERTICAL)
    		.AddGroup(B_HORIZONTAL)
	        	.Add(fNomOperation)
	        	.Add(fAriteOperation)
	        	.Add(fAjoutOperationButton)
	        .End()
	    .End()
    	.AddGroup(B_VERTICAL)
    	    .Add(fSupprimeOperationButton)
    	.End()
    	;
    	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.AddGroup(B_VERTICAL)
			.SetInsets(B_USE_WINDOW_INSETS)
			.AddSplit(B_HORIZONTAL)
				.Add(fListeSignatures_ListView)
				.Add(fTabView_Signature)
			.End()
		.End()
			;
	
	BMenu* menu = new BMenu("Signature");
	menu->AddItem(new BMenuItem("Nouvelle", new BMessage(kNewSignature), 'N'));
	menu->AddItem(new BMenuItem("Supprimer", new BMessage(kDeleteSignature), 'S'));
	fMenuBar->AddItem(menu);

	//fTabView_Signature = new BTabView("SignaturesView");
	
	BTab* nom        = new BTab();
	BTab* constantes = new BTab();
	BTab* operations = new BTab();
	BTab* relations  = new BTab();

	
	fTabView_Signature->AddTab(fNomSignature,nom);
	fTabView_Signature->AddTab(fConstantes_GroupView, constantes);
	fTabView_Signature->AddTab(fOperations_GroupView, operations);	
	fTabView_Signature->AddTab(fRelations_GroupView, relations);	
	
	nom->SetLabel("Nom");
	constantes->SetLabel("Constantes");
	operations->SetLabel("Opérations");
	relations->SetLabel("Relations");

	

			
}

void SignatureWindow::MessageReceived(BMessage* message) 
{

	switch(message->what)
	{
		//Signatures
		
		case(kNewSignature) : 
		{
			BString *nom = new BString("Signature");
			Signature* sig = new Signature(nom);

			LatexListItem* lisig = new LatexListItem(sig);
			fListeSignatures_ListView->LockLooper();
				fListeSignatures_ListView->GetView()->AddItem(lisig);
				fListeSignatures_ListView->Invalidate();
				fListeSignatures_ListView->ResizeToPreferred();
			fListeSignatures_ListView->UnlockLooper();
			break;
		}
		
		case(kDeleteSignature) :
		{
			int selected = fListeSignatures_ListView->GetView()->CurrentSelection(0);
			if ( selected >= 0) {
				LatexListItem* latexItem = dynamic_cast<LatexListItem *>(fListeSignatures_ListView->GetView()->ItemAt(selected));
				Signature* itemSig = (Signature *)latexItem->GetLView();
				BEntry *entry = new BEntry(itemSig->GetOCamlChemin()->Path());
				entry->Remove();
				fListeSignatures_ListView->GetView()->RemoveItem(selected);
					//TODO test return status
			} 
			break;
		}
		
		case(kSelectionSignature) :
		{
			int selected = fListeSignatures_ListView->GetView()->CurrentSelection(0);
			if ( selected < 0) {
				fNomSignature->SetEnabled(false);
				fNomSignature->SetText("");
				ActiveButtons(false);
				DesactivateLatexViews();
			} 
			else {
				ActiveButtons(true);
				LatexListItem* latexItem = dynamic_cast<LatexListItem *>(fListeSignatures_ListView->GetView()->ItemAt(selected));
				Signature* itemSig = (Signature *)latexItem->GetLView();
				
				fNomSignature->SetEnabled(true);
				fNomSignature->SetText(itemSig->GetLatex()->String());

				//Constantes
				fConstantes_ListView->LockLooper();
					//vidage de la liste
					int nbItems = fConstantes_ListView->CountItems(); 
					for ( int32 i = 0; i < nbItems; i++ )
   						fConstantes_ListView->RemoveItem((int32)0);
					//rajout des éléments
					for(int i = 0; i < itemSig->GetConstanteList()->CountItems(); i++) {
						
						Constante *constante = (Constante *)(itemSig->GetConstanteList()->ItemAt(i));
						LatexListItem *li_constante = new LatexListItem((LView *)constante);
						fConstantes_ListView->AddItem(li_constante);
					}
				fConstantes_ListView->Invalidate();
				fConstantes_ListView->UnlockLooper();
				
				//Operations
				fOperations_ListView->LockLooper();
					//vidage de la liste
					nbItems = fOperations_ListView->CountItems();
					for ( int32 i = 0; i <  nbItems; i++ )
   						fOperations_ListView->RemoveItem((int32)0);
					//rajout des éléments
					for(int i = 0; i < itemSig->GetOperationList()->CountItems(); i++) {
						Operation *operation = (Operation *)itemSig->GetOperationList()->ItemAt(i);
						LatexListItem *li_operation = new LatexListItem((LView *)operation);
						fOperations_ListView->AddItem(li_operation);
					}
				fOperations_ListView->Invalidate();
				fOperations_ListView->UnlockLooper();
				
				//Relations
				fRelations_ListView->LockLooper();
					//vidage de la liste
					nbItems = fRelations_ListView->CountItems();
					for ( int32 i = 0; i < nbItems; i++ )
   						fRelations_ListView->RemoveItem((int32)0);
					//rajout des éléments
					for(int i = 0; i < itemSig->GetRelationList()->CountItems(); i++) {
						Relation *relation = (Relation *)itemSig->GetRelationList()->ItemAt(i);
						LatexListItem *li_relation = new LatexListItem((LView *)relation);
						fRelations_ListView->AddItem(li_relation);
					}
				fRelations_ListView->Invalidate();
				fRelations_ListView->UnlockLooper();
				
			}
			break;
		}
		case (kNomSignatureChange) :
		{
			int selected = fListeSignatures_ListView->GetView()->CurrentSelection(0);
			
			BString* nom_sig = new BString(fNomSignature->Text());
			Signature* sig = new Signature(nom_sig);
			
			LatexListItem* lisig = new LatexListItem((LView *)sig);
			
			fListeSignatures_ListView->LockLooper();
				//TODO why ReplaceItem doesn't work ??
				fListeSignatures_ListView->GetView()->RemoveItem(selected);
				fListeSignatures_ListView->GetView()->AddItem(lisig, selected);
				fListeSignatures_ListView->GetView()->Select(selected);
				fListeSignatures_ListView->GetView()->InvalidateItem(selected);
			fListeSignatures_ListView->GetView()->UnlockLooper();
			break;
		}
		
		//Constantes
		
		case (kNewConstante) :
		{	
			BString *nom = new BString(fNomConstante->Text());
			Constante* constante = new Constante(nom);
			LatexListItem* licon = new LatexListItem((LView *)constante);
			fConstantes_ListView->LockLooper();
				fConstantes_ListView->AddItem(licon);
				fConstantes_ListView->Select(fConstantes_ListView->IndexOf(licon));
				fConstantes_ListView->Invalidate();
			fConstantes_ListView->UnlockLooper();
			//TODO vérifier si un lock sur fListeSignatures_ListView est nécessaire
			int selected = fListeSignatures_ListView->GetView()->CurrentSelection(0);
			LatexListItem* latexItem = dynamic_cast<LatexListItem *>(fListeSignatures_ListView->GetView()->ItemAt(selected));
			Signature* itemSig = (Signature *)latexItem->GetLView();
			itemSig->GetConstanteList()->AddItem(constante);
			itemSig->SetModified(true);
			break;
		}
		case (kDeleteConstante) :
		{	
			int selected = fConstantes_ListView->CurrentSelection(0);
			fConstantes_ListView->LockLooper();
				fConstantes_ListView->RemoveItem(selected);
				fConstantes_ListView->Invalidate();
			fConstantes_ListView->UnlockLooper();
			int signatureSelected = fListeSignatures_ListView->GetView()->CurrentSelection(0);
			LatexListItem* latexItem = (LatexListItem*)(fListeSignatures_ListView->GetView()->ItemAt(signatureSelected));
			Signature *itemSig = (Signature *)latexItem->GetLView();
			itemSig->GetConstanteList()->RemoveItem(selected);
			itemSig->SetModified(true);
			break;
		}
		
		case(kSelectionConstante) :
		{
			//TODO ne rien faire si pas de signature selectionnee
			int selected = fConstantes_ListView->CurrentSelection(0);
			if ( selected < 0) {
				//TODO  supprimer ligne suivante
				//fNomConstante->SetEnabled(false);
				fNomConstante->SetText("");
			} 
			else {
				LatexListItem* latexListItem = (LatexListItem*)(fConstantes_ListView->ItemAt(selected));
				Constante* constante = (Constante*)latexListItem->GetLView();
				fNomConstante->SetEnabled(true);
				fNomConstante->SetText(constante->GetLatex()->String());
				
			}
			break;
		}
		case (kNomConstanteChange) :
		{
			int selected = fConstantes_ListView->CurrentSelection(0);
			BString* nom_constante = new BString(fNomConstante->Text());
			Constante* constante = new Constante(nom_constante);
			
			LatexListItem* itemConstantes = new LatexListItem((LView *)constante);
			fConstantes_ListView->LockLooper();
				//TODO why ReplaceItem doesn't work ??
				fConstantes_ListView->RemoveItem(selected);
				fConstantes_ListView->AddItem(itemConstantes, selected);
				fConstantes_ListView->Select(selected);
				fConstantes_ListView->InvalidateItem(selected);
			fConstantes_ListView->UnlockLooper();
			break;
		}
		
		
		//Relations
		
		case (kNewRelation) :
		{	
			BString *nom = new BString(fNomRelation->Text());
			int arite = atoi(fAriteRelation->Text());
			Relation* relation = new Relation(nom, arite);
			LatexListItem* licon = new LatexListItem((LView *)relation);
			fRelations_ListView->LockLooper();
				fRelations_ListView->AddItem(licon);
				fRelations_ListView->Select(fRelations_ListView->IndexOf(licon));
				fRelations_ListView->Invalidate();
			fRelations_ListView->UnlockLooper();
			//TODO vérifier si un lock sur fListeSignatures_ListView est nécessaire
			int selected = fListeSignatures_ListView->GetView()->CurrentSelection(0);
			LatexListItem* latexItem = dynamic_cast<LatexListItem *>(fListeSignatures_ListView->GetView()->ItemAt(selected));
			Signature* itemSig = (Signature *)latexItem->GetLView();
			itemSig->GetRelationList()->AddItem(relation);
			itemSig->SetModified(true);
			break;
		}
		
		case (kDeleteRelation) :
		{	
			int32 relationSelected = fRelations_ListView->CurrentSelection(0);
			fRelations_ListView->LockLooper();
				fRelations_ListView->RemoveItem(relationSelected);
				fRelations_ListView->Invalidate();
			fRelations_ListView->UnlockLooper();
			
			int selected = fListeSignatures_ListView->GetView()->CurrentSelection(0);
			LatexListItem* latexItem = (LatexListItem*)(fListeSignatures_ListView->GetView()->ItemAt(selected));
			Signature *itemSig = (Signature *)latexItem->GetLView();
			itemSig->GetRelationList()->RemoveItem(relationSelected);
			itemSig->SetModified(true);
			break;
		}
		
		case(kSelectionRelation) :
		{
			int selected = fRelations_ListView->CurrentSelection(0);
			if ( selected < 0) {
				fNomRelation->SetText("");
				fAriteRelation->SetText("");
			} 
			else {
				LatexListItem* latexItem = (LatexListItem*)(fRelations_ListView->ItemAt(selected));
				Relation* itemRelation = (Relation*)latexItem->GetLView();
				fNomRelation->SetEnabled(true);
				
				int32 indexSuffixe = itemRelation->GetLatex()->FindLast("_r^");
				if (indexSuffixe != B_ERROR) {
					fNomRelation->SetText(itemRelation->GetLatex()->Truncate(indexSuffixe).String());
				} else {
					fNomRelation->SetText(itemRelation->GetLatex()->String());
				}

				BString arite;
				arite << itemRelation->GetArite();
				fAriteRelation->SetText(arite);
				
			}
			break;
		}
		case (kNomRelationChange) :
		{
			
			int selected = fRelations_ListView->CurrentSelection(0);
			BString* nom_relation = new BString(fNomRelation->Text());
			if (strlen(fNomRelation->Text()) > 0 && strlen(fAriteRelation->Text()) > 0) {
				int arite = atoi(fAriteRelation->Text());
				Relation* relation = new Relation(nom_relation, arite);
				
				LatexListItem* itemRelations = new LatexListItem((LView *)relation);
				fRelations_ListView->LockLooper();
					//TODO why ReplaceItem doesn't work ??
					fRelations_ListView->RemoveItem(selected);
					fRelations_ListView->AddItem(itemRelations, selected);
					fRelations_ListView->Select(selected);
					fRelations_ListView->InvalidateItem(selected);
				fRelations_ListView->UnlockLooper();
				
				int selected = fListeSignatures_ListView->GetView()->CurrentSelection(0);
				Signature* itemSig = (Signature*)(fListeSignatures_ListView->GetView()->ItemAt(selected));
				itemSig->SetModified(true);
			}
			break;
		}
		
		
		//Operations
		
		case (kNewOperation) :
		{	
			BString *nom = new BString(fNomOperation->Text());
			int arite = atoi(fAriteOperation->Text());
			Operation* operation = new Operation(nom, arite);
			LatexListItem* licon = new LatexListItem((LView *)operation);
			fOperations_ListView->LockLooper();
				fOperations_ListView->AddItem(licon);
				fOperations_ListView->Select(fOperations_ListView->IndexOf(licon));
				fOperations_ListView->Invalidate();
			fOperations_ListView->UnlockLooper();
			//TODO vérifier si un lock sur fListeSignatures_ListView est nécessaire
			int selected = fListeSignatures_ListView->GetView()->CurrentSelection(0);
			LatexListItem* latexItem = dynamic_cast<LatexListItem *>(fListeSignatures_ListView->GetView()->ItemAt(selected));
			Signature* itemSig = (Signature *)latexItem->GetLView();
			
			itemSig->GetOperationList()->AddItem(operation);
			itemSig->SetModified(true);
			break;
		}
		
		case (kDeleteOperation) :
		{	
			int32 operationSelected = fOperations_ListView->CurrentSelection(0);
			fOperations_ListView->LockLooper();
				fOperations_ListView->RemoveItem(operationSelected);
				fOperations_ListView->Invalidate();
			fOperations_ListView->UnlockLooper();
			
			int selected = fListeSignatures_ListView->GetView()->CurrentSelection(0);
			LatexListItem* latexItem = dynamic_cast<LatexListItem *>(fListeSignatures_ListView->GetView()->ItemAt(selected));
			Signature* itemSig = (Signature *)latexItem->GetLView();
			
			itemSig->GetOperationList()->RemoveItem(operationSelected);
			itemSig->SetModified(true);
			break;
		}
		
		case(kSelectionOperation) :
		{
			int selected = fOperations_ListView->CurrentSelection(0);
			if ( selected < 0) {
				fNomOperation->SetText("");
				fAriteOperation->SetText("");
			} 
			else {
				LatexListItem * latexListItem= (LatexListItem*)(fOperations_ListView->ItemAt(selected));
				Operation* itemOperation = (Operation*)latexListItem->GetLView();
				fNomOperation->SetEnabled(true);
				
					
				int32 indexSuffixe = itemOperation->GetLatex()->FindLast("_r^");
				if (indexSuffixe != B_ERROR) {
					fNomOperation->SetText(itemOperation->GetLatex()->Truncate(indexSuffixe).String());
				} else {
					fNomOperation->SetText(itemOperation->GetLatex()->String());
				}
				
				BString arite;
				arite << itemOperation->GetArite();
				fAriteOperation->SetText(arite);
				
			}
			break;
		}
		case (kNomOperationChange) :
		{
			
			int selected = fOperations_ListView->CurrentSelection(0);
			BString* nom_operation = new BString(fNomOperation->Text());
			if (strlen(fNomOperation->Text()) > 0 && strlen(fAriteOperation->Text()) > 0) {
				int arite = atoi(fAriteOperation->Text());
				Operation* operation = new Operation(nom_operation, arite);
				
				LatexListItem* itemOperations = new LatexListItem((LView *)operation);
				fOperations_ListView->LockLooper();
					//TODO why ReplaceItem doesn't work ??
					fOperations_ListView->RemoveItem(selected);
					fOperations_ListView->AddItem(itemOperations, selected);
					fOperations_ListView->Select(selected);
					fOperations_ListView->InvalidateItem(selected);
				fOperations_ListView->UnlockLooper();
				
				int selected = fListeSignatures_ListView->GetView()->CurrentSelection(0);
				Signature* itemSig = (Signature*)(fListeSignatures_ListView->GetView()->ItemAt(selected));
				itemSig->SetModified(true);
			}
			break;
		}

	}
}

void SignatureWindow::ActiveButtons(bool activate) const {
	fAjoutConstanteButton->SetEnabled(activate);
	fSupprimeConstanteButton->SetEnabled(activate);
	
	fAjoutRelationButton->SetEnabled(activate);
	fSupprimeRelationButton->SetEnabled(activate);
	
	fAjoutOperationButton->SetEnabled(activate);
	fSupprimeOperationButton->SetEnabled(activate);
	
	fNomConstante->SetEnabled(activate);
	fNomRelation->SetEnabled(activate);
	fNomOperation->SetEnabled(activate);
	
	fAriteRelation->SetEnabled(activate);
	fAriteOperation->SetEnabled(activate);
}

void SignatureWindow::DesactivateLatexViews() const {
fConstantes_ListView->DeselectAll();
fOperations_ListView->DeselectAll();
fRelations_ListView->DeselectAll();
}

	
bool sauvegarde_signature(BListItem* item) {
	
	app_info app_info;
	be_app->GetAppInfo(&app_info);
	//TODO review those three lines
	BEntry *entry = new BEntry(&app_info.ref);
	BDirectory directory;
	entry->GetParent(&directory);
	BPath* path = new BPath(&directory);
	path->Append("signatures");
	Signature* signature = 
			(Signature *)
			(
			 (dynamic_cast<LatexListItem *>(item))->GetLView()
			);
	bool sauvegardeReussie  = signature->Sauvegarde(path);
	bool compilationReussie = true;
	if (signature->GetModified()) {
		 compilationReussie = signature->Compile(false);
	};
	return (sauvegardeReussie && compilationReussie);
	
}

bool SignatureWindow::QuitRequested() 
{
	int nbSig = fListeSignatures_ListView->GetView()->CountItems();
	for (int i = 0 ; i < nbSig ; i++) {
		std::cout << " SignatureWindow::QuitRequested item " << i << " type =" << class_name(fListeSignatures_ListView->GetView()->ItemAt(i)) << std::endl;
		LatexListItem* latexItem = dynamic_cast<LatexListItem *>(fListeSignatures_ListView->GetView()->ItemAt(i));
		if (!is_kind_of(latexItem->GetLView(), Signature))
			continue;
		Signature *signature = (Signature *)(latexItem->GetLView());
		if (signature->GetModified()) {	
			BString *ocamlSource = new BString();
			BList *liConstantes = signature->GetConstanteList();
			BString liConstantes_string("");
	
			for (int i = 0; i< liConstantes->CountItems(); i++) {
				const char * constante = ((Constante*)liConstantes->ItemAt(i))->GetLatex()->String();
				liConstantes_string << 	'"' << constante << '"' << ";";
			}
	
			*ocamlSource << "open Signature\ninclude SignatureMinimale\nlet _ = constantes := ["
			<< liConstantes_string
			<<"];;\n";

			app_info app_info;
			be_app->GetAppInfo(&app_info);
			//TODO review those three lines
			BEntry *entry = new BEntry(&app_info.ref);
			BDirectory *directory = new BDirectory();
			entry->GetParent(directory);
			BPath* path = new BPath(directory);
			path->Append("signatures");
			directory->SetTo(path->Path());	
			signature->SetOCamlChemin(new BPath(directory, LatexUtils::Normalize(signature->GetLatex())->String()));
			signature->SetSource(ocamlSource);
		}
	}
	fListeSignatures_ListView->GetView()->DoForEach(sauvegarde_signature);
	return true;
}
