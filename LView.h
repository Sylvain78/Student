#ifndef LVIEW_H
#define LVIEW_H

#include <ListItem.h>
#include <Path.h>
#include <String.h>
#include <TextControl.h>
#include <View.h>

#include "Constantes.h"

class LView : public BView {
	public:
		LView(LView* lview);
		LView(const BString& texte);
		LView(BMessage *archive);
		
		virtual status_t Archive(BMessage* archive, bool deep);
		static BArchivable *Instantiate(BMessage *archive);
		
		void Draw(BRect);
		void GetPreferredSize(float* width, float* height);
		void MouseDown(BPoint pt);
		BBitmap* GetBitmap();
		virtual void PrintToStream();
		BString* GetLatex();
		
		status_t Sauvegarde(BPath* path);

	private:
		BString* fLatex;
		BString* fOldLatex;
		BBitmap* fBitmap;
};

#endif
