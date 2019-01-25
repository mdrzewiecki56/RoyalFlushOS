// Created by Wojciech Kasperski on 15-Oct-18.
#include "Memory.h"
#include "PCB.h"

//------------- Konstruktory i destruktory  --------------
MemoryManager::Page::Page(std::string data) {
	while (data.size() < 16) data += " "; // Uzupe�nianie argumentu spacjami, je�li jest za ma�y
	for (int i = 0; i < 16; i++) // Przepisywanie argumentu do stronicy
		this->data[i] = data[i];
}

MemoryManager::Page::Page() = default;

PageTableData::PageTableData(bool bit, int frame) : bit(bit), frame(frame) {}

PageTableData::PageTableData() {
	this->bit = false;
	this->frame = -1;
};

MemoryManager::FrameData::FrameData(bool isFree, int PID, int pageID, std::vector<PageTableData> *pageList) : isFree(isFree), PID(PID), pageID(pageID), pageList(pageList) {}

MemoryManager::MemoryManager() {
	for (int i = 0; i < 8; i++)
		Frames.emplace_back(FrameData(true, -1, -1, nullptr));
};

MemoryManager::~MemoryManager() = default;

//------------- Funkcje do wy�wietlania bie��cych stan�w pami�ci oraz pracy krokowej  --------------
void MemoryManager::Page::print() {
	for (auto &x : data) {
		if (x == ' ') std::cout << "_";
		else std::cout << x;
	}
	std::cout << std::endl;
}

void MemoryManager::showMem() {
	std::cout << "RAM - PHYSICAL FRAMES CONTENT: \n";
	std::cout << "First bit: 0 ->\t0123456789012345 -> 15 :last bit in frame\n";
	for (int i = 0; i < 128; i++) {
		if (i % 16 == 0 && i != 0)
			std::cout << "\nFrame no." << i / 16 << ": \t";
		else if (i % 16 == 0)
			std::cout << "Frame no." << i / 16 << ": \t";
		RAM[i] != ' ' ? std::cout << RAM[i] : std::cout << '_';
	}
	std::cout << std::endl;
}

void MemoryManager::showMem(int begin, int bytes) {
	if (begin + bytes > 128) {
		std::cout << "Error: Number of bytes to display has excced amount of memory! \n";
	}
	else {
		std::cout << "Displaying physical memory from cell " << begin << " to " << begin + bytes << ":" << std::endl;
		for (int i = begin; i < begin + bytes; i++) {
			if (i != 0 && i % 16 == 0) std::cout << "\n";
			RAM[i] != ' ' ? std::cout << RAM[i] : std::cout << '-';
		}
		std::cout << std::endl;
	}
}

void MemoryManager::showPageFile() {
	std::cout << "-------Page File-------" << std::endl;
	for (auto process : PageFile) {
		std::cout << "\n**** PID:" << process.first << " ****\n";
		//std::cout << "Pages Content:";
		for (auto page : process.second) {
			page.print();
		}
	}
}

void MemoryManager::showPageTable(std::vector<PageTableData> *pageList) {
	std::cout << "PAGE\t | \tFRAME \t | \tBIT \n";
	int i = 0;
	for (auto pageListRecord : *pageList) {
		std::cout << i++ << "\t\t" << pageListRecord.frame << "\t\t" << pageListRecord.bit << "\n";
	}
}

void MemoryManager::showStack() {
	std::cout << "FIFO Stack: ";
	for (auto frame : Stack) {
		std::cout << frame << " ";
	}
	std::cout << std::endl;
}

void MemoryManager::showFrames() {
	std::cout << "FRAMES INFO: \n";
	std::cout << "\t\tFREE \tPAGE \tPID " << std::endl;
	int i = 0;
	for (auto &frame : Frames) {
		std::cout << "Frame no." << i++ << ":\t" << frame.isFree << "\t" << frame.pageID << "\t" << frame.PID << "\n";
	}
}

//------------- Funkcje u�ytkowe MemoryManagera  --------------
void MemoryManager::memoryInit() {
	for (char &cell : RAM) {
		cell = ' ';
	}
	std::string data = "JMP [0];";
	std::vector<Page> pageVector{ Page(data) };
	PageFile.insert(std::make_pair(1, pageVector));
}

void MemoryManager::stackUpdate(int frameID) {
	if (frameID > 7) return;

	for (auto it = Stack.begin(); it != Stack.end(); it++) {
		if (*it == frameID) {
			Stack.erase(it);
			break;
		}
	}

	//Stack.push_front(frameID);
	Stack.push_back(frameID);
}

std::vector<PageTableData> *MemoryManager::createPageList(int mem, int PID) {
	double pages = ceil((double)mem / 16);
	auto *pageList = new std::vector<PageTableData>();

	for (int i = 0; i < pages; i++) {
		pageList->push_back(PageTableData(false, 0));
	}

	//Za�adowanie pierszej stronicy naszego programu do Pami�ci RAM
	PageFile[PID].emplace_back();
	loadToMemory(PageFile.at(PID).at(0), 0, PID, pageList);
	//PageFile[PID].pop_back();

	return pageList;
}

int MemoryManager::seekFreeFrame() {
	int seekedFrame = -1;

	for (size_t i = 0; i < Frames.size(); i++) {
		if (Frames[i].isFree) {
			seekedFrame = i;
			break;
		}
	}
	return seekedFrame;
}

