#include "LatexUtils.h"

BString *LatexUtils::Normalize(BString *chaineLatex) {
	BString * chaineLatexNormalisee = new BString(*chaineLatex);
	
	chaineLatexNormalisee->ReplaceAll("_", "Underscore");
	chaineLatexNormalisee->ReplaceAll("\\", "Backslash");
	chaineLatexNormalisee->ReplaceAll("{", "LAcco");
	chaineLatexNormalisee->ReplaceAll("}", "RAcco");
	chaineLatexNormalisee->ReplaceAll("^", "Caret");
	chaineLatexNormalisee->ReplaceAll("+", "Plus");
	return chaineLatexNormalisee;
}
