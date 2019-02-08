#include "Semaphore.h"
#include <iostream>

Semaphore::Semaphore()
{
	value = 1;
	//std::cout << "Utworzono semafor z wartoscia " << value << std::endl;
}

Semaphore::Semaphore(int initial_value)
{
	value = initial_value;
	//std::cout << "Utworzono semafor z wartoscia " << value << std::endl;
}

Semaphore::~Semaphore()
{
	//std::cout << "Zniszczono semafor" << std::endl;
}

void Semaphore::wait(PCB &process)
{
	value--;
	if (value < 0) {
		process.set_state(process_state::Waiting);
		waiting_queue.push_back(process);
	}
	std::cout << "Semaphore value has been decreased by 1" << std::endl;
}

void Semaphore::signal()
{
	value++;
	if (value <= 0) {
		waiting_queue.front().set_state(process_state::Running);
		waiting_queue.erase(waiting_queue.begin());
	}
	std::cout << "Semaphore value has been increased by 1" << std::endl;
}

void Semaphore::show_queue_state()
{
	std::cout << "Procesy w kolejce semafora:" << std::endl;
	for (auto proc : waiting_queue) {
		std::cout << "Nazwa: " << proc.name << " | PID: " << proc.PID << std::endl;
	}
}
