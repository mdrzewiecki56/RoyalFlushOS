#include "Scheduler.h"

void Scheduler::add_first(PCB * nowyproces)
{
	std::cout << nowyproces->name << " -> This process was added to the queue with ready processes" << "\n";
	pr_queue.push(nowyproces);//poprostu dodaje do kolejki kolejny proces a kolejka sama policzy 
	first = pr_queue.top();

	this->print_queue();
	if (first != this->pr_queue.top()) {
		first->set_state(Ready);
		first = this->pr_queue.top();
	}
	first->set_state(Running);
}
void Scheduler::add(PCB * nowyproces)
{
	int real_time_from_process;
	int predicted_time_from_process;
	//przelicza aktualnie wykonywany

	real_time_from_process = this->pr_queue.top()->real_time;
	predicted_time_from_process = this->pr_queue.top()->predicted_time;
	this->pr_queue.top()->predicted_time = (int)((0.5*real_time_from_process) + ((1 - 0.5)*predicted_time_from_process)); std::cout << "predicted time done\n";
	this->pr_queue.top()->real_time = 0; std::cout << "real time done\n";//czyscimy bo to juz nie aktualne;

	if ((this->pr_queue.top()->name == "dummy")) {
		this->pr_queue.top()->predicted_time = 999999999;
		this->pr_queue.top()->real_time = 0;
		this->pr_queue.top()->set_state(Ready);
	}
	std::cout << nowyproces->name << " -> This process was added to the queue with ready processes" << "\n";
	pr_queue.push(nowyproces);//poprostu dodaje do kolejki kolejny proces a kolejka sama policzy 
	
	
	this->print_queue();
	if (first != this->pr_queue.top()) {
		first->set_state(Ready);
		first = this->pr_queue.top();
	}
	first->set_state(Running);
}
void Scheduler::run()
{

	//ProcessManager manager;
	this->pr_queue.top()->set_state(Running);
	already_running = this->pr_queue.top();
	pr_queue.pop();
	first = this->pr_queue.top();

}//unused
/*
void Scheduler::remove(process_state state)
{

		std::cout << this->already_running->name << " -> This process was expropriated or its commands were executed" << "\n";
		this->already_running->set_state(state); // tutaj albo waiting albo Terminated  zalezy co wywola sobie interpreter
		int real_time_from_removed_process;
		int predicted_time_from_process;

		//liczenie nastepnego predicted time
		real_time_from_removed_process = this->already_running->real_time;// tu jest ile instrukcji zostalo policzonych wlasciwie przerobionych przez interpreter
		predicted_time_from_process = this->already_running->predicted_time;
		//std::cout << "Last predicted time: " << predicted_time_from_process;
		this->already_running->predicted_time = (int)((0.5*real_time_from_removed_process) + ((1 - 0.5)*predicted_time_from_process));
		this->already_running->real_time = 0;//czyscimy bo to juz nie aktualne;

		//funkcja liczaca  predicted time
		if ((this->already_running->name == "dummy")) {
			already_running->predicted_time = 999999999;
			already_running->real_time = 0;
			already_running->set_state(Ready);
			this->add(already_running);
			already_running = this->pr_queue.top();
			pr_queue.pop();
			already_running->set_state(Running);
			if (!this->size() == 0) { first = this->pr_queue.top(); }//jezeli to ostatni proces z listy to nie bedzie wskaznika na first
			else { first = nullptr; }

		}
		else {
			if (this->size() > 0) { // jezeli kolejka wieksza od 0 to pierwszy z kolejki dodstaje process przydzial 
				this->pr_queue.top()->set_state(Running);
				already_running = this->pr_queue.top();
				pr_queue.pop();
				if (!this->size() == 0) { first = this->pr_queue.top(); }//jezeli to ostatni proces z listy to nie bedzie wskaznika na first
				else { first = nullptr; }
			}
			else // jezeli kolejka procesow jest pusta process dummy musi przejac prosessor
			{
				dummy->set_state(Running);
				already_running = dummy;
				first = nullptr;
			}
		}
		//std::cout << " New predicted time: " << already_running->predicted_time << "\n";
		//nadaj mu nowy przewidywany czas na podstawie jego licznika rozkazow
		//wystartuj nowy proces
		//this->run();
	
}
*/

void Scheduler::remove(process_state state) {
	int real_time_from_process;
	int predicted_time_from_process;
	//przelicza aktualnie wykonywany

	real_time_from_process = this->pr_queue.top()->real_time;
	predicted_time_from_process = this->pr_queue.top()->predicted_time;
	this->pr_queue.top()->predicted_time = (int)((0.5*real_time_from_process) + ((1 - 0.5)*predicted_time_from_process)); std::cout << "predicted time done\n";
	this->pr_queue.top()->real_time = 0; std::cout << "real time done\n";//czyscimy bo to juz nie aktualne;
	pr_queue.top()->set_state(state);
	pr_queue.pop();
	first = pr_queue.top();
	first->set_state(Running);

}

int Scheduler::size() {
	return pr_queue.size();
}
Scheduler::Scheduler()
{
}
void Scheduler::print_queue() {
	std::priority_queue <PCB*, std::vector<PCB*>, PCB> to_print = pr_queue;
	std::cout << "This is the queue of processes with ready state: " << "\n";
	while (!to_print.empty()) {
		std::cout << to_print.top()->name << " " << "T=" << to_print.top()->predicted_time << ",";
		to_print.pop();
	}
	std::cout << '\n';

}

void Scheduler::print_running_process()
{
	std::cout << "Already running process is: " << this->already_running->name << "\n";
}

Scheduler::~Scheduler()
{
}
