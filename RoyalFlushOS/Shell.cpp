#include "Shell.h"


Shell::spis_funkcji Shell::str_to_int(const std::string & funkcja) {
	///HDD
	if (funkcja == "CF") return CREATEFILE;
	else if (funkcja == "RF") return READFILE;
	else if (funkcja == "WF") return WRITEFILE;
	else if (funkcja == "DF") return DELETEFILE;
	else if (funkcja == "RN") return RENAMEFILE;
	else if (funkcja == "FD") return FORMATDISK;
	else if (funkcja == "DD") return DISPLAYDISK;
	else if (funkcja == "SD") return SHOWDIRECTORY;
	
	/// MEMORY
	else if (funkcja == "DMEMORY") return SHOWMEMORY;
	/// PROCESS
	else if (funkcja == "CP") return CREATEPROCESS;
	else if (funkcja == "RP") return RUNPROCESS;
	else if (funkcja == "DLIST") return SHOWLIST;
	else if (funkcja == "DPCB") return SHOWPCB;
	else if (funkcja == "DPROCESS") return DELETEPROCESS;

	//KOLEJKA
	else if (funkcja == "PQ") return PQUEUE;
	else if (funkcja == "RUNTIME") return RUNTIME;
	/// INTERPRETER
	//else if (Funkcja == "GO") return GO;
	else if (funkcja == "GO") return GO;
	/// HELP
	else if (funkcja == "CLS") return CLS;
	else if (funkcja == "HELP") return HELP;
	else if (funkcja == "CREDITS")return CREDITS;
	else if (funkcja == "EXIT") return EXIT;
	else return OTHER;
}

void Shell::to_upper(std::string &s) {
	std::transform(s.begin(), s.end(), s.begin(), ::toupper);
}

bool Shell::are_there_numbers(const std::string &s)
{
	for (unsigned int i = 0; i < s.size(); i++)
	{
		if (s[i] < 48 || s[i] >= 58)
			return false;
	}
	return true;
}

void Shell::error_d()
{
	std::cout << "Command not found.\n";
}

void Shell::help()
{
		std::cout << "Commands: \n";
		std::cout << "CF - create a new file.\n";
		std::cout << "RF - read a file.\n";
		std::cout << "WF - write in a file.\n";
		std::cout << "DF - delete a file.\n";
		std::cout << "RN - rename a file.\n";
		std::cout << "COPY - copy a file.\n\n";
		std::cout << "FD - format the disk.\n";
		std::cout << "DD - display the disk.\n";
		std::cout << "SD - display the content of the catalog.\n";
		std::cout << "CP - create a process / processes.\n\n";
		std::cout << "RP - run a process / processes.\n";
		std::cout << "DPROCESS - delete a process.\n";
		std::cout << "DPCB - display the Process Control Block of a process.\n";
		std::cout << "DLIST - display the process list.\n";
		std::cout << "PQ - display the process queue.\n";
		std::cout << "RUNTIME - display the running process.\n";
		std::cout << "DMEMORY - display the content of RAM memory.\n";
		std::cout << "CREDITS - display the credits.\n\n";
		std::cout << "EXIT - exit the system.\n";
	
}

void Shell::credits()
{
	std::cout << "The creators:\n" << std::endl;
	std::cout << "Interface - Kornelia Maik\n" << std::endl;
	std::cout << "Files and catalogs - Radoslaw Leszkiewicz\n" << std::endl;
	std::cout << "Scheluder - Arkadiusz Dokowicz\n" << std::endl;
	std::cout << "Process Manager - Lukasz Swidziniewski\n" << std::endl;
	std::cout << "Interpreter - Maciej Drzewiecki\n" << std::endl;
	std::cout << "Virtual Memory - Maciej Anglart\n" << std::endl;
	std::cout << "RAM Memory - Michal Najborowski\n" << std::endl;
	std::cout << "Semaphores - Wojciech Mlynczak\n" << std::endl;
}

