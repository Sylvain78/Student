#include <iostream>
#include <string>

#include <poll.h>



#include "App.h"

unsigned int App::nb_images_produites = 0;

App::App(void)
	:	BApplication("application/x-vnd.Student"),
	fSignatures(new BList()),
	fTheories(new BList()),
	fSettings(new BMessage())
{
	fAppDirectory = new BDirectory();
	
	fMainWin = new MainWindow();
	fMainWin->Show();
}

App::~App() {
	delete fAppDirectory;
	delete fSettings;
	delete fTheories;
	delete fSignatures;
}

/**
 * Launch ocaml
 * Verifies that signatures,théories and images directories are present
 */
void App::ReadyToRun(){
	_InitModels(); 
}

bool App::QuitRequested(void)
{
	return BApplication::QuitRequested();
}

void App::MessageReceived(BMessage *m) 
{
	switch (m->what)
	{
		
		default:
		{
			BApplication::MessageReceived(m);
			break;
		}
	}
}

status_t App::latexToPNG(const BString& texte, BBitmap **image, LView_kind kind, rgb_color rgb_back_color) {
/*
\\documentclass[fleqn]{article}
\\usepackage{amssymb,amsmath,bm,color}
\\usepackage[utf8]{inputenc}
\\begin{document}
\\pagestyle{empty}
\\mathindent0cm
\\parindent0cm
		$source$
\\end{document}

		dvipng  -T tight  -bg "Transparent" -D 150 -o test.png ens.dvi
*/
	if (!texte || !strcmp(texte.String(),"")) {
		*image = new BBitmap(BRect(0.0,0.0,0.0,0.0),B_RGBA32);
		return B_OK;
	}

	std::map<BString, BBitmap *>::iterator it=images.find(texte);
		
	if (it != images.end()) {
		*image = it->second;
		return B_OK;
	}
	else {
		char tmp[MAXPATHLEN]; 
		char *student;
		const char ** arg_v = (const char **)malloc(12*sizeof(char *));
		extern char** environ;
		status_t status;
		BString *latex_string = new BString();
		
		/* tmp dir */
	    find_directory(B_SYSTEM_TEMP_DIRECTORY,0,true,tmp,sizeof(tmp));
		chdir(tmp);

		//document
		switch(kind) {
			case TEXT:
			case LTEXT : 
				{
					latex_string->Append("\"\\documentclass[fleqn]{article}\\usepackage{amssymb,amsmath,bm,color}\\usepackage[utf8]{inputenc}\\begin{document}\\pagestyle{empty}\\mathindent0cm\\parindent0cm ");
					
					//Replace all _ with \_
					std::string *escaped_text = new std::string(texte.String()); 
					size_t pos = escaped_text->find("_");
					while( pos != std::string::npos)
					{
						escaped_text->replace(pos, 1, "\\_");
						pos = escaped_text->find("_", pos + 2);
					}

					//Replace all \n with \\
					
					pos = escaped_text->find('\n');
					while( pos != std::string::npos)
					{
						escaped_text->replace(pos, 1, " \\\\ ");
						pos = escaped_text->find('\n', pos + 4);
					}
					latex_string->Append(escaped_text->c_str());
					break;
				}
			case LMATH : 
				{
					latex_string->Append("\"\\documentclass[fleqn]{article} \\usepackage{amssymb,amsmath,bm,color} \\usepackage[utf8]{inputenc} \\begin{document} \\thispagestyle{empty} \\mathindent0cm \\parindent0cm ");		     
					latex_string->Append(texte);
					break;
				}
		}

		latex_string->Append(" \\end{document}\"");

		arg_v[0] = strdup("latex");
		arg_v[1] = strdup("-interaction=batchmode");
		arg_v[2] = strdup("-jobname=Student");
		arg_v[3] = latex_string->String();
		arg_v[4] = NULL;          

		for (int j = 0 ; j <5;j++) {
			printf(arg_v[j]);printf(" ");
		}
		printf("\n");
		fflush(stdout);
		thread_id latex_id = load_image(4, arg_v, (const char**)environ);
		wait_for_thread(latex_id, &status);

		arg_v[0] = strdup("dvipng");
		arg_v[1] = strdup("-T");
		arg_v[2] = strdup("tight");
		arg_v[3] = strdup("-bg");
		arg_v[4] = strdup("Transparent");
		arg_v[5] = strdup("-D");
		arg_v[6] = strdup("150");
		arg_v[7] = strdup("-o");
		arg_v[8] = strdup("Student.png");
		arg_v[9] = strdup("Student.dvi");
		arg_v[10] = strdup("-q");
		arg_v[11] = NULL;
		for (int j = 0 ; j <12;j++) {
			printf(arg_v[j]);printf(" ");
		}printf("\n");
		fflush(stdout);
		
		thread_id dvipng_id = load_image(11, arg_v, (const char**)environ);
		wait_for_thread(dvipng_id, &status);

		for (int i =0; i <11; i++) {
			free((void*)arg_v[i]);
		}
		free(arg_v);

		student = tmp;
		student = strcat(student, "/Student.png");


		*image = new BBitmap(BTranslationUtils::GetBitmap(student)->Bounds(), B_RGBA32, true);

		BView *composite = new BView((*image)->Bounds(), NULL, B_FOLLOW_NONE, B_WILL_DRAW);
		(*image)->AddChild(composite);

		rgb_color back_color = make_color(rgb_back_color.red, rgb_back_color.green, rgb_back_color.blue, 127);
		composite->LockLooper();
		composite->SetLowColor(back_color);
		composite->FillRect(composite->Bounds(), B_SOLID_LOW);
		composite->SetDrawingMode(B_OP_OVER);
		composite->DrawBitmap(BTranslationUtils::GetBitmap(student));
		composite->Sync();
		composite->UnlockLooper();
		(*image)->RemoveChild(composite);

		images.insert(std::pair<BString,BBitmap *>(texte,*image));

		BBitmapStream *strm = new BBitmapStream(*image);
		App::nb_images_produites++;

		app_info app_info;
		be_app->GetAppInfo(&app_info);


		BEntry *entry = new BEntry(&app_info.ref);
		BDirectory *directory=new BDirectory();
		entry->GetParent(directory);



		BDirectory *dir = new BDirectory(directory,"images");
		char file_name[32];
		sprintf(file_name, "%d",App::nb_images_produites);
		BPath *path = new BPath(dir,file_name);
		BFile *output = new BFile(path->Path(),B_CREATE_FILE|B_WRITE_ONLY);
		status_t err = BTranslatorRoster::Default()->Translate(strm, NULL, NULL, output, B_PNG_FORMAT, 0);
		if (err < B_OK) 
			return err;
		err = output->WriteAttr("latex",B_STRING_TYPE,0,texte.String(),texte.Length());
		return err;

	}
}

