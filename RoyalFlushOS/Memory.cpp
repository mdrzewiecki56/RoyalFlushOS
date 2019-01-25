#include "Memory.h"

//funkcja inicjujaca
void MemoryManager::start() {
	for (int i = 0; i < 128; i++) {
		RAM[i] = ' '; //wypelnianie RAMu spacjami
	}
	std::string data = "JP 0";
	std::vector<Page> Pvec{ Page(data) };
}

//wyswietlanie pamieci od podanego adresu i podanym zasiegu
void MemoryManager::showPMemory(int start, int range) {
	if (start + range > 128)
		std::cout << "Nie mozna przeczytac pamieci podano zly zakres" << std::endl;
	else {
		std::cout << "Pamiec fizyczna od adresu " << start << " do adresu " << start + range << std::endl;
		for (int i = start; i < range + start; i++) {
			if (i % 16 == 0 && i != 0)
				std::cout << std::endl;
			if (RAM[i] != ' ')
				std::cout << RAM[i];
			else
				std::cout << '_';

		}
		std::cout << std::endl;
	}
}

//wyswietlanie calej pamieci
void MemoryManager::showPMemory() {
	for (int i = 0; i < 128; i++) {
		if (i % 16 == 0)
			std::cout << std::endl << "RAMKA " << i / 16 << " ";
		if (RAM[i] != ' ')
			std::cout << RAM[i];
		else
			std::cout << '_';

	}
	std::cout << std::endl;
}

//wyswietlanie tablicy stron
void MemoryManager::showPageTable(std::vector<PageTableData> *page_table) {
	std::cout << "RAMKA\t | \tBIT" << std::endl;
	for (int i = 0; i < page_table->size(); i++)
		std::cout << page_table->at(i).frame << "\t | \t" << page_table->at(i).bit << std::endl;
}

int MemoryManager::LoadtoMemory(Page page, int pageN, int PID, std::vector<PageTableData> *page_table) {
	int n = 0;
	int Frame = -1;
	Frame = searchForFreeFrame();
	if (Frame == -1) //nie ma wolnych ramek
		Frame = SwapPages(page_table, pageN, PID);
	int end = Frame * 16 + 16;
	for (int i = Frame * 16; i < end; i++) {
		RAM[i] = page.data[n];
		n++;
	}

	//zmiana wartosci w tablicy stron
	page_table->at(pageN).bit = 1;
	page_table->at(pageN).frame = Frame;

	//ustawiamy porzadek ramek i oznaczamy uzyta ramke jako zajeta
	setFrameOrder(Frame);
	Frames[Frame].free = 0;
	Frames[Frame].pageN = pageN;
	Frames[Frame].page_table = page_table;
	Frames[Frame].PID = PID;

	return Frame;
}

int MemoryManager::LoadProgram(std::string path, int PID, PCB *pcb) {
	std::fstream file; //plik zawierajacy program
	std::string str; //zmienna pomocnicza
	std::string program; //caly program w jednym stringu
	std::vector<Page> pagevec; //vector zawierajacy strony do dodania
	file.open(path, std::ios::in);

	if (!file.is_open()) {
		std::cout << "Nie moge otworzyc pliku" << std::endl;
		return -1;
	}

	while (std::getline(file, str)) {
		if (!file.eof())
			str += " ";
		program += str;
	}
	str.clear();

	//dzielenie programu na strony
	for (int i = 0; i < program.size(); i++) {
		str += program[i];
		//tworzenie stron
		if (str.size() == 16) {
			pagevec.push_back(Page(str));
			str.clear();
		}
	}

	//ostatnia strona
	if (str.size() != 0)
		pagevec.push_back(Page(str));
	str.clear();

	//jezeli wektor stron jest wiekszy od 8, wywala error
	if (pagevec.size() > 8) {
		std::cout << "Pamiec programu wychodzi poza zakres" << std::endl;
		return -1;
	}

	//dodanie stron do pliku wymiany
	insertToSwapFile(pagevec, PID);
	pcb->page_table = createPageTable(PID);
	
}

std::vector<PageTableData> *MemoryManager::createPageTable(int PID) {
	int pages = 8;
	int Frame = -1;
	std::vector<PageTableData> *page_table = new std::vector<PageTableData>;

	//tworzenie tablicy
	for (int i = 0; i < pages; i++)
		page_table->push_back(PageTableData(-1, 0));

	//zaladowanie pierwszej stronicy do RAMu
	LoadtoMemory(SwapFile.at(PID).at(0), 0, PID, page_table);

	return page_table;
}

