#pragma once

#include "PCB.h"
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <memory>

class ProcessManager
{
public:
	std::vector<std::shared_ptr<PCB>> processes;
	
	ProcessManager() 
	{
		fork("init", "", nullptr); //automatycznie tworzy proces init o PID = 0 przy tworzeniu obiektu ProcessManager
		fork(); //dummy proces
	};

	~ProcessManager() {};
private:
	void fork();																				//fork dla procesu dummy
public:
	void fork(const std::string &name, const std::string &file);								//tworzy proces o danej nazwie (rodzicem jest init)
	void fork(const std::string &name, std::shared_ptr<PCB> parent);							//tworzy proces o danej nazwie i wiaze go z rodzicem (do tworzenia dzieci przez rodzica)
	void fork(const std::string &name, const std::string &file, std::shared_ptr<PCB> parent);	//tworzy proces o danej nazwie i wiaze go z rodzicem (do tworzenia dzieci przez rodzica), ale przyjmuje rowniez nazwe pliku jezeli z niego jest tworzony
	void exit(const std::string &name);															//usuwa proces o danej nazwie
	void showlist();																			//wyswietla wszystkie procesy
	void showpcb(const std::string &name);														//wyswietla informacje o procesie o danej nazwie
	std::shared_ptr<PCB> get_process(const std::string &name);									//zwraca wskaznik proces o danej nazwie

};