//TODO use correctly and remove all GetAppInfo elsewhere
BPath* App::GetTheoriesFolder() {
	return fTheoriesFolder;
}

void App::ChooseSaveFolder() {
	BFilePanel* filePanel = new BFilePanel(B_OPEN_PANEL, NULL, NULL, B_DIRECTORY_NODE, false,
			NULL, NULL, false, true);
	filePanel->Show();
}

MainWindow* App::GetMainWindow() {
	return fMainWin;
}


// #pragma mark - Modèles
BList* App::GetSignatures() {
	return fSignatures;
}

BList* App::GetTheories() {
	return fTheories;
}

status_t App::_InitModels() {
	//Création répertoire de travail
	app_info app_info;
	be_app->GetAppInfo(&app_info);

	BEntry app_entry = BEntry(&app_info.ref);
	char *data = NULL;
	app_entry.GetParent(fAppDirectory);

	if (!fAppDirectory->Contains("images", B_DIRECTORY_NODE)) {
		fAppDirectory->CreateDirectory("images",NULL);
	} 
	if (!fAppDirectory->Contains("signatures", B_DIRECTORY_NODE)) {
		fAppDirectory->CreateDirectory("signatures",NULL);
	} 
	if (!fAppDirectory->Contains("théories", B_DIRECTORY_NODE)) {
		fAppDirectory->CreateDirectory("théories",NULL);
	}

	//images existe
	BDirectory dir = BDirectory(fAppDirectory,"images");
	entry_ref entry_r ;
	while (dir.GetNextRef(&entry_r)!= B_ENTRY_NOT_FOUND) {
		BBitmap *image = BTranslationUtils::GetBitmap(&entry_r);

		//Reading attribute from FolderWatcher (BeOS sample code)
		BNode node = BNode(&entry_r);
		attr_info *attrInfo = new attr_info();
		status_t res_attrInfo = node.GetAttrInfo("latex", attrInfo) ;

		if (res_attrInfo != B_OK)
			continue;

		if(data)
			free(data);
		data = (char *)malloc(attrInfo->size+1);

		if (!data)
			return B_NO_MEMORY;

		if (node.ReadAttr("latex", attrInfo->type, 0, data, attrInfo->size) != attrInfo->size)
			continue;
		data[attrInfo->size]='\0';
		//ReadAttr doesn't set the nul ending of strings
		BString texte = BString(data);

		images.insert(std::pair<BString,BBitmap *>(texte,image));
		nb_images_produites++;
	}
	{
		//signatures existe
		BDirectory dir = BDirectory(fAppDirectory,"signatures");
		entry_ref entry_r;
		while (dir.GetNextRef(&entry_r)!= B_ENTRY_NOT_FOUND) {
			BFile file = BFile(&entry_r, B_READ_ONLY);
			BMessage message = BMessage();

			message.Unflatten(&file);
			class Signature *sig = cast_as(Signature::Instantiate(&message), class Signature);

			//TODO filter the entries by mime type in the while loop instead
			if(sig) {
				fSignatures->AddItem(sig);
			}
		}
	}
	{
		//théories existe
		BDirectory dir = BDirectory(fAppDirectory,"théories");
		entry_ref entry_r;
		while (dir.GetNextRef(&entry_r)!= B_ENTRY_NOT_FOUND) {
			BFile file = BFile(&entry_r, B_READ_ONLY);
			BMessage message = BMessage();

			message.Unflatten(&file);
			class Theorie *sig = cast_as(Theorie::Instantiate(&message), class Theorie);

			//TODO filter the entries by mime type in the while loop instead
			if(sig) {
				fTheories->AddItem(sig);
			}
		}
	}

	return B_OK;
}

