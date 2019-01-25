#include "PCB.h"
#include "ProcessManager.h"

using namespace std;

//Settery
void PCB::set_state(const process_state &state)
{
	this->state = state;
	std::cout << "\nChanging state of process: " << this->name << " Current state: ";
	switch (state)
	{
	case 0:
		std::cout << " new\n"; break;
	case 1:
		std::cout << " ready\n"; break;
	case 2:
		std::cout << " running\n"; break;
	case 3:
		std::cout << " waiting\n"; break;
	case 4:
		std::cout << " terminated\n"; break;
	}
}

void PCB::set_predicted_time(const int &predicted_time)
{
	this->predicted_time = predicted_time;
}

void PCB::set_real_time(const int &real_time)
{
	this->real_time = real_time;
}

void PCB::set_reg1(const int &reg1)
{
	this->reg1 = reg1;
}

void PCB::set_reg2(const int &reg2)
{
	this->reg2 = reg2;
}

void PCB::set_reg3(const int &reg3)
{
	this->reg3 = reg3;
}

void PCB::set_command_counter(const int &value)
{
	this->command_counter = value;
}

//Gettery
process_state PCB::get_state()
{
	return this->state;
}

int PCB::get_real_time()
{
	return this->real_time;
}

int PCB::get_predicted_time()
{
	return this->predicted_time;
}

int PCB::get_reg1()
{
	return this->reg1;
}

int PCB::get_reg2()
{
	return this->reg2;
}

int PCB::get_reg3()
{
	return this->reg3;
}

int PCB::get_command_counter()
{
	return this->command_counter;
}

