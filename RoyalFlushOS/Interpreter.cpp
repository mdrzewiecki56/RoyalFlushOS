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
	command = mm->Get(this->pcb, this->pcb->get_command_counter());
	this->pcb->command_counter+=command.length() + 1;
	std::cout << "Komenda:" << command<< std::endl;
	return command;
}
char Interpreter::readByte(std::string &command)
{
	if (command.empty())
		return ' ';
	char ch = command[0];
	command.erase(0, 1);
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
		std::make_pair("MR",std::make_pair(13,3)),
		std::make_pair("MW",std::make_pair(14,2)),

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
//na podstawie parametrow z bazy pobiera argumenty
std::string Interpreter::getArguments(std::pair<int, int> parameters)
{
	std::string arg1, arg2, arg3;
	for (int i = 0; i < parameters.second; i++)
	{
		if (i == 0)
		{
			arg1 += mm->Get(this->pcb, this->pcb->get_command_counter());
			arg1 += " ";
			this->pcb->command_counter += arg1.length();
		}
		else if (i == 1)
		{
			arg2 += mm->Get(this->pcb, this->pcb->get_command_counter());
			if (parameters.second > 2)
			{
				arg2 += " ";
				this->pcb->command_counter += arg2.length();
			}
			else
				this->pcb->command_counter += arg2.length() + 1;
		}
		else if (i == 2)
		{
			arg3 += mm->Get(this->pcb, this->pcb->get_command_counter());
			this->pcb->command_counter += arg3.length() + 1;
		}

	}
	std::cout << "Argumenty:" << arg1 << "," <<arg2 << ","<<arg3 << "," << std::endl;
	return (arg1 + arg2 + arg3);
}


//teraz funkcja ktora na podstawie pary wyzej odyczta argumenty funkcji tu znajduje sie poki co lista funckji
std::vector<std::string> Interpreter::prepareArguments(std::string &arguments, std::pair<int, int> parameters)
{
	std::vector<std::string> arguments_string;
	std::string tmp;
	int argumentsConverted = 0;
	char ch;
	while (argumentsConverted != parameters.second)
	{
		//std::cout << std::endl << command << std::endl;
		do
		{
			ch = readByte(arguments);
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
		int value = std::stoi(mm->Get(this->pcb, std::stoi(sOutput)));
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
		int value = std::stoi(mm->Get(this->pcb, std::stoi(sOutput)));
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
		int value = std::stoi(mm->Get(this->pcb, std::stoi(sOutput)));
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
		int value = std::stoi(mm->Get(this->pcb, std::stoi(sOutput)));
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

//PAMIEC - do zrobienia
void Interpreter::readMemory(const std::string reg, std::string mem_str, int range)
{
	mem_str = std::regex_replace(mem_str, std::regex(R"([\D])"), "");
	std::string memContent = mm->Read(std::stoi(mem_str),range);
	std::cout << mm->Read(std::stoi(mem_str), range) << "!";
	if (reg == "AX")
		pcb->reg1 = std::stoi(memContent);
	else if (reg == "BX")
		pcb->reg2 = std::stoi(memContent);
	else if (reg == "CX")
		pcb->reg3 = std::stoi(memContent);
	else if (reg == "DX")
		pcb->reg4 = std::stoi(memContent);
}
void Interpreter::writeMemory(const int value, std::string mem_str)
{
	mem_str = std::regex_replace(mem_str, std::regex(R"([\D])"), "");
	std::cout << "\nmiejsce do zapisu:" << mem_str;
	std::cout << "\wartosc do zapisu:" << value;
	mm->Write(this->pcb, std::stoi(mem_str), std::to_string(value));
}

void Interpreter::writeMemory(const std::string reg, std::string mem_str)
{
	int value = 0;
	if (reg == "AX")
		value = pcb->reg1;
	else if (reg == "BX")
		value = pcb->reg2;
	else if (reg == "CX")
		value = pcb->reg3;
	else if (reg == "DX")
		value = pcb->reg4;

	mem_str = std::regex_replace(mem_str, std::regex(R"([\D])"), "");
	std::cout << "\nmiejsce do zapisu:" <<  mem_str;
	std::cout << "\wartosc do zapisu:" << value;
	mm->Write(this->pcb, std::stoi(mem_str), std::to_string(value));


}

//SKOKI
void Interpreter::jumpIfZero(std::string reg, int counter)
{
	if (reg == "AX"	&& this->pcb->reg1 == 0)
		pcb->set_command_counter(counter);
	else if (reg == "BX" && this->pcb->reg2 == 0)
		pcb->set_command_counter(counter);
	else if (reg == "CX" && this->pcb->reg3 == 0)
		pcb->set_command_counter(counter);
	else if (reg == "DX" && this->pcb->reg4 == 0)
		pcb->set_command_counter(counter);
}

void Interpreter::selectFunction(const std::pair<int, int >&  CommandParameters, const std::vector<std::string>& Arguments)
{
	/*std::cout << "Parametry: " << CommandParameters.first << "," << CommandParameters.second << std::endl;
	for (auto i : Arguments)
	{
		std::cout << i << std::endl;
	}
	*/

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
	case 13://MR - czytaj z pamieci
		std::cout << "arg1:" << Arguments[0] << ";\n" << "arg2:" << Arguments[1] << ";\n" << "arg3:" << Arguments[2] << ";";
		readMemory(Arguments[0], Arguments[1], std::stoi(Arguments[2]));
		break;
	case 14://MW - zapisz do pamieci
		if (Arguments[0][0] != '[' && Arguments[0][1] != 'X')
		{
			int arg2 = std::stoi(Arguments[0]);
			writeMemory(arg2, Arguments[1]);
		}
		else
			writeMemory(Arguments[0], Arguments[1]);
		break;

		//ROZWIAZANE NA INNYM POZIOMIE
	case 19://JP = Skok bezwarunkowy
		this->pcb->command_counter = std::stoi(Arguments[0]);
		break;
	case 20://JZ = Skok jesli zero gdzies
		jumpIfZero(Arguments[0], std::stoi(Arguments[1]));
		break;
	case 98:
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

bool Interpreter::interpretation()
{
	std::string command = getCommand();
	std::pair<int, int> prep = prepareCommand(command);
	std::string arg_string = getArguments(prep);
	std::vector<std::string> args = prepareArguments(arg_string, prep);

	selectFunction(prep, args);
	printState();
	if (prep.first == 98)
	{
		this->pcb->set_command_counter(0);
		return false;
	}
	else
		return true;
}
/*
void Interpreter::fullInterpretation()
{
	while (command != "SP")
	{
		command = getCommand();
		if (command != "SP")
			interpretation(command);
		std::cout << "Counter: " << this->pcb->command_counter << std::endl;
		pcb->real_time++;
	}

}*/