#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <string>
#include <map>
#include "PCB.h"
extern class PCB;

struct PageTableData {
	int frame; // ramka w ktorej znajduje sie stronica
	bool bit; //bit poprawnosci

	PageTableData(int frame, bool bit) {
		this->frame = frame;
		this->bit = bit;
	}
};

class MemoryManager {

public:
	struct Page {
		char data[16]; //dane w stronicy
		Page(std::string data) {
			while (data.size() < 16)
				data += " "; //dopelnia strone spacjami do osiagniecia rozmiaru 16
			for (int i = 0; i < 16; i++) {
				this->data[i] = data[i];
			}
		}
		Page() {}; //konstruktor zawsze jest argumentowy

		void print() {
			for (auto x : data) std::cout << x;
			std::cout << std::endl;
		}
	};

	struct FramelistData // struktura znajdujaca sie w liscie ramek ulatwiajaca wymiane stronic (nie trzeba przeszukiwac listy procesow);
	{
		bool free; // zajetosc ramki
		int PID; // PID
		int pageN; // numer_strony
		std::vector<PageTableData> *page_table; // wskaznik na tablice stronic
		FramelistData(bool free, int PID, int pageN, std::vector<PageTableData> *page_table) {
			this->free = free;
			this->PID = PID;
			this->pageN = pageN;
			this->page_table = page_table;
		};
	};

	//funkcja ladujaca podana strone do ramki w pamieci fizycznej
	//argumenty: wskaznik na obiekt klasy VirtualMemory, strona ladowana do pamieci, numer strony, PID procesu, tablica stron
	int LoadtoMemory(Page page, int pageN, int PID, std::vector<PageTableData> *page_table);

	std::list<int> FrameOrder;

	char RAM[128]; // pamiec fizyczna

	// tablice stron ma 8 wierszy dla kazdego procesu
	// frame = -1 oraz bit = 0 oznacza ze stronica nie zostala umieszczona w zadnej ramce (nie ma jej w pamieci fizycznej)
	// PageTable znajduje sie w klasie PCB
	std::vector<PageTableData> *createPageTable(int PID); //wskaznik na tablice stron

	//proces bezczynnosci umieszczany w pamieci przy starcie systemu oraz wypelnianie RAMu spacjami;
	void start();

	//funkcja wyswietlajaca pamiec od podanego indeksu o podanym zasiegu
	void showPMemory(int start, int range);

	//funkcja wyswietlajaca cala pamiec fizyczna
	void showPMemory();

	//funkcja wyswietlajaca tablice stronic
	static void showPageTable(std::vector<PageTableData> *page_table);

	//funkcja uruchamiajaca procedure ladowania programu
	//pierwsza strona zostaje zaladowana do RAMu, reszta do pliku wymiany
	int LoadProgram(std::string path, int PID);

	std::string Get(PCB *process, int LR);

	void Remove(int PID);

	//zapisywanie do pamieci fizycznej
	int Write(PCB *process, int adress, std::string data);

	std::map <int, std::vector<Page>> SwapFile;

	std::vector<FramelistData> Frames;

	int searchForFreeFrame(); //zwraca numer wolnej ramki

	int SwapPages(std::vector<PageTableData> *page_table, int pageNumber, int PID); //wyrzuca najstarsz� stronic� i w jej miejsce wrzucana jest nowa stronica; zwracany jest numer ramki

	void setFrameOrder(int frame); //ustawia porzadek wg FIFO

	void insertToSwapFile(std::vector<Page> pagevec, int PID);

	void printFIFO();
	void printSwapFile();

	~MemoryManager() {}
	MemoryManager() {
		for (int i = 0; i < 8; i++) {
			Frames.push_back(FramelistData(1, -1, -1, NULL));
		}
	}
};