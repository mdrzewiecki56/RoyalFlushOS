#include "BetterShell.h"
#include <iostream>
#include <cstdio>
#include <iostream>       // std::cout
#include <string>         // std::string
#include <stdio.h>
#include <ctype.h>

BetterShell::BetterShell()
{
	this->mem->start();
	scheduler.add_first(mng->get_process("dummy").get());
	this->mem->LoadProgram(scheduler.first->file_name, scheduler.first->PID, scheduler.first);
	running = true;
}//e of kontruktor


BetterShell::~BetterShell()
{
}

void BetterShell::initialize()
{
	loop(); // uruchamianie g³ównej pêtli
}

void BetterShell::loop()
{
	while (running) {
		read_command();
		run_command();
	}
}

void BetterShell::read_command()
{
	std::cout << ">";
	parsed_command.resize(0);
	getline(std::cin, raw_command);
	std::string temporary = "";

	// Podziel stringa na "s³owa"
	for (const char &ch : raw_command) {
		if (ch != ' ')
		{
			temporary += ch;
		}
		else {
			parsed_command.push_back(temporary);
			temporary = "";
		}
	}
	parsed_command.push_back(temporary);
}

void BetterShell::run_command()
{
	std::string komenda = parsed_command[0];
	for (std::string::size_type i = 0; i < komenda.length(); ++i) {
		komenda[i]=toupper(komenda[i]);
	};

	if (komenda == "CF") { cf(); }
	else if (komenda == "RF") { rf(); }
	else if (komenda == "WF") { wf(); }
	else if (komenda == "DF") { df(); }
	else if (komenda == "RN") { rn(); }
	else if (komenda == "FD") { fd(); }
	else if (komenda == "DD") { dd(); }
	else if (komenda == "SD") { sd(); }

	/// MEMORY
	else if (komenda == "DMEMORY") { dmemory(); }
	//else if (komenda == "SHOWPT") { show_paget(); }
	else if (komenda == "FIFO") { show_fifo(); }
	else if (komenda == "DSF") { show_swapfile(); }
	/// PROCESS
	else if (komenda == "CP") { cp(); }
	else if (komenda == "RP") { rp(); }
	else if (komenda == "DLIST") { dlist(); }
	else if (komenda == "DPCB") { dpcb(); }
	else if (komenda == "DPROCESS") { dprocess(); }
	else if (komenda == "MONSIEUR") { recognation(); }
	//KOLEJKA
	else if (komenda == "PQ") { pq(); }
	else if (komenda == "RUNTIME") { runtime(); }
	/// INTERPRETER
	else if (komenda == "GO") { go(); }
	/// HELP
	else if (komenda == "CLS") { cls(); }
	else if (komenda == "HELP") { help(); }
	else if (komenda == "CREDITS") { credits(); }
	else if (komenda == "EXIT") { exit(); }
	else { other(); }
}

// Create file
void BetterShell::cf()
{
	if (parsed_command.size() == 1 || (parsed_command.size() == 2 && parsed_command[1] == "/?"))              // help                        
	{
		help_class.CREATEFILE_H();
	}
	else if (parsed_command.size() == 2) {
		this->disk->Create_File(parsed_command[1], "");
	}
	else if (parsed_command.size() == 3)                                                    // stworz plik bez tekstu
	{

		this->disk->Create_File(parsed_command[1], parsed_command[2]);


		//std::cout << "Utworzono plik o nazwie " << command_line[1] << std::endl;
	}
	else if (parsed_command.size() >= 4)                                                    // stworz plik z tekstem
	{

		std::string tekst;
		for (int i = 2; i < parsed_command.size(); i++)
		{
			if (i == parsed_command.size() - 1)
				tekst += parsed_command[i];
			else
				tekst += parsed_command[i] += ' ';
		}
		if (tekst.at(0) == '"'&&tekst.at(tekst.size() - 1) == '"')
		{
			std::string wpisz(tekst.begin() + 1, tekst.end() - 1);
			this->disk->Create_File(parsed_command[1], wpisz);
			//std::cout << "Utworzono plik o nazwie " << command_line[1] << " z podana zawartoscia: " << wpisz << std::endl;
		}
		else
		{
			this->disk->Create_File(parsed_command[1], tekst);
			//std::cout << "Utworzono plik o nazwie " << command_line[1] << " z podana zawartoscia: " << tekst << std::endl;
		}

	}
	else
	{
		help_class.HELP_F();
	}
}

// Read file
void BetterShell::rf()
{
	if (parsed_command.size() == 1 || (parsed_command.size() == 2 && parsed_command[1] == "/?"))
	{
		help_class.READFILE_H();
	}
	else if (parsed_command.size() == 2)
	{

		std::cout << "Zawartosc pliku o nazwie: " << parsed_command[1] << std::endl;
		std::cout << this->disk->Read_From_File(parsed_command[1]);
	}
	else
	{
		help_class.HELP_F();
	}
}

