#pragma once
#include <functional>
#include <queue>
#include <vector>
#include <iostream>
#include"PCB.h"
#include"ProcessManager.h"

class Scheduler
{



private:
	std::priority_queue <PCB*, std::vector<PCB*>, PCB> pr_queue;

public:
	PCB* already_running = NULL; //wskaznik na process ktory jest running
	PCB* first = NULL; //wskaznik na pierwszy proces gotowy do wykonania
	PCB* dummy = NULL; //wskaznik na dummy 
	void add(PCB* nowyproces);
	void run();//uruchomienie procesu
	void remove(process_state state);//usuwa proces ktory zostal wykonany lub nie mozna bylo go wykonac i jego stan zmieniono na waiting jako argument interpreter podaje Terminated lub Waiting 
	// po wykonaniu remove od razu nastepny proces zaczyna dzialac 
	//dodawanie procesu do kolejki gotowych procesow
	int size();//ilosc PCB w kolejce
	Scheduler();
	void print_queue();//wyswietla kolejke procesow oczekujacych
	void print_running_process();
	//todo set pointer for dummy process!
	~Scheduler();
};

