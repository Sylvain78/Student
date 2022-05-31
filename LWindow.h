#ifndef LWINDOW_H
#define LWINDOW_H

#include <unistd.h>
#include <sys/wait.h>

#include <Bitmap.h>
#include <GridView.h>
#include <LayoutBuilder.h>
#include <String.h>
#include <StringView.h>
#include <Window.h>

#include "Constantes.h"
#include "LView.h"

class LWindow: public BWindow {

public:
		LWindow(void);
		
		void MessageReceived(BMessage* message);
};

#endif
