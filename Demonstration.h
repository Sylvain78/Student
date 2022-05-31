#ifndef DEMONSTRATION_H
#define DEMONSTRATION_H

#include <List.h>

#include "Theorie.h"

class Demonstration : public BList {
	public :
		Demonstration(Theorie* theorie);
		Demonstration(BMessage* archive);
	
	private :
		Theorie *fTheorie;
		BList* fDemonstration; //Liste de formules
};

#endif