// Write file
void BetterShell::wf()
{
	if (parsed_command.size() == 1 || (parsed_command.size() == 2 && parsed_command[1] == "/?"))              // help                        
	{
		help_class.WRITEFILE_H();
	}
	else if (parsed_command.size() >= 3)                                                                 // stworz plik z tekstem
	{
		std::string tekst;

		for (unsigned int i = 2; i < parsed_command.size(); i++)
		{
			if (i == parsed_command.size() - 1)
				tekst += parsed_command[i];
			else
				tekst += parsed_command[i] += ' ';
		}

		if (tekst.at(0) == '"'&&tekst.at(tekst.size() - 3) == '"') {
			std::string wpisz(tekst.begin() + 1, tekst.end() - 1);
			this->disk->Write_To_File(parsed_command[1], wpisz);
			//std::cout << "Do pliku o nazwie " << command_line[1] << " wpisano zawartosc: " << wpisz << std::endl;
		}
		else {
			this->disk->Write_To_File(parsed_command[1], tekst);
			//std::cout << "Do pliku o nazwie " << command_line[1] << " wpisano zawartosc: " << tekst << std::endl;
		}
	}
	else
	{
		help_class.HELP_F();
	}
}

// Delete file
void BetterShell::df()
{
	if (parsed_command.size() == 1 || (parsed_command.size() == 2 && parsed_command[1] == "/?"))
	{
		help_class.DELETEFILE_H();
	}
	else if (parsed_command.size() == 2)
	{
		this->disk->Delete_File(parsed_command[1]);
		//std::cout << "Usunieto plik o nazwie " << command_line[1] << std::endl;
	}
	else
	{
		help_class.HELP_F();
	}
}

// Rename file
void BetterShell::rn()
{
	if (parsed_command.size() == 1 || (parsed_command.size() == 2 && parsed_command[1] == "/?"))
	{
		help_class.RENAMEFILE_H();
	}
	else if (parsed_command.size() == 3)
	{
		this->disk->Change_File_Name(parsed_command[1], parsed_command[2]);
		//std::cout << "Zmieniono nazwe pliku z "<< command_line[1] <<"na " << command_line[2] << std::endl;
	}
	else
	{
		help_class.HELP_F();
	}
}

// Format disk
void BetterShell::fd()
{
	if (parsed_command.size() == 2 && parsed_command[1] == "/?") {
		help_class.FORMATDISK_H();
	}

	if (parsed_command.size() == 1)
	{
		this->disk->Format_Disc();

	}
	else
	{
		help_class.HELP_F();
	}
}

// Show disk
void BetterShell::dd()
{
	if (parsed_command.size() == 2 && parsed_command[1] == "/?") {
		help_class.DISPLAYDISK_H();
	}
	if (parsed_command.size() == 1)
	{
		this->disk->show_data_container();

	}
	else
	{
		help_class.HELP_F();
	}
}

// Show directory
void BetterShell::sd()
{
	if (parsed_command.size() == 2 && parsed_command[1] == "/?") {
		help_class.SHOW_DIR_H();
	}

	if (parsed_command.size() == 1)
	{
		this->disk->show_directory();
		//std::cout << "Zawartosc katalogu: " << std::endl;
	}
	else
	{
		help_class.HELP_F();
	}
}

// Show memory
void BetterShell::dmemory()
{
	if (parsed_command.size() == 2 && parsed_command[1] == "/?")
	{
		help_class.SHOWMEMORY_H();
	}
	else if (parsed_command.size() == 1)
	{
		this->mem->showPMemory();
	//	this->mem->printFIFO();
	}

	else
	{
		help_class.HELP_F();
	}
}

void BetterShell::show_paget()
{
}

void BetterShell::show_fifo()
{
	if (parsed_command.size() == 2 && parsed_command[1] == "/?")
	{
		//help_class.SHOWMEMORY_H();
	}
	else if (parsed_command.size() == 1)
	{
		this->mem->printFIFO();
	}

	else
	{
		help_class.HELP_F();
	}
}

void BetterShell::show_swapfile()
{
	if (parsed_command.size() == 2 && parsed_command[1] == "/?")
	{
		//help_class.SHOWMEMORY_H();
	}
	else if (parsed_command.size() == 1)
	{
		this->mem->printSwapFile();
	}

	else
	{
		help_class.HELP_F();
	}
}

