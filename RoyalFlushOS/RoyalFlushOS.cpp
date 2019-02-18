#include "dysk.h"
#include "inode.h"
#include "Interpreter.h"
#include "Memory.h"
#include "PCB.h"
#include "ProcessManager.h"
#include "Semaphore.h"
#include "BetterShell.h"
#include "Scheduler.h"
#include <windows.h>
#include <iostream>
#include <fstream>

void read_from_file(std::string file_name)
{
	std::ifstream file(file_name);
	if (file.is_open()) {
		std::string line;
		while (getline(file, line)) {
			printf("%s", line.c_str());
			std::cout << std::endl;
		}
		file.close();
	}
}

void GotoXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void intro()
{
	read_from_file("1.txt");
	Sleep(1000);
	system("cls");
	read_from_file("2.txt");
	Sleep(1000);
	system("cls");
	read_from_file("3.txt");
	Sleep(1000);
	system("cls");
	read_from_file("4.txt");
	Sleep(1000);
	system("cls");
	read_from_file("5.txt");
	Sleep(1000);
	read_from_file("6.txt");
	Sleep(2000);
	system("cls");
	read_from_file("7.txt");
	Sleep(1000);
	read_from_file("8.txt");
	Sleep(2000);
}

int main()
{
	//intro();
	system("cls");
	BetterShell shell;
	shell.initialize();

	getchar();
	return 0;
}