void MemoryManager::kill(int PID) {
	for (size_t i = 0; i < Frames.size(); i++) {
		if (Frames[i].PID == PID) {
			for (size_t j = i * 16; j < i * 16 + 16; j++)
				RAM[j] = ' ';
			stackUpdate(i);
			Frames[i].isFree = true;
			Frames[i].pageID = -1;
			Frames[i].PID = -1;
			PageFile.erase(PID);
		}
	}
}

int MemoryManager::loadProgram(std::string path, int mem, int PID) {
	double pagesAmount = ceil((double)mem / 16);
	std::fstream file(path); //Plik na dysku
	std::string scrap; //Zmienna pomocnicza
	std::string program; //Program w jednej linii
	std::vector<Page> pageVector; //Wektor stronic do dodania

	if (!file.is_open()) {
		std::cout << "Error: Can't open file! \n";
		return -1;
	}

	while (std::getline(file, scrap)) {
		//Dodanie spacji zamiast ko�ca linii
		if (!scrap.empty()) {
			scrap += ";";
			program += scrap;
		}
	}
	scrap.clear();

	//Dzielenie programu na stronice
	for (char i : program) {
		scrap += i;
		//Tworzenie Stronicy
		if (scrap.size() == 16) {
			pageVector.emplace_back(Page(scrap));
			scrap.clear();
		}
	}

	if (!scrap.empty())
		pageVector.emplace_back(Page(scrap));
	scrap.clear();

	if (pagesAmount * 16 < 16 * pageVector.size()) {
		std::cout << "Error: proces nie ma przypisane wystarczajaco duzo pamieci!\n";
		return -1;
	}


	//Sprawdzanie, czy program nie potrzebuje wiecej stronic w pami�ci
	for (int i = pageVector.size(); i < pagesAmount; i++)
		pageVector.emplace_back(scrap);

	//Dodanie stronic do pliku wymiany
	PageFile.insert(std::make_pair(PID, pageVector));

	return 1;
}

int MemoryManager::loadToMemory(MemoryManager::Page page, int pageID, int PID, std::vector<PageTableData> *pageList) {

	int frame = seekFreeFrame();

	if (frame == -1)
		frame = insertPage(pageList, pageID, PID);

	//Przepisywanie stronicy do pami�ci RAM
	int it = 0;
	int end = frame * 16 + 16;
	for (int i = frame * 16; i < end; i++)
		RAM[i] = page.data[it++];

	//Zmienianie bit'u w indeksie wymiany stronic
	pageList->at(pageID).bit = true;
	pageList->at(pageID).frame = frame;

	//Aktualizacja stosu u�ywalno�ci
	stackUpdate(frame);

	//Aktualizacja informacji o ramce
	Frames[frame].isFree = false;
	Frames[frame].pageID = pageID;
	Frames[frame].PID = PID;
	Frames[frame].pageList = pageList;

	return frame;
}

std::string MemoryManager::get(PCB *process, int LADDR) {
	std::string response;
	bool reading = true;
	int Frame = -1;
	unsigned int PageID = LADDR / 16;

	//przekroczenie zakres dla tego procesu
	if (process->pageList->size() <= PageID) {
		std::cout << "Error: Exceeded memory range!";
		reading = false;
		response = "ERROR";
	}

	while (reading) {
		PageID = LADDR / 16; //Numer stronicy w pami�ci

		if (process->pageList->size() <= PageID) {
			reading = false;
			break;
		}

		//Sprawdza, czy stronica znajduje si� w pami�ci operacyjnej
		if (!process->pageList->at(PageID).bit)
			loadToMemory(PageFile[process->PID][PageID], PageID, process->PID, process->pageList);

		if (process->pageList->at(PageID).bit) {
			Frame = process->pageList->at(PageID).frame;//Bie��co u�ywana ramka

			//stackUpdate(Frame);//Ramka zosta�a u�yta, wi�c trzeba zaktualizowa� stos

			if (RAM[Frame * 16 + LADDR - (16 * PageID)] == ';') //Odczytywanie do napotkania spacji
				reading = false;
			else
				response += RAM[Frame * 16 + LADDR - (16 * PageID)];
		}
		LADDR++;
	}

	return response;
}

int MemoryManager::write(PCB *process, int adress, std::string data) {

	if (data.empty()) return 1;

	if (adress + data.size() - 1 > process->pageList->size() * 16 - 1 || adress < 0) {
		std::cout << "Error: Exceeded memory amount for this process! \n";
		return -1;
	}

	int pageID;
	for (size_t i = 0; i < data.size(); i++) {
		pageID = (adress + i) / 16;
		if (process->pageList->at(pageID).bit == 0)
			loadToMemory(PageFile[process->PID][pageID], pageID, process->PID, process->pageList);
		RAM[process->pageList->at(pageID).frame * 16 + adress + i - (16 * pageID)] = data[i];

		//stackUpdate(process->pageList->at(pageID).frame);
	}
	return 1;
}

//TODO: ZROBIC FIFIO
int MemoryManager::insertPage(std::vector<PageTableData> *pageList, int pageID, int PID) {
	//*it numer ramki ktora jest ofiara
	auto it = Stack.begin();
	// auto it = Stack.end(); it--;
	int Frame = *it;
	// Przepisuje zawartosc z ramki ofiary do Pliku wymiany
	for (int i = Frame * 16; i < Frame * 16 + 16; i++) {
		PageFile[Frames[Frame].PID][Frames[Frame].pageID].data[i - (Frame * 16)] = RAM[i];
	}

	//Zmieniam wartosci w tablicy stronic ofiary
	Frames[Frame].pageList->at(Frames[Frame].pageID).bit = false;
	Frames[Frame].pageList->at(Frames[Frame].pageID).frame = -1;

	return Frame;
}