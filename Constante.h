#ifndef CONSTANTE_H
#define CONSTANTE_H

#include <stdio.h>

#include <Archivable.h>
#include <ClassInfo.h>


#include "LView.h"

class Constante : public LView {

	public :
		Constante(BString* nom);
		Constante(Constante* Constante);
		Constante(BMessage* archive);
		
		virtual status_t Archive(BMessage* archive, bool deep) const;
		void PrintToStream();
		static BArchivable* Instantiate(BMessage* archive);
	
};

#endif
