// Created by Wojciech Kasperski on 15-Oct-18.
#include <list>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cmath>
#include <fstream>

class PCB;

//------------- Tablica stronic procesu-------------
//Indeks stronic dla ka¿dego procesu, vector tej struktury znajduje siê w PCB
struct PageTableData {
	bool bit;  //Wartoœæ bool'owska sprawdzaj¹ca zajêtoœæ tablicy w pamiêci [Sprawdza, czy ramka znajduje siê w pamiêci RAM]
	int frame; //Numer ramki w której znajduje siê stronica

	PageTableData();
	PageTableData(bool bit, int frame);
};

class MemoryManager {

	//------------- Struktury u¿ywane przez MemoryManager'a oraz zmienne--------------
public:
	char RAM[128]; //Pamiêæ Fizyczna Komputera [256 bajtów]
private:
	//------------- Struktura Pojedynczej Stronicy w Pamiêci -------------
	struct Page {
		char data[16]{}; //Dane stronicy

		Page();
		Page(std::string data);

		void print();
	};

	//------------- Lista Ramek -------------
	//Struktura wykorzystywana do lepszego przeszukiwania pamiêci ram i ³atwiejszej wymiany stronic
	struct FrameData {
		bool isFree; //Czy ramka jest wolna (True == wolna, False == zajêta)
		int PID; //Numer Procesu
		int pageID; //Numer stronicy
		std::vector<PageTableData> *pageList; //WskaŸnik do tablicy stronic procesu, która znajduje siê w PCB

		FrameData(bool isFree, int PID, int pageID, std::vector<PageTableData> *pageList);
	};

	//------------- Ramki za³adowane w Pamiêci Fizycznej [w pamiêci RAM]-------------
	std::vector<FrameData> Frames;

	//------------- Plik stronicowania -------------
	// map < PID procesu, Stronice danego procesu>
	std::map<int, std::vector<Page>> PageFile;
	//std::vector<std::pair<int, std::vector<Page>>> PageFile;

	//------------- Stos ostatnio u¿ywanych ramek (Least Recently Used Stack) -------------
	//Stos dziêki, którem wiemy, która ramka jest najd³u¿ej w pamiêci i któr¹ ramkê mo¿emy zast¹piæ
	//Jako, ¿e mamy 256B pamiêci ram, a jedna ramka posiada 16B, to bêdziemy mieæ ³¹cznie 16 ramek [0-15]
	//Wiêcej: https://pl.wikipedia.org/wiki/Least_Recently_Used
	std::list<int> Stack{ 0, 1, 2, 3, 4, 5, 6, 7 };


	//------------- Funkcje do wyœwietlania bie¿¹cych stanów pamiêci oraz pracy krokowej  --------------
public:
	//Pokazuje zawartoœæ pamiêci operacyjnej [RAM][fizycznej]
	void showMem();

	//Pokazuje odpowiednie fragmenty pamiêci [RAM]
	/* begin - miejsce w pamiêci od którego ma byæ wyœwietlona zawartoœæ
	 * bytes - iloœæ bitów do wyœwietlenia
	 */
	void showMem(int begin, int bytes);

	//Pokazuje zawartoœæ pliku stronicowania
	void showPageFile();

	//Pokazuje zawartoœæ tablicy wymiany processu
	/* pageList - wskaŸnik na tablicê stronic procesu
	 */
	void showPageTable(std::vector<PageTableData> *pageList);

	//Pokazuje Stos ostatnio u¿ywanych ramek
	void showStack();

	//Pokazuje listê ramek w pamiêci wraz z informacj¹ do kogo dana ramka nale¿y
	void showFrames();

	//------------- Funkcje u¿ytkowe MemoryManagera  --------------

			//Tworzy proces bezczynnoœci systemu umieszczany w pamiêci RAM przy starcie systemu
	void memoryInit();

	//Metoda ³aduj¹ca program do pliku wymiany
	/* path - œcie¿ka do programu na dysku twardym
	 * mem - potrzebna iloœæ pamiêci
	 * PID - ID procesu
	 */
	int loadProgram(std::string path, int mem, int PID);

	//Usuwa z pamiêci dane wybranego procesu
	void kill(int PID);

	//Tworzy wskaŸnik do tablicy stronic danego procesu - funkcja wywo³ywana przy tworzeniu procesu
	/*  mem - potrzebna iloœæ pamiêci
	 *  PID - ID procesu
	 */
	std::vector<PageTableData> *createPageList(int mem, int PID);

	//Pobiera rozkaz z danego adresu
	/* PCB *process - wskaŸnik do PCB danego procesu
	 * int LADDR - adres logiczny z którego chcemy pobraæ wartoœæ
	 */
	std::string get(PCB *process, int LADDR);

	//Zapisuje dany fragment do pamiêci
	/* *process - wskaŸnik do PCB danego procesu
	 * adress - adres logiczny w pamiêci na którym chemy coœ zapisaæ
	 * data - dane do zapisania w pamiêci - np. dane z rejestru
	 */
	int write(PCB *process, int adress, std::string data);





private:
	//Zwraca adres pierwszej wolnej ramki w pamiêci
	int seekFreeFrame();

	//Przesuwa ramkê podan¹ jako argument na pocz¹tek stack'u ostatnio u¿ywanych (Least Recently Used - frames)
	/* frameID - numer ramki, któr¹ chcemy przesun¹æ na pocz¹tek stack'u
	 */
	void stackUpdate(int frameID);

	//£aduje dan¹ stronicê do pamiêci RAM
	/*  page - stronica do za³adowania
	 *  pageID - numer stronicy
	 *  PID - numer procesu
	 *  *pageList - wskaŸnik na tablicê stronic procesu
	 */
	int loadToMemory(Page page, int pageID, int PID, std::vector<PageTableData> *pageList);

	//Zamienia stronice zgodnie z algorytmem  podanym dla pamiêci virtualnej
	/*  *pageList - wskaŸnik na indeks stronic procesu
	 *  pageID - numer stronicy do zamiany
	 *  PID - ID procesu
	 * @return int zwraca numer podmienionej ramki, do której zosta³a wstawiona stronica
	*/
	int insertPage(std::vector<PageTableData> *pageList, int pageID, int PID);

public:
	//------------- Konstruktor  -------------
	MemoryManager();
	//------------- Destruktor  --------------
	~MemoryManager();
};