#ifndef LATEXLISTSCROLLVIEW_H
#define LATEXLISTSCROLLVIEW_H 

#include <ListView.h>
#include <ScrollView.h>

#include "LatexListView.h"
class LatexListScrollView : public BScrollView {  
	public :  
		LatexListScrollView();
		LatexListView* GetView();
	private :
	LatexListView* fListView;	
};  

bool addToLatexListScrollView(void *lView, void* listScrollView_arg);

#endif
