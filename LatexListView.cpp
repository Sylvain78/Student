#include "LatexListView.h"

LatexListView::LatexListView(list_view_type type):
	BListView(type) {};

bool LatexListView::AddItem(BListItem* item, int32 atIndex) {
	float width,height;
	GetPreferredSize(&width, &height);
	bool added = BListView::AddItem(item, atIndex);
	if (added) {
		height	+= item->Height();
		width = std::max(width, item->Width());
		ResizeTo(width,height);
		
		InvalidateLayout();
	}
	return added;
}

bool LatexListView::InitiateDrag(BPoint  point, int32  index, bool  wasSelected) {
	BPoint temp;
	uint32 buttons;
	LatexListItem* latex_list_item = (LatexListItem*)ItemAt(index);	
	BRect r(latex_list_item->GetLView()->Bounds());
	
	GetMouse(&temp, &buttons);
	
	// SetMouseEventMask can only be called from within
	// MouseDown(). It forces all mouse events to be sent to this
	// view until the button is released. This saves us from having
	// to manually code the mouse tracking.
	SetMouseEventMask(B_POINTER_EVENTS, 0);
	BMessage dragMsg(kLatex);
	dragMsg.AddInt32("buttons", buttons);
	BString *latex_string = latex_list_item->GetLView()->GetLatex();
	dragMsg.AddData("text/plain", B_MIME_TYPE, latex_string->String(),
	latex_string->Length()+1 /*ending \0*/);

	latex_list_item->GetLView()->Archive(&dragMsg, true);
	

	//	create new bitmap to drag around, deleted by DragMessage
	BBitmap * bmp = new BBitmap(latex_list_item->GetLView()->Bounds(), latex_list_item->GetLView()->GetBitmap()->ColorSpace(), false);
	memcpy(bmp->Bits(), latex_list_item->GetLView()->GetBitmap()->Bits(), latex_list_item->GetLView()->GetBitmap()->BitsLength());

	//	start the drag (the rest will be handled by MessageReceived())
	BPoint pt(point.x-r.left, point.y-r.top);
	dragMsg.PrintToStream();
	DragMessage(&dragMsg,bmp, B_OP_ALPHA, pt, (BHandler *) Window());
	// If you allocate the drag message on the heap, make sure
	// you delete it after DragMessage() returns. If you use a
	// BBitmap version of DrawMessage(), DON'T delete the bitmap
	// after calling DragMessage() because the system will do
	// that for you.
	

	return true;
} 


bool addToLatexListView(void *lView, void* listView_arg) {
	
	LatexListView *listView = (LatexListView*)listView_arg;
	LatexListItem* latexListItem = new LatexListItem((LView *)lView);
	//listView->LockLooper();
		bool added = listView->AddItem(latexListItem);
		if (!added) {
			return true;
		}
		float width, height;
		listView->GetPreferredSize(&width,&height);
		width = fmax(width, latexListItem->Width());
		listView->SetExplicitMinSize(BSize(width,height));
		listView->ResizeToPreferred();
		listView->Relayout();
		//TODO Lock and Invalidate necessary ?
		listView->Invalidate();
	//listView->UnlockLooper();
	return false;
}
