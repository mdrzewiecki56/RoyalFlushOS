#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Memory.h"
#include "dysk.h"
#include "Interpreter.h"
#include "Help.h"
#include "ProcessManager.h"
#include "Scheduler.h"

class BetterShell
{
private:
	bool running;
	std::string raw_command;
	std::vector<std::string> parsed_command;
	Help help_class;

	std::shared_ptr<MemoryManager> mem = std::make_shared<MemoryManager>();
	std::shared_ptr<ProcessManager> mng = std::make_shared<ProcessManager>();
	std::shared_ptr<dysk> disk = std::make_shared<dysk>();
	Scheduler scheduler;

	// komendy
	void cf();
	void rf();
	void wf();
	void df();
	void rn();
	void fd();
	void dd();
	void sd();
	
	/// MEMORY
	void dmemory();
	void show_paget();
	void show_fifo();
	void show_swapfile();

	/// PROCESS
	void wait();
	//filenames
	void fn();
	//
	void cp();
	void rp();
	void dlist();
	void dpcb();
	void dprocess();
	void recognation();
	//KOLEJKA
	void pq();
	void runtime();
	/// INTERPRETER
	//"GO"
	void go();
	/// HELP
	void cls();
	void help();
	void credits();
	void exit();
	void other();
public:
	BetterShell();
	~BetterShell();
	void initialize();
	void loop();
	void read_command();
	void run_command()
	{
		std::string komenda = parsed_command[0];
		for (std::string::size_type i = 0; i < komenda.length(); ++i) {
			komenda[i] = toupper(komenda[i]);
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
		else if (komenda == "FN") { fn(); }
		else if (komenda == "WP") { wait(); }
		else if (komenda == "RP") {
			if (this->mng->get_process(parsed_command[1]).get()->file_name == "testowy.txt" || this->mng->get_process(parsed_command[1]).get()->file_name == "nwd.txt"
				|| this->mng->get_process(parsed_command[1]).get()->file_name == "silnia.txt" || this->mng->get_process(parsed_command[1]).get()->file_name == "pamiec.txt"
				|| this->mng->get_process(parsed_command[1]).get()->file_name == "pliki.txt" || this->mng->get_process(parsed_command[1]).get()->file_name == "cpkp.txt" 
				|| this->mng->get_process(parsed_command[1]).get()->file_name == "procent.txt") {
				rp();
			}
			else {
				std::cout << "You have typed wrong command\n";
			}
		
		}
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
	
};