// Create process
void BetterShell::cp()
{
	if (parsed_command.size() == 1 || (parsed_command.size() == 2 && parsed_command[1] == "/?"))
	{
		help_class.CREATEPROCESS_H();
	}
	else if (parsed_command.size() >= 3) {
		std::ifstream myReadFile;
		myReadFile.open(parsed_command[parsed_command.size() - 1]);
		if (myReadFile) {
			for (int i = 1; i < parsed_command.size() - 1; i++) {
				this->mng->fork(parsed_command[i], parsed_command[parsed_command.size() - 1], this->mng->get_process("init"));
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
}

// Run process
void BetterShell::rp()
{
	if (parsed_command.size() == 2) {
		PCB* wsk = scheduler.first; //TEST

		scheduler.add(this->mng->get_process(parsed_command[1]).get());
		if(wsk!=scheduler.first){//TEST
		this->mem->LoadProgram(scheduler.first->file_name, scheduler.first->PID, scheduler.first);
		}//TEST
	}

	else { std::cout << "Wrong command"; }
}

// Show list
void BetterShell::dlist()
{
	if (parsed_command.size() == 2 && parsed_command[1] == "/?")
	{
		help_class.SHOWLIST_H();
	}

	if (parsed_command.size() >= 2) {
		help_class.SHOWPCB_H();
	}
	else this->mng->showlist();
}

// Show PCB
void BetterShell::dpcb()
{
	if (parsed_command.size() == 1 || (parsed_command.size() == 2 && parsed_command[1] == "/?"))
	{
		help_class.SHOWPCB_H();
	}
	else if (parsed_command.size() == 2)
	{
		this->mng->showpcb(parsed_command[1]);
	}
	else
	{
		help_class.HELP_F();
	}
}

// Delete process
void BetterShell::dprocess()
{
	if (parsed_command.size() == 1 || (parsed_command.size() == 2 && parsed_command[1] == "/?"))
	{
		help_class.DELETEPROCESS_H();
	}
	else if (parsed_command.size() >= 2) {
		for (int i = 1; i < parsed_command.size(); i++) {
			this->mng->exit(parsed_command[i]);
		}

	}
}

void BetterShell::recognation()
{
	if (parsed_command[1] == "BOND") {
		std::cout << "Access approved -Welcome 007" << "\n";
	}
	else { std::cout << "Error autodestruction process starts.\n"; 
	
	
	Sleep(100);
	exit();
	};
}

// Pqueue
void BetterShell::pq()
{
	if (parsed_command.size() == 2 && parsed_command[1] == "/?")
	{
		help_class.QUEUE_H();
	}
	else if (parsed_command.size() == 1) {

		scheduler.print_queue();
	}
	else
	{
		help_class.HELP_F();
	}
}

// Runtime
void BetterShell::runtime()
{
	if (parsed_command.size() == 2 && parsed_command[1] == "/?")
	{
		help_class.QUEUE_H();
	}
	else if (parsed_command.size() == 1) {

		//TUTAJ
		std::cout << "Already running program is: " << scheduler.first->name << "\n";
	}
	else
	{
		help_class.HELP_F();
	}
}

// Go
// TODO: MAKE THIS COMMAND
void BetterShell::go()
{
	//this->mem->showPMemory();
	//this->mem->printSwapFile();
	Interpreter interpreter(this->mng->get_process(scheduler.first->name).get(), this->mem.get(), this->disk.get(), this->mng.get());
	if (interpreter.interpretation() == false) {
		scheduler.remove(Terminated);
		this->mem->LoadProgram(scheduler.first->file_name, scheduler.first->PID, scheduler.first);//test
	}
	//scheduler.first->real_time += 1;
	std::cout << "scheduler.first" << scheduler.first->name << "eof \n";
}

// Cls
void BetterShell::cls()
{
	system("cls");
}

// Help
void BetterShell::help()
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
	std::cout << "FIFO - display FIFO order.\n";
	std::cout << "DSF - show Swap File.\n";
	std::cout << "CREDITS - display the credits.\n\n";
	std::cout << "EXIT - exit the system.\n";
}

// Credits
void BetterShell::credits()
{
	std::cout << "The creators:\n" << std::endl;
	std::cout << "Scheluder - Arkadiusz Dokowicz\n" << std::endl;
	std::cout << "Process Manager - Lukasz Swidziniewski\n" << std::endl;
	std::cout << "Interpreter - Maciej Drzewiecki\n" << std::endl;
	std::cout << "Virtual Memory - Maciej Anglart\n" << std::endl;
	std::cout << "RAM Memory - Michal Najborowski\n" << std::endl;
	std::cout << "Semaphores - Wojciech Mlynczak\n" << std::endl;
}

// Exit
void BetterShell::exit()
{
	char odp;
	std::cout << "Do you really wanna fold ? y/n ";
	std::cin >> odp;
	if (odp == 'y' || odp == 'Y') { running = false; }
}

// Other
void BetterShell::other()
{
	std::cout << "Command not found.\n";
}
