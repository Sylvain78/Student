#include "App.h"
#include "TheorieWindow.h"

TheorieWindow::TheorieWindow() :
	BWindow(BRect(100, 100, 500, 500),"Theories",B_TITLED_WINDOW,B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS)
{
	fMenuBar = new BMenuBar("menubar", B_ITEMS_IN_ROW, B_INVALIDATE_AFTER_LAYOUT);
	
	//fListeTheories = new BList();
	fListeTheories_ListView = new LatexListScrollView();
	fSignatures_ListView = new LatexListScrollView();
	fSignatures_ListView->GetView()->SetSelectionMessage(new BMessage(kSelectionSignature));
	
//	BView* listeTheories_ScrollView = new BScrollView("Vue déroulante de la liste des théories", fListeTheories_ListView, B_FRAME_EVENTS|B_SUPPORTS_LAYOUT,true, true);
//	listeTheories_ScrollView->SetExplicitMaxSize(BSize(190, B_SIZE_UNLIMITED));

	((App *)be_app)->GetTheories()->DoForEach(&addToLatexListScrollView, (void *)fListeTheories_ListView);
	((App *)be_app)->GetSignatures()->DoForEach(&addToLatexListScrollView, (void *)fSignatures_ListView);

	fListeTheories_ListView->SetDrawingMode(B_OP_ALPHA);
	fListeTheories_ListView->GetView()->SetSelectionMessage(new BMessage(kSelectionTheorie));
	fListeTheories_ListView->GetView()->ResizeToPreferred();
	//listeTheories_ScrollView->ResizeToPreferred();

	fNomTheorie = new BTextControl("Nom","", new BMessage(kNomTheorieChange));
	fNomTheorie->SetTarget(this);
	fNomTheorie->SetEnabled(false);

	//Signature
	fLangageView = new BView("Langage View", B_FOLLOW_NONE|B_WILL_DRAW);
	fLangage_GroupView = new BGroupView();
	
	//Axiomes
	fAxiomes_ListView = new BListView();
	fAxiomes_ListView->SetDrawingMode(B_OP_ALPHA);
	fAxiomes_ListView->SetSelectionMessage(new BMessage(kSelectionAxiome));
	fAxiomes_GroupView = new BGroupView();

	//Schemas
	fSchemas_ListView = new BListView();
	fSchemas_ListView->SetDrawingMode(B_OP_ALPHA);
	fSchemas_ListView->SetSelectionMessage(new BMessage(kSelectionSchema));
	fSchemas_GroupView = new BGroupView();


	fNomAxiome = new BTextControl("Nom","", new BMessage(kNomAxiomeChange));
	fNomAxiome->SetTarget(this);

	fAxiome = new BTextControl("Formule","", new BMessage(kAxiomeChange));
	fAxiome->SetTarget(this);
	//	fNomAxiome->SetEnabled(false);

	fAjoutAxiomeButton = new BButton("+", new BMessage(kNewAxiome));
	//	fAjoutAxiomeButton->SetEnabled(false);
	fSupprimeAxiomeButton = new BButton("-", new BMessage(kDeleteAxiome));
	//	fSupprimeAxiomeButton->SetEnabled(false);


	fNomSchema = new BTextControl("Nom","", new BMessage(kNomSchemaChange));
	fNomSchema->SetTarget(this);
	//	fNomSchema->SetEnabled(false);

	fAjoutSchemaButton = new BButton("+", new BMessage(kNewSchema));
	//	fAjoutSchemaButton->SetEnabled(false);
	fSupprimeSchemaButton = new BButton("-", new BMessage(kDeleteSchema));
	//	fSupprimeSchemaButton->SetEnabled(false);

	ActiveButtons(false);

	BTextView* langage_tv = new BTextView("Langage");
	langage_tv->SetText("Langage : ");


	langage_tv->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	rgb_color textColor = ui_color(B_PANEL_TEXT_COLOR);
	langage_tv->SetFontAndColor(be_plain_font, B_FONT_ALL, &textColor);
	langage_tv->MakeEditable(false);
	langage_tv->MakeSelectable(false);
	langage_tv->SetWordWrap(false);
	////////////////////
	/*
	   BLayoutBuilder::Group<>(this, B_VERTICAL, 0.0f)
	   878 		.Add(fMenuBar = new BMenuBar("Menu"))
	   879 		.AddSplit(B_VERTICAL, splitSpacing)
	   880 			.GetSplitView(&fFunctionSplitView)
	   881 			.SetInsets(B_USE_SMALL_INSETS)
	   882 			.Add(fTabView = new BTabView("tab view"), 0.4f)
	   883 			.AddSplit(B_HORIZONTAL, splitSpacing)
	   884 				.GetSplitView(&fSourceSplitView)
	   885 				.AddGroup(B_VERTICAL, B_USE_SMALL_SPACING)
	   886 					.AddGroup(B_HORIZONTAL, B_USE_SMALL_SPACING)
	   887 						.Add(fRunButton = new BButton("Run"))
	   888 						.Add(fStepOverButton = new BButton("Step over"))
	   889 						.Add(fStepIntoButton = new BButton("Step into"))
	   890 						.Add(fStepOutButton = new BButton("Step out"))
	   891 						.AddGlue()
	   892 					.End()
	   */
	////////////////////

	BLayoutBuilder::Group<>(fLangage_GroupView, B_VERTICAL, 0.0f)
		.AddSplit(B_HORIZONTAL)
			.Add(fLangageView)
			.Add(fSignatures_ListView)
		.End()
		;

	BLayoutBuilder::Group<>(fAxiomes_GroupView, B_VERTICAL, 0.0f)
		.Add(fAxiomes_ListView)
		.Add(fNomAxiome)
		.Add(fAxiome)
		.Add(fAjoutAxiomeButton)
		.Add(fSupprimeAxiomeButton);
	
	BLayoutBuilder::Group<>(fSchemas_GroupView)
		.Add(fSchemas_ListView)
		.Add(fNomSchema)
		.Add(fAjoutSchemaButton)
		.Add(fSupprimeSchemaButton)
		.End();

	BMenu* menu = new BMenu("Théorie");
	menu->AddItem(new BMenuItem("Nouvelle", new BMessage(kNewTheory), 'N'));
	fMenuBar->AddItem(menu);

	fTabView_Theorie = new BTabView("TheoriesView");

	BTab* nom     = new BTab();
	BTab* langage = new BTab();
	BTab* axiomes = new BTab();
	BTab* schemas = new BTab();
	
	fTabView_Theorie->AddTab(fNomTheorie,nom);
	fTabView_Theorie->AddTab(fLangage_GroupView, langage);
	fTabView_Theorie->AddTab(fAxiomes_GroupView, axiomes);	
	fTabView_Theorie->AddTab(fSchemas_GroupView, schemas);	

	nom->SetLabel("Nom");
	langage->SetLabel("Langage");
	axiomes->SetLabel("Axiomes");
	schemas->SetLabel("Schémas");


	BLayoutBuilder::Group<>(this, B_VERTICAL, 0.0f)
		.Add(fMenuBar)
//		.AddGroup(B_HORIZONTAL, B_USE_SMALL_SPACING)
//		.AddSplit(B_HORIZONTAL,3.0f)
//		.Add(fSignatureView)
//	 					.Add(langage)
//						.Add(fLangageView)
//		.End()
//		.End()
//		.AddGroup(B_VERTICAL)
		.AddGroup(B_HORIZONTAL)
			.SetInsets(B_USE_WINDOW_INSETS)
			.AddSplit(B_HORIZONTAL)
				.Add(fListeTheories_ListView)
				.Add(fTabView_Theorie)
			.End()
		.End()
//		.End()
		;
}

