#include "LatexListScrollView.h"
#include <Size.h>

LatexListScrollView::LatexListScrollView() : BScrollView("latex list scroll", (BView *)NULL, (uint32)0, true,true),
				 fListView(new LatexListView(B_SINGLE_SELECTION_LIST))
{
 	SetTarget(fListView);
}

LatexListView* LatexListScrollView::GetView() {
	return fListView;
}

bool addToLatexListScrollView(void *lView, void* listScrollView_arg) {
	LatexListItem *lItem = new LatexListItem((LView *)lView);
	return !(((LatexListView *)((LatexListScrollView *)listScrollView_arg)->GetView())->AddItem(lItem));
}
