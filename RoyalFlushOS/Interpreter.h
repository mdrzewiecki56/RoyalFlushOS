#pragma once
//#include <bitset>
//typedef std::bitset<16> command;
#include <vector>
#include "Interpreter.h"
#include "dysk.h"
#include "inode.h"
#include "ProcessManager.h"
#include "Memory.h"
#include "PCB.h"
#include <memory>

class Interpreter
{
private:
	dysk* disk;
	PCB* pcb;
	MemoryManager* mm;
	ProcessManager* pm;

	std::string getCommand();
	char static readByte(std::string &command);	//Czyta jeden znak rozkazu
	std::pair<int, int> static prepareCommand(std::string & command); //Odczytuje z komendy jej id i liczbe argumentow

	std::vector<std::string> static getArguments(std::string & command, std::pair<int, int> parameters); //pobiera argumenty w stringach z rozkazu

	void add(std::string reg, int value); //dodawanko
	void add(std::string reg, std::string address);
	void substract(std::string reg, int value); //odejmowanko
	void substract(std::string reg, std::string address);

	void multiply(std::string reg, int value); //mnozonko
	void multiply(std::string reg, std::string address);

	void divide(std::string reg, int value); //dzielonko
	void divide(std::string reg, std::string address);

	void decrement(std::string reg); //dekrementowanko

	void increment(std::string reg); //inkrementowanko

	void load(std::string reg, int value); //ladowanie wartosci do rejestru
	void load(std::string reg, std::string address);

	void equals(std::string address, int value); //czy rowne?
	void equals(std::string address, std::string address2);

	void selectFunction(const std::pair<int, int >&  CommandParameters, const std::vector<std::string>& Arguments); //wybieranie funkcji do wywolanie na podstawie rozkazu
	void readFile(std::string argument);

public:
	Interpreter(PCB* process, MemoryManager* manager, dysk* disk, ProcessManager* pm);

	void interpretation(std::string &command);
	void fullInterpretation();
	void printState();

};