#ifndef LATEXLISTITEM_H
#define LATEXLISTITEM_H

#include <Bitmap.h>
#include <GraphicsDefs.h>
#include <View.h>

#include "ListItem.h"
#include "LView.h"

class LatexListItem : public BListItem {
	public : 
		LatexListItem(LView* latex);
		virtual void DrawItem(BView* owner, BRect frame, bool complete = true);	
		virtual void Update(BView* owner, const BFont* font);
		virtual LView* GetLView();
		void PrintToStream();
		
	private :
	 LView* fLatex;
};

#endif