void Shell::rundummy()
{
	this->first_dummy_run = true;
	this->mng->get_process("dummy")->set_state(Ready);
	scheduler.add(this->mng->get_process("dummy").get());
	scheduler.run();
	scheduler.dummy = this->mng->get_process("dummy").get();
	this->mem->LoadProgram("dummy.txt", mng->get_process("dummy")->PID, scheduler.already_running);
	scheduler.print_queue();
	this->mem->showPMemory();
	this->mem->printSwapFile();
	Interpreter interpreter(this->mng->get_process(scheduler.already_running->name).get(),this->mem.get(),this->disk.get(),this->mng.get());
	while (interpreter.interpretation())
	{
		this->run();
		this->running = true;
	}

	scheduler.remove(Terminated);
}




void Shell::command() {

	std::string zdanie;
	std::getline(std::cin, zdanie);                                 // pobranie linijki wypisanej przez uzytkownika
	std::smatch sm;
	std::regex regex("[a-zA-Z0-9_!@#$%\"*^&()\\[\\]+-={}';.,/?]+|[1-9][0-9]*");   // minimum 1 literka, string zaczyna sie od obojetnie czego
																				  // liczba zaczyna sie od 1: czyli nie mo¿na zrobiæ 001 - wlasciwie to uzytkownik moze to wpisac, ale my zobaczymy tylko 1...
																				  // no i nasz parametr specjalny: /? (wywolanie pomocy)

	while (std::regex_search(zdanie, sm, regex))                    // za pomoca wyrazenia regularnego wyfiltrowanie rzeczy, ktore wypisal uzytkownik
	{
		command_line.push_back(sm.str());
		zdanie = sm.suffix();
	}
	if (command_line.size() < 1)                                    // jesli ktos podal zdanie zawierajacy tylko: " \ / | % ! - = * +, to wtedy size jest rowny 0, gdyz regex go nie wylapie
	{
		std::cout << "::> ";
		command_line.clear();
	}
	else if (/*letters(command_line[0])*/true)                    // jesli w pierwszym wyrazie sa literki
	{
		to_upper(command_line[0]);                          // zmieniamy je na duze litery: cF -> CF / cf -> CF
	}                                                               // w CMD kolejnosc nie ma znaczenia, rowniez chcialem aby moj shell byl podobny
	else                                                            // jezeli ktos wpisze np. tylko: ";)" "$$" to wtedy command_line.size() == 0, czyli wywolujemy ze ma podac w 1. parametrze funkcje
	{
		//error_d();
		std::cout << "::> ";
		command_line.clear();
	}
}


 void Shell::run()
{
	 if (!this->first_dummy_run) {
		 this->rundummy();
	 }
	//this->mem->start();
	//this->mng->get_process("dummy")->set_state(Ready);
	//scheduler.add(this->mng->get_process("dummy").get());
	//scheduler.run();
	//scheduler.dummy = this->mng->get_process("dummy").get();
	//this->mem->LoadProgram("dummy.txt", mng->get_process("dummy")->PID, scheduler.already_running);	//wczytanie dummy do ram

	do {

		std::cout << "::> ";                                   // wypisanie naszego "znaku poczatku komendy" - czy jak to nazwac
		command_line.clear();                                    // wyczyszczenie command_line z poprzedniej komendy (tej ktora zostala wykonana)
		while (command_line.size() <= 0)    //chujowykod                     // jezeli ktos podal np. "||" jako nazwe funkcji, to petla sie powtarza lub jesli sa jakies nieprawidlowe parametry
		{
			command();                                           // wywolanie funkcji pobierajacej komende od uzytkownika
		}
		switch_case();                                           // w przypadku komendy, ktora wydaje sie w miare "poprawna" wywolujemy switch_case()

	} while (running);                                           // program bedzie sie wykonywal w nieskonczonosc dopoki uzytkownik go nie przerwie
}


