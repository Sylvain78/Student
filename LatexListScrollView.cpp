#include "LatexListScrollView.h"
#include <Size.h>

LatexListScrollView::LatexListScrollView() : BScrollView("latex list scroll", (BView *)NULL, (uint32)0, (uint32)B_SUPPORTS_LAYOUT, true,true, B_FANCY_BORDER),
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
