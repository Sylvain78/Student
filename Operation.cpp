#include "Operation.h"


Operation::Operation(Operation* operation)
	:LView(operation),
	fArite(operation->fArite)
{}

Operation::Operation(BString* operation, int arite)
	:LView(operation->String(), LMATH, NULL),
	fArite(arite)
{}
	
Operation::Operation(BMessage* archive)
	:LView(archive)
{
	archive->FindInt32("fArite", (int32*)&fArite);
}

int Operation::GetArite() 
{
	return fArite;
}

void Operation::PrintToStream()
{
	LView::PrintToStream();
}

status_t Operation::Archive(BMessage* archive, bool deep)
{
	status_t status;
	
	status = LView::Archive(archive, deep);
	if (status < B_OK) return status;
	
	status = archive->AddInt32("fArite", fArite);
	
	return status;
}


BArchivable* Operation::Instantiate(BMessage* archive) {
	if (!validate_instantiation(archive, "Operation")) {
		return NULL;
	}
	return new Operation(archive);
}
