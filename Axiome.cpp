#include "Axiome.h"


Axiome::Axiome(Axiome* axiome)
	:LView(axiome)
{
	fFormule = new BString(axiome->GetLatex()->String());
}

Axiome::Axiome(BString* nom, BString* formule)
	:LView(nom->String())
{
	fFormule = formule;
}
	
Axiome::Axiome(BMessage* archive)
	:LView(archive)
{
	fFormule= new BString();
	archive->FindString("fFormule", fFormule);
}

BString* Axiome::GetFormule() 
{
	return fFormule;
}

void Axiome::PrintToStream()
{
	puts(fFormule->String());
	LView::PrintToStream();
	
}

status_t Axiome::Archive(BMessage* archive, bool deep)
{
	status_t status;

	LView::Archive(archive, deep);
	
	status = archive->AddString("fFormule",fFormule->String());
	if (status < B_OK) return status;
	
	return status;
}


BArchivable* Axiome::Instantiate(BMessage* archive) {
	if (!validate_instantiation(archive, "Axiome")) {
		return NULL;
	}
	return new Axiome(archive);
}
