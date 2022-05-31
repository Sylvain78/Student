#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <Archivable.h>
#include <List.h>
#include <Path.h>
#include <support/ClassInfo.h>

#include "Constante.h"
#include "Relation.h"
#include "Operation.h"
#include "LView.h"
#include "OCamlModule.h"

class Signature : public LView, public OCamlModule {

	public :
		Signature(BString* name);
		Signature(Signature* signature);
		Signature(BMessage *archive);
		
		BList* GetConstanteList() const;
		BList* GetOperationList() const;
		BList* GetRelationList() const;

		bool GetModified() const;
		void SetModified(bool modified);

		virtual status_t Archive(BMessage* archive, bool deep);
		static BArchivable *Instantiate(BMessage *archive);

		void PrintToStream();
	
	private :
		bool fModified;
		BList*   fConstante_List;
		BList*	 fOperation_List;
		BList*   fRelation_List;	
};

#endif
