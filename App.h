#ifndef APP_H
#define APP_H

#include <stdio.h>

#include <map>

#include <Application.h>
#include <BitmapStream.h>
#include <fs_attr.h>
#include <storage/Directory.h>
#include <Message.h>
#include <Path.h>
#include <TranslatorRoster.h>
#include <TranslationUtils.h>
#include <storage/FindDirectory.h>
#include <storage/FilePanel.h>
#include <support/ClassInfo.h>

#include "Signature.h"
#include "Constantes.h"
#include "MainWindow.h"


int32 boucle(void *fd) ;

class App : public BApplication
{
	public:
		App(void);
		virtual bool QuitRequested();
		virtual void MessageReceived(BMessage *msg);
		void ReadyToRun();
		
		BMessage* GetSettings();

		int32 boucle_interactive(int fdin, int fdout);
		status_t latexToPNG(const BString& texte, BBitmap **image);
		
		BPath* GetTheoriesFolder();
		void ChooseSaveFolder();
		
		void SendOCaml(BString texte);
		BString* ReadOCaml();
		
		MainWindow* GetMainWindow();
		static unsigned int nb_images_produites;
		
		BList* GetSignatures();
		BList* GetTheories();
		
	private:
	// #pragma mark - OCaml
		int inputocaml[2];
		int outputocaml[2];
		int errorocaml[2];
		
		/* Cache des images */
		std::map <BString, BBitmap *> images;
		
		/* Dossier où sont stockées les théories*/
		BDirectory *fAppDirectory;
		BPath *fTheoriesFolder;
		
		// #pragma mark - Modèles
		BList *fSignatures;
		BList *fTheories;
		status_t _InitModels();
		
		BMessage *fSettings;
		
		MainWindow *fMainWin;
		
		
};

#endif
