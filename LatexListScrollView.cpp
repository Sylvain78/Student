#include "LatexListScrollView.h"
#include <Size.h>

LatexListScrollView::LatexListScrollView() : BScrollView("latex list scroll", NULL,B_SUPPORTS_LAYOUT,true,true),
				 fListView(new LatexListView(B_SINGLE_SELECTION_LIST))
 {
 	//BScrollView* scrollView = new BScrollView("latex list scroll", fListView);
	//BGroupView::AddChild(/*scrollView*/ fListView);
	SetTarget(fListView);
	
}

LatexListView* LatexListScrollView::GetView() {
	return fListView;
}

bool addToLatexListScrollView(void *lView, void* listScrollView_arg) {
	LatexListItem *lItem = new LatexListItem((LView *)lView);
	return !(((LatexListView *)((LatexListScrollView *)listScrollView_arg)->GetView())->AddItem(lItem));
}
