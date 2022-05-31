#include "Constante.h"


Constante::Constante(Constante* constante)
	:LView(constante)
{}

Constante::Constante(BString* nom)
	:LView(nom->String())
{}
	
Constante::Constante(BMessage* archive)
	:LView(archive)
{
}


void Constante::PrintToStream()
{
	LView::PrintToStream();
}

status_t Constante::Archive(BMessage* archive, bool deep)
{
	LView::Archive(archive, deep);
	return archive->AddString("class", "Constante");
}


BArchivable* Constante::Instantiate(BMessage* archive) {
	if (!validate_instantiation(archive, "Constante")) {
		return NULL;
	}
	return new Constante(archive);
}