BMessage* App::GetSettings() {
	return fSettings;
}

// #pragma mark - Serveur


void App::LaunchLocalServer(const uint16 port) {
	// Load the app image... and pass the args
	printf("%s\n","App::LaunchLocalServer");fflush(stdout);	
	int arg_c=4;
	char **arg_v;

	char * serverAddress = (char *)malloc(strlen("localhost:"+5));
	sprintf(serverAddress, "localhost:%d", port);
	arg_v = (char **)malloc(6*sizeof(char *));
	arg_v[0] = strdup("proof_server.exe");
	arg_v[1] = strdup("-v");
	arg_v[2] = strdup("-s");
	arg_v[3] = serverAddress;
	arg_v[4] = strdup("--verbosity=debug");
	arg_v[5] = NULL;
	extern char** environ;

	BPath dir_path;
	dir_path.SetTo(fAppDirectory);

	chdir(dir_path.Path());

	thread_id serverThread = load_image(arg_c, const_cast<const char**>(arg_v), const_cast<const char**>(environ));

	while (--arg_c >= 0) {
		free(arg_v[arg_c]);
	}
	free(arg_v);

	//Lancement thread proof_server
	resume_thread(serverThread);
}

	int
main(void)
{
	App *app = new App();
	app->Run();

	delete app;
	return 0;
}
