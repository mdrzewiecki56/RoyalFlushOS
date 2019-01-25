#include "Interpreter.h"
#include "dysk.h"
#include "inode.h"
#include "ProcessManager.h"
#include "Memory.h"
#include <iostream>
#include <algorithm> 
#include <string> 
#include <regex>

Interpreter::Interpreter(PCB* process, MemoryManager* manager, dysk* disk, ProcessManager* pm) {

	this->disk = disk;
	this->pcb = process;
	this->mm = manager;
	this->pm = pm;
}

std::string Interpreter::getCommand()
{
	std::string command;
	//funkcja z pamieci
	//command = mm->nullptr;

	return command;
}
char Interpreter::readByte(std::string &command)
{
	if (command.empty())
		return ' ';
	char ch = command[0];
	command.erase(0, 1);
	//this->counter++;
	//std::cout << ch << std::endl;

	return ch;
}
//odczytuje dwa pierwsze bajty wiem co to za komenda, ma id i wiem ile argumentow odczytac
std::pair<int, int>  Interpreter::prepareCommand(std::string &command)
{
	std::string commandCode;
	commandCode += readByte(command);
	commandCode += readByte(command);
	std::transform(commandCode.begin(), commandCode.end(), commandCode.begin(), ::toupper);
	//std::cout << commandCode;

	std::vector<std::pair<std::string, std::pair<int, int >>> CommandTab{
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
		std::make_pair("CP",std::make_pair(11,2)),	//Tworzenie procesu
		std::make_pair("KP",std::make_pair(12,1)),	//Zabicie procesu

		//Pamiec
		std::make_pair("RM",std::make_pair(13,2)),
		std::make_pair("WM",std::make_pair(14,2)),

		//Dysk
		std::make_pair("RF",std::make_pair(15,1)),

		//Programowe
		std::make_pair("JP",std::make_pair(19,1)),	//Skok
		std::make_pair("JZ",std::make_pair(20,2)),	//Skok jesli zero
		std::make_pair("SP", std::make_pair(98, 0)), //Koniec pracy
		std::make_pair("ER", std::make_pair(99, 0)) //Error - nie obslugiwane polecenie

	};

	std::pair<int, int>commandParams;

	for (int i = 0; i < CommandTab.size(); i++)
	{
		if (CommandTab[i].first == commandCode)
		{
			commandParams.first = CommandTab[i].second.first;
			commandParams.second = CommandTab[i].second.second;
			return commandParams;
		}
	}

	return std::make_pair(99, 0);

}

//teraz funkcja ktora na podstawie pary wyzej odyczta argumenty funkcji tu znajduje sie poki co lista funckji
std::vector<std::string> Interpreter::getArguments(std::string &command, std::pair<int, int> parameters)
{
	std::vector<std::string> arguments_string;
	std::string tmp;
	int argumentsConverted = 0;
	char ch;
	readByte(command);
	while (argumentsConverted != parameters.second)
	{
		//std::cout << std::endl << command << std::endl;
		do
		{
			ch = readByte(command);
			//std::cout << ch << std::endl;
			if (ch != ' ')
				tmp += ch;
		} while (ch != ' ');

		//std::cout << tmp;
		arguments_string.push_back(tmp);
		tmp = "";
		argumentsConverted++;
	}
	return arguments_string;
}

///FUNKCJE INTERPRETERA

//Dodawanie

void Interpreter::add(std::string reg, int value)
{

	if (reg == "AX")
		this->pcb->reg1 += value;
	else if (reg == "BX")
		this->pcb->reg2 += value;
	else if (reg == "CX")
		this->pcb->reg3 += value;
	else if (reg == "DX")
		this->pcb->reg4 += value;

}

void Interpreter::add(std::string reg, std::string address)
{
	int value = 0;

	if (address == "AX")
		value = pcb->reg1;
	else if (address == "BX")
		value = pcb->reg2;
	else if (address == "CX")
		value = pcb->reg3;
	else if (address == "DX")
		value = pcb->reg4;
	else
	{
		std::string sOutput = std::regex_replace(address, std::regex(R"([\D])"), "");
		int value = std::stoi(sOutput);
	}

	if (reg == "AX")
		pcb->reg1 += value;
	else if (reg == "BX")
		pcb->reg2 += value;
	else if (reg == "CX")
		pcb->reg3 += value;
	else if (reg == "DX")
		pcb->reg4 += value;

}

