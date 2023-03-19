#include <iostream>
#include <string>

#include <poll.h>

#include "server_protocol.pb.h"

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
	google::protobuf::ShutdownProtobufLibrary();
}

/**
 * Launch ocaml
 * Verifies that signatures,théories and images directories are present
 */
void App::ReadyToRun(){
	_InitModels(); 
/*

create two pipes with pipe(...), one for stdin, one for stdout.
You could also redirect stderr, but usually stderr is meant for
messages that should not be redirected.

fork(...) the process.

in the child process (the one where fork(...) returns 0) dup
(...) the pipes to stdin/stdout.

exec[v][e] the to be started programm file in the child process.

In the parent process (the one where fork) returns the PID of the
child) do a loop that reads from the child's stdout (
select(...) or poll(...), read(...) ) into a buffer, until the
child terminates ( waitpid(...) ). Eventually supply the child
with input on stdin if it expects some.

When done close(...) the pipes.

man 2 pipe
man 2 fork
man 2 dup2
man 2 waitpid
man 2 select
man 2 poll
man 2 read
man 2 close

Wolfgang Draxinger
--
E-Mail address works, Jabber: hexarith@jabber.org, ICQ: 134682867
*/	
	
	pipe(inputocaml);
	pipe(outputocaml);
	pipe(errorocaml);
	int *fd;
	
	int oldStdIn;
	int oldStdOut;
	int oldStdErr;
	
	fd = (int *)malloc(2*sizeof(int));			
	
	//copie des anciens STD*_FILENO
	oldStdIn  = dup(STDIN_FILENO);
	oldStdOut = dup(STDOUT_FILENO);
	oldStdErr = dup(STDERR_FILENO);
	
	//fermeture
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	
	//création des nouveaux STD*_FILENO
	dup2(inputocaml[0] , STDIN_FILENO);
	dup2(outputocaml[1], STDOUT_FILENO);
	dup2(errorocaml[1] , STDERR_FILENO);
	
	

	// Load the app image... and pass the args
	int arg_c=9;
	char **arg_v;
	
	arg_v = (char **)malloc(10*sizeof(char *));
	arg_v[0] = strdup("ocaml");
	arg_v[1] = strdup("-I");
	arg_v[2] = strdup("signatures");
	arg_v[3] = strdup("-I");
	arg_v[4] = strdup("Preuves/_build/util");
	arg_v[5] = strdup("-I");
	arg_v[6] = strdup("Preuves/_build/prop");
	arg_v[7] = strdup("-I");
	arg_v[8] = strdup("Preuves/_build/premier_ordre");
	
	arg_v[9] = NULL;
	
	BPath dir_path;
	dir_path.SetTo(fAppDirectory);

	chdir(dir_path.Path());
	extern char** environ;
	/*
	FILE *sortie = fdopen(oldStdOut, "w");
	for(char* s =*environ; s; s = *++environ)
	{
		fprintf(sortie, s);
		fprintf(sortie, "\n");
		fflush(sortie);
	}
	*/
	thread_id ocamlThread = load_image(arg_c, const_cast<const char**>(arg_v), const_cast<const char**>(environ));
	
	while (--arg_c >= 0)
	   free(arg_v[arg_c]);
	
	free(arg_v);

	//Récupération des STD*_FILENO
	dup2(oldStdIn,  STDIN_FILENO);
	dup2(oldStdOut, STDOUT_FILENO);
	dup2(oldStdErr, STDERR_FILENO);

	//Fermeture des copies temporaires
	close(oldStdIn);
	close(oldStdOut);
	close(oldStdErr);

	//Lancement thread ocaml
	resume_thread(ocamlThread);
	
	
	fd[0]=inputocaml[1];
	fd[1]=outputocaml[0];

	thread_id thid = spawn_thread(&boucle,"boucle",B_LOW_PRIORITY, fd);
	resume_thread(thid);

	//ferme la lecture sur stdin
	close(inputocaml[0]);
	//on garde l'écriture sur stdout de ocaml pour y insérer le texte entré
	//close(outputocaml[1]);	

	SendOCaml("#load \"util.cmo\";;\n");

	SendOCaml("#load \"formule_prop.cmo\";;\n");
	SendOCaml("#load \"axiomes_prop.cmo\";;\n");
	
	SendOCaml("#load \"signature.cmo\";;\n");
	SendOCaml("#load \"base_terme.cmo\";;\n");
	SendOCaml("#load \"terme.cmo\";;\n");
	SendOCaml("#load \"formule.cmo\";;\n");
	SendOCaml("#load \"axiomes_premier_ordre.cmo\";;\n");
	SendOCaml("#load \"theorie.cmo\";;\n");
	SendOCaml("open Signature;;\n");
	
}
		
