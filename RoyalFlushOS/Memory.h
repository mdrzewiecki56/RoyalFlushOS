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
//Indeks stronic dla ka�dego procesu, vector tej struktury znajduje si� w PCB
struct PageTableData {
	bool bit;  //Warto�� bool'owska sprawdzaj�ca zaj�to�� tablicy w pami�ci [Sprawdza, czy ramka znajduje si� w pami�ci RAM]
	int frame; //Numer ramki w kt�rej znajduje si� stronica

	PageTableData();
	PageTableData(bool bit, int frame);
};

class MemoryManager {

	//------------- Struktury u�ywane przez MemoryManager'a oraz zmienne--------------
public:
	char RAM[128]; //Pami�� Fizyczna Komputera [256 bajt�w]
private:
	//------------- Struktura Pojedynczej Stronicy w Pami�ci -------------
	struct Page {
		char data[16]{}; //Dane stronicy

		Page();
		Page(std::string data);

		void print();
	};

	//------------- Lista Ramek -------------
	//Struktura wykorzystywana do lepszego przeszukiwania pami�ci ram i �atwiejszej wymiany stronic
	struct FrameData {
		bool isFree; //Czy ramka jest wolna (True == wolna, False == zaj�ta)
		int PID; //Numer Procesu
		int pageID; //Numer stronicy
		std::vector<PageTableData> *pageList; //Wska�nik do tablicy stronic procesu, kt�ra znajduje si� w PCB

		FrameData(bool isFree, int PID, int pageID, std::vector<PageTableData> *pageList);
	};

	//------------- Ramki za�adowane w Pami�ci Fizycznej [w pami�ci RAM]-------------
	std::vector<FrameData> Frames;

	//------------- Plik stronicowania -------------
	// map < PID procesu, Stronice danego procesu>
	std::map<int, std::vector<Page>> PageFile;
	//std::vector<std::pair<int, std::vector<Page>>> PageFile;

	//------------- Stos ostatnio u�ywanych ramek (Least Recently Used Stack) -------------
	//Stos dzi�ki, kt�rem wiemy, kt�ra ramka jest najd�u�ej w pami�ci i kt�r� ramk� mo�emy zast�pi�
	//Jako, �e mamy 256B pami�ci ram, a jedna ramka posiada 16B, to b�dziemy mie� ��cznie 16 ramek [0-15]
	//Wi�cej: https://pl.wikipedia.org/wiki/Least_Recently_Used
	std::list<int> Stack{ 0, 1, 2, 3, 4, 5, 6, 7 };


	//------------- Funkcje do wy�wietlania bie��cych stan�w pami�ci oraz pracy krokowej  --------------
public:
	//Pokazuje zawarto�� pami�ci operacyjnej [RAM][fizycznej]
	void showMem();

	//Pokazuje odpowiednie fragmenty pami�ci [RAM]
	/* begin - miejsce w pami�ci od kt�rego ma by� wy�wietlona zawarto��
	 * bytes - ilo�� bit�w do wy�wietlenia
	 */
	void showMem(int begin, int bytes);

	//Pokazuje zawarto�� pliku stronicowania
	void showPageFile();

	//Pokazuje zawarto�� tablicy wymiany processu
	/* pageList - wska�nik na tablic� stronic procesu
	 */
	void showPageTable(std::vector<PageTableData> *pageList);

	//Pokazuje Stos ostatnio u�ywanych ramek
	void showStack();

	//Pokazuje list� ramek w pami�ci wraz z informacj� do kogo dana ramka nale�y
	void showFrames();

	//------------- Funkcje u�ytkowe MemoryManagera  --------------

			//Tworzy proces bezczynno�ci systemu umieszczany w pami�ci RAM przy starcie systemu
	void memoryInit();

	//Metoda �aduj�ca program do pliku wymiany
	/* path - �cie�ka do programu na dysku twardym
	 * mem - potrzebna ilo�� pami�ci
	 * PID - ID procesu
	 */
	int loadProgram(std::string path, int mem, int PID);

	//Usuwa z pami�ci dane wybranego procesu
	void kill(int PID);

	//Tworzy wska�nik do tablicy stronic danego procesu - funkcja wywo�ywana przy tworzeniu procesu
	/*  mem - potrzebna ilo�� pami�ci
	 *  PID - ID procesu
	 */
	std::vector<PageTableData> *createPageList(int mem, int PID);

	//Pobiera rozkaz z danego adresu
	/* PCB *process - wska�nik do PCB danego procesu
	 * int LADDR - adres logiczny z kt�rego chcemy pobra� warto��
	 */
	std::string get(PCB *process, int LADDR);

	//Zapisuje dany fragment do pami�ci
	/* *process - wska�nik do PCB danego procesu
	 * adress - adres logiczny w pami�ci na kt�rym chemy co� zapisa�
	 * data - dane do zapisania w pami�ci - np. dane z rejestru
	 */
	int write(PCB *process, int adress, std::string data);





private:
	//Zwraca adres pierwszej wolnej ramki w pami�ci
	int seekFreeFrame();

	//Przesuwa ramk� podan� jako argument na pocz�tek stack'u ostatnio u�ywanych (Least Recently Used - frames)
	/* frameID - numer ramki, kt�r� chcemy przesun�� na pocz�tek stack'u
	 */
	void stackUpdate(int frameID);

	//�aduje dan� stronic� do pami�ci RAM
	/*  page - stronica do za�adowania
	 *  pageID - numer stronicy
	 *  PID - numer procesu
	 *  *pageList - wska�nik na tablic� stronic procesu
	 */
	int loadToMemory(Page page, int pageID, int PID, std::vector<PageTableData> *pageList);

	//Zamienia stronice zgodnie z algorytmem  podanym dla pami�ci virtualnej
	/*  *pageList - wska�nik na indeks stronic procesu
	 *  pageID - numer stronicy do zamiany
	 *  PID - ID procesu
	 * @return int zwraca numer podmienionej ramki, do kt�rej zosta�a wstawiona stronica
	*/
	int insertPage(std::vector<PageTableData> *pageList, int pageID, int PID);

public:
	//------------- Konstruktor  -------------
	MemoryManager();
	//------------- Destruktor  --------------
	~MemoryManager();
};