//Odejmowanie

void Interpreter::substract(std::string reg, int value)
{

	if (reg == "AX")
		pcb->reg1 -= value;
	else if (reg == "BX")
		pcb->reg2 -= value;
	else if (reg == "CX")
		pcb->reg3 -= value;
	else if (reg == "DX")
		pcb->reg4 -= value;

}

void Interpreter::substract(std::string reg, std::string address)
{

	int value = 0;
	if (address == "AX")
		value = pcb->reg1;
	else if (address == "BX")
		value = pcb->reg2;
	else if (address == "CX")
		value = pcb->reg3;
	else if (address == "DX")
		value = pcb->reg4;
	else
	{
		std::string sOutput = std::regex_replace(address, std::regex(R"([\D])"), "");
		int value = std::stoi(sOutput);
	}

	if (reg == "AX")
		pcb->reg1 -= value;
	else if (reg == "BX")
		pcb->reg2 -= value;
	else if (reg == "CX")
		pcb->reg3 -= value;
	else if (reg == "DX")
		pcb->reg4 -= value;

}

//Mnozenie
void Interpreter::multiply(std::string reg, int value)
{

	if (reg == "AX")
		pcb->reg1 *= value;
	else if (reg == "BX")
		pcb->reg2 *= value;
	else if (reg == "CX")
		pcb->reg3 *= value;
	else if (reg == "DX")
		pcb->reg4 *= value;

}

void Interpreter::multiply(std::string reg, std::string address)
{

	int value = 0;
	if (address == "AX")
		value = pcb->reg1;
	else if (address == "BX")
		value = pcb->reg2;
	else if (address == "CX")
		value = pcb->reg3;
	else if (address == "DX")
		value = pcb->reg4;
	else
	{
		std::string sOutput = std::regex_replace(address, std::regex(R"([\D])"), "");
		int value = std::stoi(sOutput);
	}

	if (reg == "AX")
		pcb->reg1 *= value;
	else if (reg == "BX")
		pcb->reg2 *= value;
	else if (reg == "CX")
		pcb->reg3 *= value;
	else if (reg == "DX")
		pcb->reg4 *= value;

}

//Dzielenie
void Interpreter::divide(std::string reg, int value)
{
	if (reg == "AX")
		pcb->reg1 /= value;
	else if (reg == "BX")
		pcb->reg2 /= value;
	else if (reg == "CX")
		pcb->reg3 /= value;
	else if (reg == "DX")
		pcb->reg4 /= value;

}

void Interpreter::divide(std::string reg, std::string address)
{

	int value = 0;
	if (address == "AX")
		value = pcb->reg1;
	else if (address == "BX")
		value = pcb->reg2;
	else if (address == "CX")
		value = pcb->reg3;
	else if (address == "DX")
		value = pcb->reg4;
	else
	{
		std::string sOutput = std::regex_replace(address, std::regex(R"([\D])"), "");
		int value = std::stoi(sOutput);
	}

	if (reg == "AX")
		pcb->reg1 /= value;
	else if (reg == "BX")
		pcb->reg2 /= value;
	else if (reg == "CX")
		pcb->reg3 /= value;
	else if (reg == "DX")
		pcb->reg4 /= value;

}

