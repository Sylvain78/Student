#ifndef AXIOME_H
#define AXIOME_H

#include <stdio.h>

#include <Archivable.h>
#include <ClassInfo.h>


#include "LView.h"

class Axiome : public LView {

	public :
		Axiome(BString* nom, BString* formule);
		Axiome(Axiome* Axiome);
		Axiome(BMessage* archive);
		
		BString* GetFormule();
		virtual status_t Archive(BMessage* archive, bool deep);
		void PrintToStream();
		static BArchivable* Instantiate(BMessage* archive);
	
	private :
		BString* fFormule;
};

#endif
