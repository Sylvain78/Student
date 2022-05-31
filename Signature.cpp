#include "Signature.h"

#include "App.h"

Signature::Signature(BString* name)
	:LView(*name), OCamlModule(new BPath())
{
	fModified = true;
	fConstante_List = new BList();
	fOperation_List = new BList();
	fRelation_List = new BList();
	AddDependance(new BString("../Preuves/_build/premier_ordre"));
}

Signature::Signature(BMessage* archive)
	:LView(archive), OCamlModule(archive)
{
	archive->FindBool("fModified", &fModified);
	
	//fConstante_List
	fConstante_List = new BList();
	BMessage *constanteArchive = new BMessage();
	int32 i=0;
	while(archive->FindMessage("constante", i++, constanteArchive) == B_OK) {
	Constante *constante = new Constante(constanteArchive);
	fConstante_List->AddItem(constante);
	}
	
	//fRelation_List
	fRelation_List = new BList();
	BMessage *relationArchive = new BMessage();
	i=0;
	while(archive->FindMessage("relation", i++, relationArchive) == B_OK) {
		Relation *relation = new Relation(relationArchive);
		fRelation_List->AddItem(relation);
	}
	
	//fOperation_List
	fOperation_List = new BList();
	BMessage *operationArchive = new BMessage();
	i=0;
	while(archive->FindMessage("operation", i++, operationArchive) == B_OK) {
		Operation *operation = new Operation(operationArchive);
		fOperation_List->AddItem(operation);
	}	
}

Signature::Signature(Signature* signature)
	:LView(signature),
	OCamlModule(signature) 
{
	fModified = signature->fModified;
	fConstante_List = new BList(*(signature->fConstante_List));
	fRelation_List  = new BList(*(signature->fRelation_List));
	fOperation_List = new BList(*(signature->fOperation_List));
}

bool Signature::GetModified() const 
{
	return fModified;
}

void Signature::SetModified(bool modified) 
{
	fModified = modified;
}

BList* Signature::GetConstanteList() const
{
	return fConstante_List;
}

BList* Signature::GetRelationList() const
{
	return fRelation_List;
}

BList* Signature::GetOperationList() const
{
	return fOperation_List;
}

void Signature::PrintToStream()
{
	LView::PrintToStream();
}

status_t Signature::Archive(BMessage* archive, bool deep) {
	status_t status;
	LView::Archive(archive,deep);
	OCamlModule::Archive(archive, deep);
	status = archive->AddString("class", "Signature");
	if (status < B_OK) return status;
	
	status = archive->AddBool("fModified",false); //when saved, a signature is considered unmodified
	if (status < B_OK) return status;
	
	Constante* constante;
	int index = 0;
	while(index < fConstante_List->CountItems()) {
		constante = (Constante*)fConstante_List->ItemAt(index);
		BMessage archive_constante;
		constante->Archive(&archive_constante,deep);
		if (status < B_OK) return status;
		
		status = archive->AddMessage("constante", &archive_constante);
		index++;
	}
	if (status < B_OK) return status;

	Relation* relation;
	index = 0;
	while(index < fRelation_List->CountItems()) {
		relation = (Relation*)fRelation_List->ItemAt(index);
		BMessage archive_relation;
		relation->Archive(&archive_relation,deep);
		if (status < B_OK) return status;
		
		status = archive->AddMessage("relation", &archive_relation);
		index++;
	}

	Operation* operation;
	index = 0;
	while(index < fOperation_List->CountItems()) {
		operation = (Operation*)fOperation_List->ItemAt(index);
		BMessage archive_operation;
		operation->Archive(&archive_operation,deep);
		if (status < B_OK) return status;
		
		status = archive->AddMessage("operation", &archive_operation);
		index++;
	}
	return status;
}

BArchivable *
Signature::Instantiate(BMessage *archive) {
	Signature *sig;
	if (!validate_instantiation(archive, "Signature")) {
		return NULL;
	}
	sig = new Signature(archive);
	return sig;
}
