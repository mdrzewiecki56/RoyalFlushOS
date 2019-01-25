#pragma once
#include <iostream>
#include <bitset>
#include <array>
#include <unordered_map>
#include <cmath>
#include "inode.h"
#include "Semaphore.h"

class dysk
{
private:
	std::array<char, 1024> data_container;
	std::bitset<32> bit_vector = 0;
	std::unordered_map<std::string, inode> directory;
	unsigned int const block_size = 32;
	unsigned int const max_number_of_blocks = 32;
	int inode_counter = 0;
	Semaphore file_synchronization;

public:
	dysk();
	~dysk();
	//funkcje podstawowe
	bool Create_File(std::string file_name, std::string data); //tworzenie pliku pustego lub od razu z zapisem danych na dysk (data moze byc pustym stringiem)
	bool Delete_File(std::string file_name); //usuwanie pliku o podanej nazwie z katalogu i pamieci dysku
	bool Write_To_File(std::string file_name, std::string data); //dopisywanie danych do plik juz istniejacego
	std::string Read_From_File(std::string file_name); //sczytanie danych z pliku
	//funkcje dodatkowe
	bool Change_File_Name(std::string old_file_name, std::string new_file_name); //zmiana nazwy pliku w katalogu
	bool Copy_File(std::string file_one, std::string new_file); //kopiowanie zawartosci pliku do nowego 
	void Format_Disc(); //formatowanie całego dysku do zera i wyczyszczenie katalogu
	//pokazywanie zawartosci dysku
	void show_data_container(); //pokazywanie zawartosci calego dysku z podzialem na bloki
	void show_directory(); //wypisanie calej zawartosci katalogu
	void show_bit_vector(); //pokazanie calego bitsetu (glownie dla mnie, sprawdzam czy dobrze wszystko sie dodaje/dopisuje/usuwa)
};

