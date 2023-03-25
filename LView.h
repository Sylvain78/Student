#ifndef LVIEW_H
#define LVIEW_H

#include <ListItem.h>
#include <Path.h>
#include <String.h>
#include <TextControl.h>
#include <View.h>

#include "server_protocol.pb.h"
#include "Constantes.h"

namespace SP = Server_Protocol;

enum LView_kind  
{
	LTEXT = SP::Latex_mode::LTEXT,
	LMATH = SP::Latex_mode::LMATH
};

class LView : public BView {
	public:
		LView(LView* lview);
		LView(const BString& texte, LView_kind kind = LTEXT, rgb_color *bgColor = NULL);
		LView(BMessage *archive);

		virtual status_t Archive(BMessage* archive, bool deep) const;
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
