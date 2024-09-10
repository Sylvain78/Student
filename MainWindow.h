#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Alert.h>
#include <Application.h>
#include <Button.h>
#include <GroupView.h>
#include <LayoutBuilder.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <ScrollView.h>
#include <TextView.h>
#include <Window.h>

#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include "Constantes.h"
#include "DemonstrationWindow.h"
#include "LWindow.h"
#include "TheorieWindow.h"

const float TEXT_INSET = 3.0;

class MainWindow : public BWindow
{
public:
							MainWindow(void);
			void			MessageReceived(BMessage *msg);
			virtual bool	QuitRequested(void);
			const char *	GetInputText();
			
private:

			BMenuBar*  fMenuBar;
};

#endif
