#ifndef SCHEMA_H
#define SCHEMA_H

#include "LView.h"

class Schema : public LView {

	public :
		Schema(BString* nom);
		/* 
		BString* schema,
		BString* formule_parametre, 
		BList* variables, 
			//Liste de BString
		BString* alias_variables_muettes,
		BList* variables_reservees);
		*/
};
#endif