//Dekrementacja
void Interpreter::decrement(std::string address)
{
	int value = 0;

	if (address == "AX")
		pcb->reg1--;
	else if (address == "BX")
		pcb->reg2--;
	else if (address == "CX")
		pcb->reg3--;
	else if (address == "DX")
		pcb->reg4--;
	else
	{
		std::string sOutput = std::regex_replace(address, std::regex(R"([\D])"), "");
		int value = std::stoi(sOutput);
		//pomniejszanie miejsca w pamieci
	}
	//to trzeba poprawic jak dostane funkcje pobierania z pamieci

}
//Inkrementacja
void Interpreter::increment(std::string address)
{

	int value = 0;
	if (address == "AX")
		pcb->reg1++;
	else if (address == "BX")
		pcb->reg2++;
	else if (address == "CX")
		pcb->reg3++;
	else if (address == "DX")
		pcb->reg4++;
	else
	{
		std::string sOutput = std::regex_replace(address, std::regex(R"([\D])"), "");
		int value = std::stoi(sOutput);
		//powiekszanie miejsca w pamieci
	}
	//to trzeba poprawic jak dostane funkcje pobierania z pamieci
}
//Przypisywanie wartosci
void Interpreter::load(std::string reg, int value)
{

	if (reg == "AX")
		pcb->reg1 = value;
	else if (reg == "BX")
		pcb->reg2 = value;
	else if (reg == "CX")
		pcb->reg3 = value;
	else if (reg == "DX")
		pcb->reg4 = value;

}
void Interpreter::load(std::string reg, std::string address)
{

	int value = 0;
	if (address == "AX")
		value = pcb->reg1;
	else if (address == "BX")
		value = pcb->reg2;
	else if (address == "CX")
		value = pcb->reg3;
	else if (address == "DX")
		value = pcb->reg4;
	else
	{
		std::string sOutput = std::regex_replace(address, std::regex(R"([\D])"), "");
		int value = std::stoi(sOutput);
	}
	//to trzeba poprawic jak dostane funkcje pobierania z pamieci

	if (reg == "AX")
		pcb->reg1 = value;
	else if (reg == "BX")
		pcb->reg2 = value;
	else if (reg == "CX")
		pcb->reg3 = value;
	else if (reg == "DX")
		pcb->reg4 = value;
}

//DYSK
void Interpreter::readFile(std::string argument)
{
	std::string num_str = this->disk->Read_From_File(argument);

	this->pcb->reg4 = std::stol(num_str);

}
//OPERACJE LOGICZNE

//Rowne - nie skonczone

//Rozne

//Wieksze

//Mniejsze

//Wieksze rowne

//Mniejsze rowne

//PAMIEC - do zrobienia
void readMemory(const std::string register, const std::string mem_str)
{
	//int registerIndex = std::stoi(register);
	//std::string memContetn = mm->readString(ActiveProcess, stoi(mem_str));
	//ActiveProcess->registers[registerIndex] = stoi(memContetn);
}
void writeMemory(const std::string register, const std::string mem_str)
{
	//int registerIndex = stoi(Arguments[0]);
	//string memContetn = to_string(ActiveProcess->registers[registerIndex]);
	//mm->writeString(ActiveProcess, stoi(Arguments[1]), memContetn);
}

