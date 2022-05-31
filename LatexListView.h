#ifndef LATEXLISTVIEW_H
#define LATEXLISTVIEW_H 

//#include <GroupView.h>
#include <ListView.h>

#include "LView.h"

#include "LatexListItem.h"

class LatexListView : public BListView {  
	public :  
		LatexListView(list_view_type type);
		virtual bool InitiateDrag(BPoint point, int32  index,bool  wasSelected);   
		virtual bool AddItem(BListItem* item, int32 atIndex=0);
};  

bool addToLatexListView(void *lView, void* listView_arg);

#endif
