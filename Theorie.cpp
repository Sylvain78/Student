#include "Theorie.h"

#include "App.h"

Theorie::Theorie(BString* name)
	:LView(*name),
	 OCamlModule(new BPath()),
	 fModified(true),
	 fSignature(new Signature(new BString(""))),
	 	fAxiome_List(new BList()) {
}

Theorie::Theorie(BMessage* archive)
	:LView(archive), OCamlModule(archive)
{
	archive->FindBool("fModified", &fModified);
		
	//fSignature
	BMessage *signatureArchive = new BMessage();
	archive->FindMessage("signature", 0, signatureArchive);
	if (signatureArchive) 
		fSignature = new Signature(signatureArchive);
	else
	    fSignature = NULL;

		
	//fAxiome_List
	fAxiome_List = new BList();
	BMessage *axiomeArchive = new BMessage();
	int i=0;
	while(archive->FindMessage("axiome", i++, axiomeArchive) == B_OK) {
		Axiome *axiome = new Axiome(axiomeArchive);
 		fAxiome_List->AddItem(axiome);
	}
}

Theorie::Theorie(Theorie* theorie) 
	: LView(theorie), OCamlModule(theorie),
	fModified(theorie->GetModified())
{
	fAxiome_List = new BList(*theorie->GetAxiomeList());
	fSignature = new Signature(theorie->GetSignature());
}

bool Theorie::GetModified() const
{
	return fModified;
}


void Theorie::SetModified(bool modified) 
{
	fModified = modified;
}

BList* Theorie::GetAxiomeList() const
{
	return fAxiome_List;
}

Signature* Theorie::GetSignature() const
{
	return fSignature;
}

void Theorie::SetSignature(Signature* signature) {
	fSignature = new Signature(signature);
}

void Theorie::PrintToStream()
{
	puts(GetLatex()->String());
}

status_t Theorie::Archive(BMessage* archive, bool deep)
{
	status_t status;
	
	LView::Archive(archive,deep);
	OCamlModule::Archive(archive,deep);
		status = archive->AddString("class", "Theorie");
	if (status < B_OK) return status;
	
	status = archive->AddBool("fModified",false); //when saved, a theory is considered unmodified
	if (status < B_OK) return status;


	if(fSignature) {
		BMessage *archive_Signature = new BMessage();		
		status = fSignature->Archive(archive_Signature,deep);
		if (status < B_OK) return status;
		status = archive->AddMessage("signature", archive_Signature);
		
	} else {
		status = archive->AddMessage("signature", NULL);
	}
	if (status < B_OK) return status;

	Axiome* axiome;
	int index = 0;
	while(index < fAxiome_List->CountItems()) {
		axiome = (Axiome*)fAxiome_List->ItemAt(index);
		BMessage archive_axiome;
		axiome->Archive(&archive_axiome,deep);
		if (status < B_OK) return status;
		
		status = archive->AddMessage("axiome", &archive_axiome);
		index++;
	}
	
	return status;
}

BArchivable *
Theorie::Instantiate(BMessage *archive) {
	Theorie *theorie;
	if (!validate_instantiation(archive, "Theorie")) {
		return NULL;
	}
	theorie = new Theorie(archive);
	
	return theorie;
}