void TheorieWindow::MessageReceived(BMessage* message) 
{
	//TODO Nettoyer les cas
	switch(message->what)
	{
		//Theories
		case(kNewTheory) : 
			{
				BString *nom = new BString("Theorie1");
				Theorie* theorie = new Theorie(nom);
				LatexListItem* lisig = new LatexListItem(theorie);
				fListeTheories_ListView->LockLooper();
				fListeTheories_ListView->GetView()->AddItem(lisig);

				fListeTheories_ListView->Invalidate();
				fListeTheories_ListView->ResizeToPreferred();
				fListeTheories_ListView->UnlockLooper();
				break;
			}
		case(kSelectionTheorie) :
			{
				int selected = fListeTheories_ListView->GetView()->CurrentSelection(0);
				if ( selected < 0) {
					fNomTheorie->SetEnabled(false);
					fNomTheorie->SetText("");
					ActiveButtons(false);
				} 
				else {
					ActiveButtons(true);
					LatexListItem* itemTheorie = (LatexListItem*)(fListeTheories_ListView->GetView()->ItemAt(selected));
					fNomTheorie->SetEnabled(true);
					Theorie *theorie = (Theorie *)itemTheorie->GetLView();
					fNomTheorie->SetText(theorie->GetLatex()->String());

					//Signature
					Signature *signature = theorie->GetSignature();
					LatexListItem* li = new LatexListItem((LView *)signature);
					int index = fSignatures_ListView->GetView()->IndexOf(li);
					fSignatures_ListView->GetView()->Select(index);
					fSignatures_ListView->Invalidate();
					
					//TODO pave duplique
					BString langage = BString("\\[\\mathfrak{L}_{");
					langage << signature->GetLatex()->String() << "}\\]";
					LView * langageView =new LView(langage);

					fLangageView->LockLooper();
					if (fLangageView->ChildAt(0)) {
						fLangageView->ChildAt(0)->RemoveSelf();
					}
					fLangageView->AddChild(langageView);
					fLangageView->Invalidate();
					fLangageView->UnlockLooper();

					//axiomes
					fAxiomes_ListView->LockLooper();
					//vidage de la liste
					int nbItems = fAxiomes_ListView->CountItems();
					for ( int32 i = 0; i <  nbItems; i++ )
						fAxiomes_ListView->RemoveItem((int32)0);
					//rajout des éléments
					for(int i = 0; i < theorie->GetAxiomeList()->CountItems(); i++) {
						Axiome *axiome = (Axiome *)theorie->GetAxiomeList()->ItemAt(i);
						LatexListItem *li_Axiome = new LatexListItem((LView *)axiome);
						fAxiomes_ListView->AddItem(li_Axiome);
					}
					fAxiomes_ListView->Invalidate();
					fAxiomes_ListView->UnlockLooper();
				}
				break;
			}
		case (kNomTheorieChange) :
			{
				int selected = fListeTheories_ListView->GetView()->CurrentSelection(0);

				BString* nom_sig = new BString(fNomTheorie->Text());
				Theorie* theorie = new Theorie(nom_sig);

				LatexListItem* litheorie = new LatexListItem((LView *)theorie);


				fListeTheories_ListView->LockLooper();

				//TODO why ReplaceItem doesn't work ??
				fListeTheories_ListView->GetView()->RemoveItem(selected);
				fListeTheories_ListView->GetView()->AddItem(litheorie, selected);
				fListeTheories_ListView->GetView()->Select(selected);
				fListeTheories_ListView->GetView()->InvalidateItem(selected);
				fListeTheories_ListView->UnlockLooper();
				break;
			}

			//Axiomes

		case (kNewAxiome) :
			{	
				BString *nom = new BString(fNomAxiome->Text());
				BString *formule = new BString(fAxiome->Text());
				Axiome* axiome = new Axiome(nom, formule);
				LatexListItem* licon = new LatexListItem((LView *)axiome);
				fAxiomes_ListView->LockLooper();
				fAxiomes_ListView->AddItem(licon);
				fAxiomes_ListView->Select(fAxiomes_ListView->IndexOf(licon));
				fAxiomes_ListView->Invalidate();
				fAxiomes_ListView->UnlockLooper();
				//TODO vérifier si un lock sur fListeTheories_ListView est nécessaire
				int selected = fListeTheories_ListView->GetView()->CurrentSelection(0);
				LatexListItem* itemTheorie = (LatexListItem*)(fListeTheories_ListView->GetView()->ItemAt(selected));
				Theorie *theorie = (Theorie*)itemTheorie->GetLView();
				theorie->GetAxiomeList()->AddItem(axiome);
				theorie->SetModified(true);
				break;
			}

		case (kDeleteAxiome) :
			{	
				fAxiomes_ListView->LockLooper();
				int32 axiome = fAxiomes_ListView->CurrentSelection(0);
				fAxiomes_ListView->RemoveItem(axiome);
				fAxiomes_ListView->Invalidate();

				fAxiomes_ListView->UnlockLooper();
				int selected = fListeTheories_ListView->GetView()->CurrentSelection(0);
				LatexListItem* itemTheorie = (LatexListItem*)(fListeTheories_ListView->GetView()->ItemAt(selected));
				Theorie *theorie = (Theorie*)itemTheorie->GetLView();
				theorie->GetAxiomeList()->RemoveItem(axiome);
				theorie->SetModified(true);
				break;
			}

		case(kSelectionSchema) :
			{
				int selected = fAxiomes_ListView->CurrentSelection(0);
				if ( selected < 0) {
					fNomSchema->SetText("");
				} 
				else {
					LatexListItem* itemSchema = (LatexListItem*)(fAxiomes_ListView->ItemAt(selected));
					fNomSchema->SetEnabled(true);
					fNomSchema->SetText(itemSchema->GetLView()->GetLatex()->String());
				}
				break;
			}

		case(kSelectionAxiome) :
			{
				int selected = fAxiomes_ListView->CurrentSelection(0);
				if ( selected < 0) {
					fNomAxiome->SetText("");
					fAxiome->SetText("");
				} 
				else {
					LatexListItem* itemAxiome = (LatexListItem*)(fAxiomes_ListView->ItemAt(selected));
					fNomAxiome->SetEnabled(true);
					fNomAxiome->SetText(((Axiome*)itemAxiome->GetLView())->GetLatex()->String());
					fAxiome->SetEnabled(true);
					fAxiome->SetText(((Axiome*)itemAxiome->GetLView())->GetFormule()->String());

				}
				break;
			}
		case (kNomSchemaChange) :
			{

				int selected = fAxiomes_ListView->CurrentSelection(0);
				BString* nom_schema = new BString(fNomSchema->Text());
				if (strlen(fNomSchema->Text()) > 0) {

					Schema* schema = new Schema(nom_schema);

					LatexListItem* itemSchemas = new LatexListItem((LView *)schema);
					fAxiomes_ListView->LockLooper();
					//TODO why ReplaceItem doesn't work ??
					fAxiomes_ListView->RemoveItem(selected);
					fAxiomes_ListView->AddItem(itemSchemas, selected);
					fAxiomes_ListView->Select(selected);
					fAxiomes_ListView->InvalidateItem(selected);
					fAxiomes_ListView->UnlockLooper();

					int selected = fListeTheories_ListView->GetView()->CurrentSelection(0);
					Theorie* itemTheorie = (Theorie*)(fListeTheories_ListView->GetView()->ItemAt(selected));
					itemTheorie->SetModified(true);
				}
				break;
			}

			//Drag 'n drop signature
		case (kLatex) :
			{
				int selected = fListeTheories_ListView->GetView()->CurrentSelection(0);
				if ( selected >= 0) {
					ActiveButtons(true);

					LatexListItem* latexItemTheorie = ( (LatexListItem*)(fListeTheories_ListView->GetView()->ItemAt(selected)) );
					Theorie *itemTheorie = (Theorie *)latexItemTheorie->GetLView();

					if(!validate_instantiation(message, "Signature")) break;

					Signature* sig = new Signature(message);

					itemTheorie->SetSignature(sig);
					
					BString langage = BString("\\[\\mathfrak{L}_{");
					langage << sig->GetLatex()->String() << "}\\]";
					LView * langageView =new LView(langage);

					fLangageView->LockLooper();
					if (fLangageView->ChildAt(0)) {
						fLangageView->ChildAt(0)->RemoveSelf();
					}
					fLangageView->AddChild(langageView);
					fLangageView->Invalidate();
					fLangageView->UnlockLooper();
					itemTheorie->SetModified(true);

				}
				break;
			}
		case (kSelectionSignature) :
			{
				int selected = fListeTheories_ListView->GetView()->CurrentSelection(0);
				if ( selected >= 0) {
					ActiveButtons(true);

					LatexListItem* latexItemTheorie = ( (LatexListItem*)(fListeTheories_ListView->GetView()->ItemAt(selected)) );
					Theorie *itemTheorie = (Theorie *)latexItemTheorie->GetLView();

					int signature_index = fSignatures_ListView->GetView()->CurrentSelection(0);
					if (signature_index >= 0) {
						Signature* sig = (Signature *)((LatexListItem *)fSignatures_ListView->GetView()->ItemAt(signature_index))->GetLView();;
	
						itemTheorie->SetSignature(sig);
						
						BString langage = BString("\\[\\mathfrak{L}_{");
						langage << sig->GetLatex()->String() << "}\\]";
						LView * langageView =new LView(langage);
	
						fLangageView->LockLooper();
						if (fLangageView->ChildAt(0)) {
							fLangageView->ChildAt(0)->RemoveSelf();
						}
						fLangageView->AddChild(langageView);
						fLangageView->Invalidate();
						fLangageView->UnlockLooper();
						itemTheorie->SetModified(true);
					}
				}
				break;
			}
	}
}

