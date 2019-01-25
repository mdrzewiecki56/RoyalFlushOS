#pragma once
#include<string>
#include<vector>
#include<algorithm>
#include<iostream>
#include<regex>
#include"Help.h"
#include "dysk.h"
#include "inode.h"
#include "Interpreter.h"
#include "Memory.h"
#include "PCB.h"
#include "ProcessManager.h"
#include "Semaphore.h"
#include "Shell.h"
#include "Scheduler.h"

#include<windows.h>
#include<time.h>
#include<fstream>

class Shell
{
private:

	/*STRUKTURY DANYCH*/
	bool running=true; //warunek dzialania programu
	std::vector<std::string> command_line; //komenda
	enum spis_funkcji {
		CREATEFILE, READFILE, WRITEFILE, DELETEFILE, RENAMEFILE, COPYFILE, DISPLAYFILE,
		FORMATDISK, DISPLAYDISK, SHOWDIRECTORY,
		CREATEPROCESS, RUNPROCESS, DELETEPROCESS, SHOWLIST, SHOWPCB,
		PQUEUE, RUNTIME,
		SHOWMEMORY,
		HELP, OTHER, CLS, EXIT, CREDITS
	};

	Help help_class;
public:
	std::shared_ptr<MemoryManager> mem = std::make_shared<MemoryManager>();
	std::shared_ptr<ProcessManager> mng = std::make_shared<ProcessManager>();
	std::shared_ptr<dysk> disk = std::make_shared<dysk>();
	Scheduler scheduler;

	/*METODY G��WNE*/
	 void run(); //dzialanie programu
	void command(); //obsluga komand
	void switch_case();

	/*FORMATOWANIE LINII*/
	spis_funkcji str_to_int(const std::string & funkcja);
	void to_upper(std::string & s);
	bool letters(const std::string & s);
	bool are_there_numbers(const std::string &s);

	/*METODY POMOCNICZE*/
	void error_d();
	void help();
	
	void credits();
	
	

	void exit();
	void r_u_sure();

public:
	Shell();
	~Shell();
};

