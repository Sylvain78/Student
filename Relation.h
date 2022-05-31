#ifndef RELATION_H
#define RELATION_H

#include <stdio.h>

#include <Archivable.h>
#include <ClassInfo.h>


#include "LView.h"

class Relation : public LView {

	public :
		Relation(BString* nom, int arite);
		Relation(Relation* Relation);
		Relation(BMessage* archive);
		
		int GetArite();
		virtual status_t Archive(BMessage* archive, bool deep);
		void PrintToStream();
		static BArchivable* Instantiate(BMessage* archive);
	
	private :
		int fArite;
};

#endif