void TheorieWindow::ActiveButtons(bool activate) {
	fAjoutSchemaButton->SetEnabled(activate);
	fSupprimeSchemaButton->SetEnabled(activate);

	fAjoutAxiomeButton->SetEnabled(activate);
	fSupprimeAxiomeButton->SetEnabled(activate);

	fNomSchema->SetEnabled(activate);
	fNomAxiome->SetEnabled(activate);
	fAxiome->SetEnabled(activate);

}

bool sauvegarde_theorie(BListItem* theorieItem) {

	app_info app_info;
	be_app->GetAppInfo(&app_info);
	BEntry entry = BEntry(&app_info.ref);
	BDirectory directory;
	entry.GetParent(&directory);
	BPath path = BPath(&directory);
	path.Append("théories");

	Theorie* theorie = 
		(Theorie *)
		(
		 (dynamic_cast<LatexListItem *>(theorieItem))->GetLView()
		);
	bool sauvegardeReussie  = theorie->Sauvegarde(&path);
	bool compilationReussie = true;
	if (theorie->GetModified()) {
		compilationReussie = theorie->Compile(false);
	};
	return (sauvegardeReussie && compilationReussie);
}

bool TheorieWindow::QuitRequested() 
{
	int nbSig = fListeTheories_ListView->GetView()->CountItems();
	for (int i = 0 ; i < nbSig ; i++) {
		LatexListItem* latexItem = (LatexListItem*)(fListeTheories_ListView->GetView()->ItemAt(i));
		Theorie *theorie = (Theorie *)latexItem->GetLView();
		if (theorie->GetModified()) {	
			BString *ocamlSource = new BString();
			BList *liAxiomes = theorie->GetAxiomeList();
			BString liAxiomes_string("");

			for (int i = 0; i< liAxiomes->CountItems(); i++) {
				const char * axiome = ((Axiome*)liAxiomes->ItemAt(i))->GetLatex()->String();
				liAxiomes_string << 	'"' << axiome << '"' << ";";
			}

			//TODO texte a revoir	
			*ocamlSource << "open Theorie\ninclude Theorie("
				<< (LatexUtils::Normalize(theorie->GetSignature()->GetLatex()))->String() 
				<< ")\nlet _ = langage := ["
				<< liAxiomes_string
				<<"];;\n";

			app_info app_info;
			be_app->GetAppInfo(&app_info);
			//TODO review those three lines
			BEntry *entry = new BEntry(&app_info.ref);
			BDirectory *directory = new BDirectory();
			entry->GetParent(directory);
			BPath* path = new BPath(directory);
			path->Append("theories");
			directory->SetTo(path->Path());	
			theorie->SetOCamlChemin(new BPath(directory, LatexUtils::Normalize(theorie->GetLatex())->String()));
			theorie->SetSource(ocamlSource);
		}
	}
	fListeTheories_ListView->GetView()->DoForEach(sauvegarde_theorie);
	return true;
}
