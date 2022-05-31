#include "App.h"
#include "OCamlModule.h"

OCamlModule::OCamlModule(OCamlModule* ocamlModule) {
	fSource 	 = new BString(*ocamlModule->fSource);
	fDependances = new BString(*ocamlModule->fDependances);
	
	if (ocamlModule->fOCamlChemin == NULL) {
		fOCamlChemin = NULL;
	} else {
		fOCamlChemin = new BPath(*(ocamlModule->fOCamlChemin));
	}
}

OCamlModule::OCamlModule(BPath* ocamlChemin) 
	:fSource(new BString("")),
	fDependances(new BString(""))	
{
	BString *module = LatexUtils::Normalize(new BString(ocamlChemin->Leaf()));
		//Normalisation au cas où…
	ocamlChemin->GetParent(ocamlChemin);
		//TODO check return status
	fOCamlChemin = new BPath(ocamlChemin->Path(), module->String());
}

OCamlModule::OCamlModule(BMessage* archive) {
	
	fOCamlChemin = new BPath();
	fSource = new BString();
	fDependances = new BString();
	
	archive->FindFlat("fOCamlChemin", fOCamlChemin);
	archive->FindString("fSource", fSource);
	archive->FindString("fDependances", fDependances);
}

void OCamlModule::SetOCamlChemin(BPath* chemin) {
	fOCamlChemin = chemin;
}

BPath* OCamlModule::GetOCamlChemin() {
	return fOCamlChemin;
}

void OCamlModule::AddDependance(BString* dependance){
	fDependances->Append(" -I ");
	fDependances->Append(dependance->String());
	fDependances->Append(" ");
}

/////////////////////
//Compilation OCaml//
/////////////////////
void OCamlModule::SetSource(BString *source) {
	fSource = new BString(source->CharacterEscape("\\",'\\'));
}


status_t OCamlModule::Compile(bool native) {
	BString sourceName;
	BString objectName;
	BString command("");
	
	objectName = fOCamlChemin->Path();
	objectName << (native?".cmx":".cmo");
	sourceName = fOCamlChemin->Path();
	sourceName << ".ml";
	
	BFile *source = new BFile(sourceName, B_CREATE_FILE|B_ERASE_FILE|B_WRITE_ONLY);
		//TODO Gérer status
	int octetsEcrits = source->Write(fSource->String(), fSource->Length());
	if (octetsEcrits < 0 || octetsEcrits != fSource->Length()) {
		//TODO Gérer l'erreur
	}
	
	//((App *)be_app)->SendOCaml(*fSource);
		//Compilation dans la fenetre
		
	/*Compilation en ligne*/	
	BPath *chemin = new BPath();
	fOCamlChemin->GetParent(chemin);
	chdir(chemin->Path());
	command << (native?"ocamlopt.opt":"ocamlc.opt") 
	        << " -c "
	        << fDependances->String() 
	        << " -o" 
	        << " " 
	        << objectName
	        << " " << sourceName;

	return //system(command);
	B_OK;
}

status_t OCamlModule::Archive(BMessage *archive, bool deep) {
	status_t status;
	
	status = archive->AddString("fDependances", fDependances->String());
	if (status != B_OK) return status;
	
	archive->AddString("fSource", fSource->String());
	if (status != B_OK) return status;
	
	archive->AddFlat("fOCamlChemin", fOCamlChemin);
	return status;
	
}
