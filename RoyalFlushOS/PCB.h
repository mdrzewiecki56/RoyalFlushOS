#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <memory>
#include "Memory.h"	
extern struct PageTableData;

enum process_state { New = 0, Ready = 1, Running = 2, Waiting = 3, Terminated = 4 };


class PCB
{
public:
	std::string name;										//nazwa procesu
	int PID;												//unikalny identyfikator procesu
	std::shared_ptr<PCB> parent;							//wskaznik na rodzica
	std::vector<std::shared_ptr<PCB>> children;							//wektor zawierajacy wskazniki na dzieci
	process_state state;									//stan procesu								
	std::vector<PageTableData>* page_table;					//wektor stronic (Michal)
	std::string file_name;									//nazwa pliku w ktorym znajduje sie proces
	int real_time, predicted_time;							//czas wykonywania procesu (Arek)
	int reg1, reg2, reg3, reg4;								//rejestry
	int command_counter;									//licznik rozkazow
	
	//konstruktor init
	PCB()
	{
		this->PID = PID;
		this->parent = nullptr;
		this->state = New;
		this->real_time = NULL;
		this->reg1 = 0;
		this->reg2 = 0;
		this->reg3 = 0;
		this->reg4 = 0;
		this->command_counter = 0;
	}

	//destruktor
	~PCB() {}

	//komparator dla Arka
	bool operator ()(const PCB* pcb1, const PCB* pcb2) 
	{
		return  pcb1->predicted_time > pcb2->predicted_time;
	}

	//Settery
	void set_state(const process_state &state);
	void set_predicted_time(const int &predicted_time);
	void set_real_time(const int &real_time);
	void set_reg1(const int &reg1);
	void set_reg2(const int &reg2);
	void set_reg3(const int &reg3);
	void set_command_counter(const int &value);

	//Gettery
	process_state get_state();
	int get_real_time();
	int get_predicted_time();
	int get_reg1();
	int get_reg2();
	int get_reg3();
	int get_command_counter();

};