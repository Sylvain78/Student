#include "LatexListItem.h"


LatexListItem::LatexListItem(LView* latex) :
	BListItem(),
	fLatex(latex) 
{
	SetHeight(fLatex->GetBitmap()->Bounds().Height());
	SetWidth(fLatex->GetBitmap()->Bounds().Width());
}
	
void LatexListItem::DrawItem(BView *owner, BRect frame, bool complete) {
	//TODO use standard colors
	rgb_color selected_background ={216,216,216,255};
	rgb_color draw_color={0,0,0,255};
	rgb_color color = draw_color;//TODO remove draw_color
    
    if (IsSelected() || complete) {
		color = selected_background;
	}
	else {
		color = owner->ViewColor();
	}
	
    owner->SetHighColor(color);
    owner->FillRect(frame);
	owner->MovePenTo(frame.left+4, frame.top);
	owner->SetHighColor(draw_color);
	owner->DrawBitmap(fLatex->GetBitmap());
}

void LatexListItem::Update(BView* owner, const BFont* font)
{
	BListItem::Update(owner, font);
	SetHeight(fLatex->GetBitmap()->Bounds().Height()+1);//TODO why +1
	SetWidth(fLatex->GetBitmap()->Bounds().Width()+1);//TODO why +1
}

void LatexListItem::PrintToStream()
{
	BMessage* archive = new BMessage();
	BListItem::Archive(archive,true);
	fLatex->Archive(archive,true);
	archive->PrintToStream();
}

LView* LatexListItem::GetLView() {
	return fLatex;
}