//do obrobki
void Interpreter::selectFunction(const std::pair<int, int >&  CommandParameters, const std::vector<std::string>& Arguments)
{
	if (CommandParameters.second == 2)
		this->pcb->command_counter += 2 + 1 + Arguments[0].length() + 1 + Arguments[1].length() + 1;
	if (CommandParameters.second == 1)
		this->pcb->command_counter += 2 + 1 + Arguments[0].length() + 1;
	int value;
	switch (CommandParameters.first)
	{
	case 0://AD = Dodawanie
		if (Arguments[1][0] != '[' && Arguments[1][1] != 'X')
		{
			value = std::stoi(Arguments[1], nullptr);
			//std::cout << "Wartosc dodawana:" << value << std::endl;
			add(Arguments[0], value);
		}
		else
			add(Arguments[0], Arguments[1]);
		break;
	case 1://SU = Odejmowanie
		if (Arguments[1][0] != '[' && Arguments[1][1] != 'X')
		{
			value = std::stoi(Arguments[1], nullptr);
			substract(Arguments[0], value);
		}
		else
			substract(Arguments[0], Arguments[1]);
		break;
	case 2://MP = MNOzENIE
		if (Arguments[1][0] != '[' && Arguments[1][1] != 'X')
		{
			value = std::stoi(Arguments[1], nullptr);
			multiply(Arguments[0], value);
		}
		else
		{
			//std::cout << Arguments[0] << " " << Arguments[1];
			multiply(Arguments[0], Arguments[1]);
		}

		break;
	case 3://DV = DZIELENIE
		if (Arguments[1][0] != '[' && Arguments[1][1] != 'X')
		{
			value = std::stoi(Arguments[1], nullptr);
			divide(Arguments[0], value);
		}
		else
			divide(Arguments[0], Arguments[1]);
		break;
	case 4://DR = DEKREMENTACJA
		decrement(Arguments[0]);
		break;
	case 5://IR = INKREMENTACJA
		increment(Arguments[0]);
		break;
	case 6://LO = PRZYPISYWANIE WARTOsCI
		if (Arguments[1][0] != '[' && Arguments[1][1] != 'X')
		{
			int arg2 = std::stoi(Arguments[1]);
			load(Arguments[0], arg2);
		}
		else
			load(Arguments[0], Arguments[1]);
		break;
		break;

		//OPERACJE NA PLIKACH
	case 7://CF = Utworz plik
		disk->Create_File(Arguments[0], Arguments[1]);
		disk->show_data_container();
		break;
	case 8://DF = Usuwanie pliku
		disk->Delete_File(Arguments[0]);
		break;
	case 9://AF = Dopisywanie do pliku
		disk->Write_To_File(Arguments[0], Arguments[1]);
		break;
	case 10://CN - zmiana nazwy pliku
		disk->Change_File_Name(Arguments[0], Arguments[1]);
		break;
	case 15:
		this->readFile(Arguments[0]);
		break;

		//PROCESY
	case 11://CP = Utworz proces
		pm->fork(Arguments[0], Arguments[1], pm->get_process(this->pcb->name));
		break;
	case 12://KP - zabij proces
		pm->exit(Arguments[0]);
		break;

		//PAMIEC
	case 13:
		readMemory(Arguments[0], Arguments[1]);
		break;
	case 14:
		writeMemory(Arguments[0], Arguments[1]);
		break;

		//ROZWIAZANE NA INNYM POZIOMIE
	case 19://JP = Skok bezwarunkowy
		this->pcb->command_counter = std::stoi(Arguments[0]);
		break;
	case 20://JZ = Skok jesli zero gdzies
		this->pcb->command_counter = std::stoi(Arguments[1]);
		break;
	case 23://SP = Koniec programu
		//end();
		break;

		//INNE
	case 99:
		throw "Nieznana Komenda!";
		break;
	default:
		throw "Nieznana Komenda!";

		break;
	}

}


void Interpreter::printState()
{
	std::cout << "\nRegisters AX: " << pcb->reg1 << " BX: " << pcb->reg2 << " CX: " << pcb->reg3 << " DX: " << pcb->reg4 <<std::endl;
}

void Interpreter::interpretation(std::string &command)
{
	std::pair<int, int> prep = prepareCommand(command);
	//std::cout << prep.first << " " << prep.second;
	std::vector<std::string> args = getArguments(command, prep);
	//std::cout << args[0] << " " << args[1];
	//std::cout << std::endl << prep.first;
	if (prep.first == 19)
	{
		command = this->command_cp.substr(stoi(args[0]));
	}

	if (prep.first == 20)
	{
		if (args[0] == "AX")
		{
			if (pcb->reg1 == 0)
			{
				command = this->command_cp.substr(stoi(args[1]));
			}
		}
		else if (args[0] == "BX")
		{
			if (pcb->reg2 == 0)
			{
				command = this->command_cp.substr(stoi(args[1]));
			}
		}
		else if (args[0] == "CX")
		{
			if (pcb->reg3 == 0)
			{
				command = this->command_cp.substr(stoi(args[1]));
			}
		}
		else if (args[0] == "DX")
		{
			if (pcb->reg4 == 0)
			{
				command = this->command_cp.substr(stoi(args[1]));
			}
		}
	}
	selectFunction(prep, args);
	pcb->command_counter += 2;

	/*if (args.size() == 1)
		this->counter+= 3 + args[0].length();
	if (args.size() == 2)
		this->counter += 3 + args[0].length() + 1 + args[1].length();
	printState();*/
	//std::cout << command << std::endl;
	printState();
}

void Interpreter::fullInterpretation()
{
	std::string command = getCommand();
	while (command != "SP")
	{
		interpretation(command);
		pcb->real_time++;
	}
	

}
/*
int main()
{
	Interpreter interpreter;
	std::string command = "LO AX 6 LO BX AX SU BX 1 JZ BX 49 MP AX BX JP 17 SP";
	interpreter.fullCommand = command;
	interpreter.fullInterpretation(command);
	system("pause");
	return 0;
}
*/