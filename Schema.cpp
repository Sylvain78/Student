#include "Schema.h"

Schema::Schema(BString* nom)
	:LView(nom->String(), LTEXT, NULL)
{}
