#include "Relation.h"


Relation::Relation(Relation* relation)
	:LView(relation),
	fArite(relation->fArite)
{}

Relation::Relation(BString* relation, int arite)
	:LView(*relation << "_r^" << arite),
	fArite(arite)
{}
	
Relation::Relation(BMessage* archive)
	:LView(archive)
{
	archive->FindInt32("fArite", (int32*)&fArite);
}

int Relation::GetArite() 
{
	return fArite;
}

void Relation::PrintToStream()
{
	LView::PrintToStream();
}

status_t Relation::Archive(BMessage* archive, bool deep)
{
	status_t status;
	
	status = LView::Archive(archive, deep);
	if (status < B_OK) return status;
	
	status = archive->AddInt32("fArite", fArite);
	
	return status;
}


BArchivable* Relation::Instantiate(BMessage* archive) {
	if (!validate_instantiation(archive, "Relation")) {
		return NULL;
	}
	return new Relation(archive);
}
