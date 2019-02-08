#pragma once
#include <vector>
#include "PCB.h"

class Semaphore
{
private:
	int value;
	std::vector<PCB> waiting_queue;
public:
	Semaphore();
	Semaphore(int initial_value);
	~Semaphore();
	void wait(PCB &new_process);
	void signal();
	void show_queue_state();
};
