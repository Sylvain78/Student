#include "LView.h"

#include "App.h"


LView::LView(LView* lview)
	:BView("LView", B_FOLLOW_NONE|B_WILL_DRAW),
	fLatex(lview->fLatex),
	fOldLatex(lview->fOldLatex),
	fBitmap(lview->fBitmap)
{}

LView::LView(const BString& texte, LView_kind kind, rgb_color bgColor)
	:BView("LView", B_FOLLOW_NONE|B_WILL_DRAW),
       fLatex(new BString(texte)),
       fOldLatex(new BString("")),
       fBitmap(NULL) {
        (((App*)be_app)->latexToPNG(texte, &fBitmap, kind, bgColor));
       	ResizeToPreferred();
       	SetDrawingMode(B_OP_ALPHA);
}

LView::LView(BMessage *archive)
	:BView(archive),
	fBitmap(new BBitmap(archive))
{
		fLatex = new BString();
		archive->FindString("fLatex", fLatex);

			
		fOldLatex = new BString("");
			//le champ sert juste à la sauvegarde
			
		BMessage* archive_bitmap = new BMessage();
		archive->FindMessage("fBitmap", archive_bitmap);
		fBitmap = new BBitmap(archive_bitmap);

		ResizeToPreferred();
       	SetDrawingMode(B_OP_ALPHA);
}

BString* LView::GetLatex() {
	return fLatex;
}


BBitmap* LView::GetBitmap()
{
	return fBitmap;
}

void LView::Draw(BRect)
{
	DrawBitmap(fBitmap, B_ORIGIN);
}

void LView::GetPreferredSize(float* width, float* height)
{

	if (fBitmap != NULL) {
		*width=fBitmap->Bounds().Width();
		*height=fBitmap->Bounds().Height()+10;
	} else {
		*width=0.0;
		*height=0.0;
		}	
}


void LView::MouseDown(BPoint where)
	{
	// For our example, we only support dragging with the left button
	BPoint temp;
	uint32 buttons;
	BRect r(fBitmap->Bounds());
	
	GetMouse(&temp, &buttons);
	

	// SetMouseEventMask can only be called from within
	// MouseDown(). It forces all mouse events to be sent to this
	// view until the button is released. This saves us from having
	// to manually code the mouse tracking.
	SetMouseEventMask(B_POINTER_EVENTS, 0);
	BMessage dragMsg(kLatex);
	Archive(&dragMsg,true);
	dragMsg.AddInt32("buttons", buttons);
	dragMsg.AddData("text/plain", B_MIME_TYPE, fLatex->String(),fLatex->Length()+1 /*ending \0*/);
	//	create new bitmap to drag around, deleted by DragMessage
	BBitmap * bmp = new BBitmap(fBitmap->Bounds(), fBitmap->ColorSpace(), false);
	memcpy(bmp->Bits(), fBitmap->Bits(), fBitmap->BitsLength());
	//	start the drag (the rest will be handled by MessageReceived())
	BPoint pt(where.x-r.left, where.y/*-r.top*/);
	DragMessage(&dragMsg, bmp, B_OP_ALPHA, pt, (BHandler *) Window());
	// If you allocate the drag message on the heap, make sure
	// you delete it after DragMessage() returns. If you use a
	// BBitmap version of DrawMessage(), DON'T delete the bitmap
	// after calling DragMessage() because the system will do
	// that for you.
	
}

status_t LView::Archive(BMessage* archive, bool deep) const
{
	status_t status;
	//if (LockLooper()) 
	//{
		status = BView::Archive(archive, deep);
		if (status < B_OK) {
//			UnlockLooper();
			return status;
		}
	
		archive->AddString("fLatex", fLatex->String());
		BMessage *archive_bitmap = new BMessage();
		status = fBitmap->Archive(archive_bitmap, deep);
		if (status < B_OK) {
			return status;
		}
		archive->AddMessage("fBitmap", archive_bitmap);
		
	//	UnlockLooper();
	//}
	return status;
}

BArchivable *
LView::Instantiate(BMessage *archive) {
	if (!validate_instantiation(archive, "LView")) {
		return NULL;
	}
	return new LView(archive);
}

void LView::PrintToStream()
{
	BMessage* archive=new BMessage();
	
	Archive(archive, true);
	archive->PrintToStream();
	
}


status_t LView::Sauvegarde(BPath* path) {
       BMessage* message = new BMessage();
       path->Append(fLatex->String());
       if(fOldLatex) {
               //TODO si nom modifier, supprimer ancien fichier
               
               fOldLatex = NULL;
               BFile* file = new BFile(path->Path(), B_CREATE_FILE | B_ERASE_FILE | B_WRITE_ONLY );
               this->Archive(message, true);
               return message->Flatten(file);
               
       }
       else {
               return B_OK;
       }
}
