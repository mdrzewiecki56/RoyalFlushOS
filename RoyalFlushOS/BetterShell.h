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
	/// PROCESS
	void cp();
	void rp();
	void dlist();
	void dpcb();
	void dprocess();

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
	void run_command();
};