void Shell::switch_case()
{
	switch (str_to_int(command_line[0]))
	{

		/// DYSK

	case Shell::spis_funkcji::CREATEFILE:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))              // help                        
		{
			help_class.CREATEFILE_H();
		}
		else if (command_line.size() == 2) {
			this->disk->Create_File(command_line[1], "");
		}
		else if (command_line.size() == 3)                                                    // stworz plik bez tekstu
		{

			this->disk->Create_File(command_line[1], command_line[2]);


			//std::cout << "Utworzono plik o nazwie " << command_line[1] << std::endl;
		}
		else if (command_line.size() >= 4)                                                    // stworz plik z tekstem
		{

			std::string tekst;
			for (int i = 2; i < command_line.size(); i++)
			{
				if (i == command_line.size() - 1)
					tekst += command_line[i];
				else
					tekst += command_line[i] += ' ';
			}
			if (tekst.at(0) == '"'&&tekst.at(tekst.size() - 1) == '"')
			{
				std::string wpisz(tekst.begin() + 1, tekst.end() - 1);
				this->disk->Create_File(command_line[1], wpisz);
				//std::cout << "Utworzono plik o nazwie " << command_line[1] << " z podana zawartoscia: " << wpisz << std::endl;
			}
			else
			{
				this->disk->Create_File(command_line[1], tekst);
				//std::cout << "Utworzono plik o nazwie " << command_line[1] << " z podana zawartoscia: " << tekst << std::endl;
			}

		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}
	case Shell::spis_funkcji::READFILE:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.READFILE_H();
		}
		else if (command_line.size() == 2)
		{

			std::cout << "Zawartosc pliku o nazwie: " << command_line[1] << std::endl;
			std::cout << this->disk->Read_From_File(command_line[1]);
		}
		else
		{
			help_class.HELP_F();
		}

		break;
	}
	case Shell::spis_funkcji::WRITEFILE:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))              // help                        
		{
			help_class.WRITEFILE_H();
		}
		else if (command_line.size() >= 3)                                                                 // stworz plik z tekstem
		{
			std::string tekst;

			for (unsigned int i = 2; i < command_line.size(); i++)
			{
				if (i == command_line.size() - 1)
					tekst += command_line[i];
				else
					tekst += command_line[i] += ' ';
			}

			if (tekst.at(0) == '"'&&tekst.at(tekst.size() - 3) == '"') {
				std::string wpisz(tekst.begin() + 1, tekst.end() - 1);
				this->disk->Write_To_File(command_line[1], wpisz);
				//std::cout << "Do pliku o nazwie " << command_line[1] << " wpisano zawartosc: " << wpisz << std::endl;
			}
			else {
				this->disk->Write_To_File(command_line[1], tekst);
				//std::cout << "Do pliku o nazwie " << command_line[1] << " wpisano zawartosc: " << tekst << std::endl;
			}
		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}
	case Shell::spis_funkcji::DELETEFILE:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.DELETEFILE_H();
		}
		else if (command_line.size() == 2)
		{
			this->disk->Delete_File(command_line[1]);
			//std::cout << "Usunieto plik o nazwie " << command_line[1] << std::endl;
		}
		else
		{
			help_class.HELP_F();
		}

		break;
	}
	case Shell::spis_funkcji::RENAMEFILE:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.RENAMEFILE_H();
		}
		else if (command_line.size() == 3)
		{
			this->disk->Change_File_Name(command_line[1], command_line[2]);
			//std::cout << "Zmieniono nazwe pliku z "<< command_line[1] <<"na " << command_line[2] << std::endl;
		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}
	case Shell::spis_funkcji::FORMATDISK:
	{
		if (command_line.size() == 2 && command_line[1] == "/?") {
			help_class.FORMATDISK_H();
		}

		if (command_line.size() == 1)
		{
			this->disk->Format_Disc();

		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}

	case Shell::spis_funkcji::COPYFILE:
	{
		if ((command_line.size() == 1) || (command_line.size() == 2 && command_line[1] == "/?"))
		{

			help_class.COPYFILE_H();
		}
		else if (command_line.size() == 3)
		{
			this->disk->Copy_File(command_line[1], command_line[2]);
		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}

	/*case OPENFILE:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.DELETEFILE_H();
		}
		else if (command_line.size() == 2)
		{
			dysk.open_file(command_line[1]);
		}
		else
		{
			help_class.HELP_F();
		}

		break;
	}
	case CLOSEFILE:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.DELETEFILE_H();
		}
		else if (command_line.size() == 2)
		{
			dysk.close_file(command_line[1]);
		}
		else
		{
			help_class.HELP_F();
		}

		break;
	}*/
	case Shell::spis_funkcji::DISPLAYDISK:
	{
		if (command_line.size() == 2 && command_line[1] == "/?") {
			help_class.DISPLAYDISK_H();
		}
		if (command_line.size() == 1)
		{
			this->disk->show_data_container();
			
		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}

	case Shell::spis_funkcji::SHOWDIRECTORY:
	{
		if (command_line.size() == 2 && command_line[1] == "/?") {
			help_class.SHOW_DIR_H();
		}

		if (command_line.size() == 1)
		{
			this->disk->show_directory();
			//std::cout << "Zawartosc katalogu: " << std::endl;
		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}



	/// MEMORY
	case SHOWMEMORY:
	{
		if (command_line.size() == 2 && command_line[1] == "/?")
		{
			help_class.SHOWMEMORY_H();
		}
		else if (command_line.size() == 1)
		{
			this->mem->showPMemory();
		}
		
		else
		{
			help_class.HELP_F();
		}
		break;

	}

	/// PROCESS
	case Shell::spis_funkcji::CREATEPROCESS:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.CREATEPROCESS_H();
		}
		else if (command_line.size() >= 3) {
			std::ifstream myReadFile;
			myReadFile.open(command_line[command_line.size()-1]);
			if (myReadFile) {
				for (int i = 1; i < command_line.size() - 1; i++) {
					this->mng->fork(command_line[i], command_line[command_line.size() - 1], this->mng->get_process("init"));
					//scheduler.add(mng.get_process(command_line[i]).get());
				}
				myReadFile.close();
			}
			else std::cout << "The file you've typed in doesn't exist.\n";
		}

		else
		{
			help_class.HELP_F();
		}
		break;
	}

	case Shell::spis_funkcji::RUNPROCESS:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?")) {
			help_class.RUNPROCESS_H();
		}
		else if(command_line.size() == 2){

			

			
				this->mng->get_process(command_line[1])->set_state(Ready);
				scheduler.add(this->mng->get_process(command_line[1]).get());
				scheduler.print_queue();
				scheduler.remove(Waiting);
				//mem.LoadProgram(mng.get_process(command_line[1]).get(), mng.get_process(command_line[1]).get()->file_name, mng.get_process(command_line[1])->PID);
				//this is alternative function of the upper f.
				this->mem->LoadProgram(scheduler.already_running->file_name, scheduler.already_running->PID,scheduler.already_running);

				//mem.showPageTable(mem.createPageTable(mng.get_process(command_line[1])->PID));
				this->mem->showPMemory();
				this->mem->printSwapFile();


				Interpreter interpreter( this->mng->get_process(scheduler.already_running->name).get(), this->mem.get(), this->disk.get(), this->mng.get());
				//std::cout<<interpreter.pcb->predicted_time;
				
				
				while (interpreter.interpretation())
				{
					this->run();
					this->running = true;
				}
				
				scheduler.remove(Terminated);
				//std::cout << "scheduler" << scheduler.dummy->predicted_time;
			
		}
		else if (command_line.size() > 2) {
			
				for (int i = 1; i < command_line.size(); i++) {
					scheduler.add(this->mng->get_process(command_line[i]).get());
					this->mng->get_process(command_line[i])->set_state(Ready);
					//std::cout << "wywolalem sie";
				}
				scheduler.remove(Waiting);
				for (int i = 1; i < command_line.size(); i++) {




					//std::cout << "already running" << scheduler.already_running->name << "\n";
					this->mem->LoadProgram(scheduler.already_running->file_name, scheduler.already_running->PID,scheduler.already_running);
					//mem.LoadProgram(scheduler.already_running, scheduler.already_running->file_name, scheduler.already_running->PID);
					//mem.showPageTable(mem.createPageTable(mng.get_process(command_line[1])->PID));
					this->mem->showPMemory();
					this->mem->printSwapFile();



					Interpreter interpreter(scheduler.already_running, this->mem.get(), this->disk.get(), this->mng.get());
					//std::cout<<interpreter.pcb->predicted_time;
					//interpreter.fullInterpretation();
					scheduler.remove(Terminated);
					//std::cout << "scheduler" << scheduler.dummy->predicted_time;
				}
			
		}
		
		
		else
		{
			help_class.HELP_F();
		}
		break;
	}
	case Shell::spis_funkcji::SHOWLIST:
	{
		if (command_line.size() == 2 && command_line[1] == "/?")
		{
			help_class.SHOWLIST_H();
		}

		if (command_line.size() >= 2) {
			help_class.SHOWPCB_H();
		}
		else this->mng->showlist();
		break;
	}
	case Shell::spis_funkcji::SHOWPCB:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.SHOWPCB_H();
		}
		else if (command_line.size() == 2)
		{
			this->mng->showpcb(command_line[1]);
		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}
	case Shell::spis_funkcji::DELETEPROCESS:
	{
		if (command_line.size() == 1 || (command_line.size() == 2 && command_line[1] == "/?"))
		{
			help_class.DELETEPROCESS_H();
		}
		else if (command_line.size() >= 2) {
			for (int i = 1; i < command_line.size(); i++) {
				this->mng->exit(command_line[i]);
			}
			
		}
		break;
	}

	case Shell::spis_funkcji::PQUEUE:
	{

		if (command_line.size() == 2 && command_line[1] == "/?")
		{
			help_class.QUEUE_H();
		}
		else if (command_line.size() == 1) {

			scheduler.print_queue();
		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}

	case Shell::spis_funkcji::RUNTIME:
	{

		if (command_line.size() == 2 && command_line[1] == "/?")
		{
			help_class.QUEUE_H();
		}
		else if (command_line.size() == 1) {

			//TUTAJ
			std::cout <<"Already running program is: "<< scheduler.already_running->name<<"\n";
		}
		else
		{
			help_class.HELP_F();
		}
		break;
	}

	/// INTERPRETER
	/*case GO:
	{
		planista.check( tree); //ustawianie procesu running
		//std::cout << &pcb.PID << " [Shell]\n";
		inter.WykonajProgram(mm, *troll, planista, tree, pipeline, dysk);
		break;
	}*/
	/// MY
	case Shell::spis_funkcji::HELP:
	{
		help();
		break;
	}

	case Shell::spis_funkcji::CLS:
	{
		system("cls");
		break;
	}
	case Shell::spis_funkcji::EXIT:
	{
		//dysk.save_to_file();
		//dysk.show_all_info();
		
		exit();
		break;
	}
	case Shell::spis_funkcji::GO:
	{
		running = false;
		break;
	}
	case Shell::spis_funkcji::CREDITS:
	{
		credits();
		break;
	}
	case Shell::spis_funkcji::OTHER:
	{
		error_d();
		break;
	}

	}
}


void Shell::exit()
{
	char odp;
	std::cout << "Czy na pewno chcesz wyjsc z systemu? t/n ";
	std::cin >> odp;
	if (odp == 't' || odp == 'T') {running = false;}
}



Shell::Shell()
{
	this->mem->start();
	
}


Shell::~Shell()
{
}
