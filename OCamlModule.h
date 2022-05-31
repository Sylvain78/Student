#ifndef OCAMLMODULE_H
#define OCAMLMODULE_H

#include <stdlib.h>

#include <Archivable.h>
#include <Entry.h>
#include <File.h>
#include <Path.h>
#include <String.h>

#include "LatexUtils.h"

class OCamlModule {
	public :
		OCamlModule(OCamlModule* ocamlModule);
		OCamlModule(BPath* ocamlChemin);
		OCamlModule(BMessage* archive);
		
		void SetOCamlChemin(BPath* chemin);
		BPath* GetOCamlChemin();
		
		void AddDependance(BString* dependance);
		virtual void SetSource(BString* sourceCode);
		status_t Compile(bool native);
		
		virtual status_t Archive(BMessage* archive, bool deep);
		static BArchivable *Instantiate(BMessage *archive);
		
	private :
	
	BString* fSource;
	BString* fDependances;
	BPath*   fOCamlChemin;
};

#endif