// #pragma mark - Boucle
status_t boucle(void *fd) {
	
	
	int fdi=(reinterpret_cast<int*>(fd))[0];
	int fdo=(reinterpret_cast<int*>(fd))[1];
	return ((App *)be_app)->boucle_interactive(fdi,fdo);
}
		
int32 App::boucle_interactive(int fdin, int fdout) {
	ReadOCaml();	
		
	return B_OK;
}
// #pragma mark - Ocaml
void App::SendOCaml(BString texte) {
	if (texte.Length() > 0) {

		//envoyer texte à ocaml
		write(inputocaml[1],texte,texte.Length());

		//dupliquer à l'écran

		BTextView *outView = (BTextView *)fMainWin->FindView("OutputView");

		if (fMainWin->LockLooper()) {
			outView->Insert(texte, texte.Length());
			fMainWin->UnlockLooper();
		}
	}
}

BString *App::ReadOCaml() {
	char c[2];
	FILE *f;
	BString *texte = new BString();
	BTextView *outView = (BTextView *)fMainWin->FindView("OutputView");
	
	c[1] = '\0';
	f=fdopen(outputocaml[0],"r");
	//boucle
	do {
		
		c[0] = getc(f);
		texte->Append(c);
		if (fMainWin->LockLooper()) {
			outView->Insert(c);
			fMainWin->UnlockLooper();
		}
	} while (c[0] != EOF);
	return texte;
}

bool App::QuitRequested(void)
{
	write(inputocaml[1], "#quit;;\n", 8);
	close(inputocaml[1]);
	close(outputocaml[0]);
	close(outputocaml[1]);
	
	return BApplication::QuitRequested();
}

void App::MessageReceived(BMessage *m) 
{
	switch (m->what)
	{
		case(kEnvoi):
		{
			const char * text= fMainWin->GetInputText();
			//envoyer texte à ocaml
			SendOCaml(BString(text));
			SendOCaml(BString("\n"));
			
			BWindow *w =this->WindowAt(0);
				if (w->LockLooper()) {
					//réinitialiser texte
					((BTextView *)w->FindView("InputView"))->SetText("");
					w->UnlockLooper();
				}
			break;
		}
		
		default:
		{
			BApplication::MessageReceived(m);
			break;
		}
	}
}

status_t App::latexToPNG(const BString& texte, BBitmap **image, LView_kind kind, rgb_color *rgb_back_color) {
/*
\\documentclass[fleqn]{article}
\\usepackage{amssymb,amsmath,bm,color}
\\usepackage[latin1]{inputenc}
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
			case LTEXT : 
				{
					latex_string->Append("\"\\documentclass[fleqn]{article}\\usepackage{amssymb,amsmath,bm,color}\\usepackage[latin1]{inputenc}\\begin{document}\\pagestyle{empty}\\mathindent0cm\\parindent0cm ");
					
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
					latex_string->Append("\"\\documentclass[fleqn]{article} \\usepackage{amssymb,amsmath,bm,color} \\usepackage[latin1]{inputenc} \\begin{document} \\thispagestyle{empty} \\mathindent0cm \\parindent0cm ");		     
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

		rgb_back_color->set_to(rgb_back_color->red, rgb_back_color->green, rgb_back_color->blue, 127);
		composite->LockLooper();
		composite->SetLowColor(*rgb_back_color);
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
	arg_v[4] = strdup("--verbosity=info");
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
