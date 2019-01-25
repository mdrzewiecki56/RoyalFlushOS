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


	/*tablica rozkazow <KOD ROZKAZU <ID rozkazu/ilosc argumentow>>
	std::vector<std::pair<std::string, std::pair<int, int >>> CommandTab {
		//Arytmetyka
		std::make_pair("AD",std::make_pair(0,2)),	//Dodawanie
		std::make_pair("SU",std::make_pair(1,2)),	//Odejmowanie
		std::make_pair("MP",std::make_pair(2,2)),	//Mnozenie
		std::make_pair("DV",std::make_pair(3,2)),	//Dzielenie
		std::make_pair("DR",std::make_pair(4,1)),	//Dekrementacja
		std::make_pair("IR",std::make_pair(5,1)),	//Inkrementacja
		//Przypisywanie wartosci
		std::make_pair("LO",std::make_pair(6,2)),	//Zapisywanie do rejestru
		//Operacje na plikach
		std::make_pair("CF",std::make_pair(7,2)),	//Tworzenie pliku o nazwie
		std::make_pair("DF",std::make_pair(8,1)),	//Usuwanie pliku o nazwie
		std::make_pair("AF",std::make_pair(9,2)),	//Dopisywanie do pliku
		std::make_pair("CN",std::make_pair(10,2)),	//Zmiana nazwy pliku
		//Operacje logiczne

		//Procesy
		std::make_pair("CP",std::make_pair(11,1)),	//Tworzenie procesu
		std::make_pair("KP",std::make_pair(12,1)),	//Zabicie procesu
		//Programowe
		std::make_pair("JP",std::make_pair(19,1)),	//Skok
		std::make_pair("JZ",std::make_pair(20,2)),	//Skok jesli zero
		std::make_pair("SP", std::make_pair(98, 0)), //Koniec pracy
		std::make_pair("ER", std::make_pair(99, 0)) //Error - nie obslugiwane polecenie

	};
	*/

	std::string Get(std::vector<PageTableData>* page_table, int PID, int LR);
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

	void load(std::string reg, int value); //ladowanie wartosci do rejestru

	void equals(std::string address, int value); //czy rowne?

	void equals(std::string address, std::string address2);

	void load(std::string reg, std::string address);

	void increment(std::string reg); //inkrementowanko

	void selectFunction(const std::pair<int, int >&  CommandParameters, const std::vector<std::string>& Arguments); //wybieranie funkcji do wywolanie na podstawie rozkazu
	void readFile(std::string argument);

public:
	dysk* disk;
	PCB* pcb;
	MemoryManager* mm;
	ProcessManager* pm;

	std::string command_cp;
	Interpreter(PCB* process, MemoryManager* manager, dysk* disk, ProcessManager* pm);

	std::string fullCommand; //kopia pelnego rozkazu nie wiem czy jeszcze potrzebna
	//Interpreter() : regA(0), regB(0), regC(0), regD(0), counter(0), PID(0) {};
	std::string getCommand();

	void interpretation(std::string &command);

	void fullInterpretation();

	void printState();


};