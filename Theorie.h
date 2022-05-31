#ifndef THEORIE_H
#define THEORIE_H

#include <Archivable.h>
#include <List.h>
#include <Path.h>
#include <ClassInfo.h>

#include "LView.h"
#include "OCamlModule.h"
#include "Signature.h"
#include "Axiome.h"

class Theorie : public LView, public OCamlModule {

	public :
		Theorie(BString* name);
		Theorie(Theorie* theorie);
		Theorie(BMessage *archive);
				
		BList* GetAxiomeList() const;
		
		void   SetSignature(Signature* signature);
		Signature* GetSignature() const;
		
		bool GetModified() const;
		void SetModified(bool modified);
		
		virtual status_t Archive(BMessage* archive, bool deep);
		static BArchivable *Instantiate(BMessage *archive);
		
		void PrintToStream();
			
	private :
		bool		fModified;
		Signature*	fSignature;
		BList*		fAxiome_List;
};

#endif
