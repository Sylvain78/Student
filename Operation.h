#ifndef OPERATION_H
#define OPERATION_H

#include <stdio.h>

#include <Archivable.h>
#include <ClassInfo.h>


#include "LView.h"

class Operation : public LView {

	public :
		Operation(BString* nom, int arite);
		Operation(Operation* Operation);
		Operation(BMessage* archive);
		
		int GetArite();
		virtual status_t Archive(BMessage* archive, bool deep);
		void PrintToStream();
		static BArchivable* Instantiate(BMessage* archive);
	
	private :
		int fArite;
};

#endif