std::string MemoryManager::Get(PCB *process, int LR) {
	//string do wyslania
	std::string order;
	bool koniec = false;
	int Frame = -1;
	int stronica = LR / 16;

	//przekroczenie zakresu dla tego procesu
	if (process->page_table->size() <= stronica) {
		std::cout << "Przekroczenie zakresu";
		koniec = true;
		order = "ERROR";
	}
	while (!koniec) {
		stronica = LR / 16; //stronica ktora musi znajdowac sie w pamieci
							// koniec programu
		if (process->page_table->size() <= stronica) {
			koniec = true;
			break;
		}

		//brak stronicy w pamieci operacyjnej
		if (process->page_table->at(stronica).bit == 0)
			LoadtoMemory(SwapFile[process->PID][stronica], stronica, process->PID, process->page_table);

		//stronica w pamieci operacyjnej
		if (process->page_table->at(stronica).bit == 1) {
			Frame = process->page_table->at(stronica).frame;		//ramka w ktorej pracuje
			setFrameOrder(Frame);									//uzywam ramki wiec zmieniam jej pozycje na liscie porzadku ramek
			if (RAM[Frame * 16 + LR - (16 * stronica)] == ' ')	//czytam do napotkania spacji
				koniec = true;
			else
				order += RAM[Frame * 16 + LR - (16 * stronica)];
		}
		LR++;
	}
	return order;
}

void MemoryManager::Remove(int PID) {
	for (int i = 0; i < Frames.size(); i++) {
		if (Frames.at(i).PID == PID) { // Zerowanie pamieci RAM
			for (int j = i * 16; j < i * 16 + 16; j++) {
				RAM[j] = ' ';
			}

			Frames.at(i).free = 1; // Komorka znowu wolna
			Frames.at(i).pageN = -1;
			Frames.at(i).PID = -1;
			SwapFile.erase(PID);
			//delete Frames.at(i).page_table; // usuwanie tablicy stronic dla procesu
		}
	}
}

int MemoryManager::Write(PCB *process, int adress, std::string data) {
	int stronica = adress / 16;
	std::string str;

	if (data.size() == 0) {
		return 1;
	}

	if (adress + data.size() - 1 > process->page_table->size() * 16 - 1 || adress < 0) {
		std::cout << "Przekroczono zakres dla tego procesu" << std::endl;
		return -1;
	}

	for (int i = 0; i < data.size(); i++) {
		stronica = (adress + i) / 16;
		if (process->page_table->at(stronica).bit == 0)
			LoadtoMemory(SwapFile[process->PID][stronica], stronica, process->PID, process->page_table);

		RAM[process->page_table->at(stronica).frame * 16 + adress + i - (16 * stronica)] = data[i];
		setFrameOrder(process->page_table->at(stronica).frame);
	}
	return 1;
}

int MemoryManager::SwapPages(std::vector<PageTableData>* page_table, int pageNumber, int PID)
{
	std::list<int>::iterator victim = FrameOrder.begin();

	int frame = *victim;

	for (int i = frame * 16; i < frame * 16 + 16; i++)
	{
		SwapFile[Frames[frame].PID][Frames[frame].pageN].data[i - (frame * 16)] = RAM[i];
	}

	Frames[frame].page_table->at(Frames[frame].pageN).bit = 0;
	Frames[frame].page_table->at(Frames[frame].pageN).frame = -1;

	return frame;
}

int MemoryManager::searchForFreeFrame()
{
	int free_frame = -1; //nie ma wolnej
	for (int i = 0; i < Frames.size(); i++)
	{
		if (Frames[i].free == 1) {
			free_frame = i;
			break;
		}
	}
	return free_frame;
}

//JEST GIT
void MemoryManager::setFrameOrder(int frame)
{
	bool in_memory = false;
	for (std::list<int>::iterator it = FrameOrder.begin(); it != FrameOrder.end(); it++)
	{
		if (*it == frame)
		{
			in_memory = true;
			break;
		}
	}

	if (FrameOrder.size() < 8)
	{
		if (in_memory == false)
		{
			FrameOrder.push_back(frame);
		}
	}
	else if (FrameOrder.size() >= 8)
	{
		if (in_memory == false)
		{
			FrameOrder.erase(FrameOrder.begin());
			FrameOrder.push_back(frame);
		}
	}
}

//JEST GIT
void MemoryManager::printFIFO()
{
	std::cout << "Frame Order (FIFO): ";
	for (std::list<int>::iterator it = FrameOrder.begin(); it != FrameOrder.end(); it++)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}

//JEST GIT
void MemoryManager::printSwapFile()
{
	int i = 0;
	std::cout << "Swap File: " << std::endl;

	for (auto x : SwapFile)
	{
		std::cout << "ID PROCESU: " << x.first << std::endl;
		for (int i = 0; i < x.second.size(); i++) {
			x.second.at(i).print();
		}
	}
}

//JEST GIT
void MemoryManager::insertToSwapFile(std::vector<Page> pagevec, int PID)
{
	SwapFile.insert(std::make_pair(PID, pagevec));
}