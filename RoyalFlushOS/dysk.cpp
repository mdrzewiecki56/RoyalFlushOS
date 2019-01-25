#include "dysk.h"

dysk::dysk()
{
	for (int i = 0; i < data_container.size(); i++) {
		data_container[i] = NULL;
	}
}


dysk::~dysk()
{
	std::cout << "Disc destroyed.\n" << std::endl;
}

bool dysk::Create_File(std::string file_name, std::string data)
{	
	file_synchronization.wait();
	const int size_of_file_name = file_name.size();
	const char* raw_data = data.c_str();
	
	//sprawdzenie, czy plik o podanej nazwie juz istnieje
	for (const auto& e : directory) {
		if (e.first == file_name) {
			std::cout << "File with the same name already exists.\n";
			file_synchronization.signal();
			return false;
		}
	}

	//sprawdzenie czy nie osiagnieto limitu i-wezlow
	if (inode_counter == 32) {
		std::cout << "Couldn't create file " << file_name << ", inode limit\n";
		file_synchronization.signal();
		return false;
	}

	//sprawdzenie czy mamy odpowiednio duzo wolnego miejsca na dysku
	int data_size = data.size();
	double blocks_to_occupy = ceil(data_size / 32.0);

	int free_blocks_counter = 0;
	for (std::size_t i = 0; i < bit_vector.size(); i++) {
		if (bit_vector[i] == 0) {
			free_blocks_counter++;
		}
	}

	if (free_blocks_counter < blocks_to_occupy) {
		std::cout << "Couldn't create file " << file_name << ", not enough free space.\n";
		file_synchronization.signal();
		return false;
	}

	//stworzenie pliku na dysku
	inode node;
	node.set_number_of_occupied_blocks(blocks_to_occupy);
	node.set_size_of_file(size_of_file_name);

	int licznik = 0;
	for (std::size_t i = 0; i < bit_vector.size(); i++) {
		if (bit_vector[i] == false) {
			//ustawianie wskaznikow na bloki
			if (node.get_direct_index1() == -1) {
				node.set_dir_index_1(i);
				node.increase_number_of_indexes();
				bit_vector[i] = true;
			}
			else if (node.get_direct_index2() == -1) {
				node.set_dir_index_2(i);
				node.increase_number_of_indexes();
				bit_vector[i] = true;
			}
			else {
				node.set_indir_index_block(i);
				node.increase_number_of_indexes();
				bit_vector[i] = true;
			}
			//jezeli tworzymy pusty plik wychodzimy z pêtli
			if (data.size() == 0) {
				break;
			}
			//wpisywanie danych na dysk
			for (int k = 0; k < 32; k++) {
				if (licznik == data.size()) {
					break;
				}
				licznik++;
				data_container[i * 32 + k] = data[licznik-1];
				
			}
			if (licznik == data.size()) {
				break;
			}
		}
	}

	directory.emplace(file_name, node);
	inode_counter++;
	std::cout << "Creating file " << file_name << " finished successfully.\n";
	file_synchronization.signal();
	return true;
}

bool dysk::Delete_File(std::string file_name)
{
	file_synchronization.wait();
	//sprawdzenie, czy plik o podanej nazwie juz istnieje
	bool exists = false;
	for (auto& e : directory) {
		if (e.first == file_name) {
			exists = true;

			//usuwanie danych z dysku
			for (int i = 0; i < 32; i++) {
				data_container[e.second.get_direct_index1() * 32 + i] = NULL;
			}
			bit_vector[e.second.get_direct_index1()] = false;
			
			if (e.second.get_direct_index2() != -1) {
				for (int i = 0; i < 32; i++) {
					data_container[e.second.get_direct_index2() * 32 + i] = NULL;
				}
				bit_vector[e.second.get_direct_index2()] = false;
			}
			for (int i = 0; i < 30; i++) {
				if (e.second.get_indirect_block()[i] != -1) {
					for (int j = 0; j < 32; j++) {
						data_container[e.second.get_indirect_block()[i] * 32 + j] = NULL;
					}
					bit_vector[e.second.get_indirect_block()[i]] = false;
				}
				else {
					break;
				}
			}
			//usuwanie wpisu z katalgu
			//e.second.~inode();
			directory.erase(file_name);
			break;
		}
	}

	if (exists == false) {
		std::cout << "Couldn't find file named " << file_name << ".\n";
		file_synchronization.signal();
		return false;
	}
	
	std::cout << "Deleting file " << file_name << " finished successfully.\n";
	file_synchronization.signal();
	return true;
}

bool dysk::Write_To_File(std::string file_name, std::string data)
{
	file_synchronization.wait();
	bool exists = false;
	bool end = false;
	inode write_node;
	int licznik = 0, pierwszy_wolny_index = 0;
	for (auto& e : directory) {
		if (e.first == file_name) {
			exists = true;
			int free_blocks_counter = 0;
			for (std::size_t i = 0; i < bit_vector.size(); i++) {
				if (bit_vector[i] == false) {
					free_blocks_counter++;
				}
			}
			if (free_blocks_counter < (data.size()/32.0)) {
				std::cout << "Not enough free space on disc.\n";
				file_synchronization.signal();
				return false;
				break;
			}
			else{
				inode node = e.second;
				int number_of_needed_blocks = ceil(data.size() / 32);
				switch (node.get_number_of_indexes()) //dostanie sie do ostatniego indeksu
				{
				case 1: //jezeli plik ma tylko jeden wskaznik bezposredni zajety
					for (int i = 0; i < 32; i++) { //znalezienie pierwszego wolnego indeksu w bloku wskazywanym przez index1
						if (data_container[node.get_direct_index1() * 32 + i] == NULL) {
							pierwszy_wolny_index = i;
							break;
						}
					}
					for (int i = 0; i < 32 - pierwszy_wolny_index; i++) { //wypelnienie do konca 1-go indeksu
						if (i <= data.size()) { //sprawdzenie czy nie wypelniamy za duzo, gdy data jest male
							licznik++;
							data_container[node.get_direct_index1() * 32 + pierwszy_wolny_index + i] = data[licznik-1];
						}
						else {
							end = true;
							goto outOfloops;
						}
					}
					if (data.size() > licznik) { //jesli musimy wpisac wiecej danych niz pomiesci blok wskazywany przez index1 szukamy wolnego bloku
						for (std::size_t i=0; i < bit_vector.size(); i++) {
							if (bit_vector[i] == false) { //znaleziony wolny blok + wpisanie go do inode i zarezerwowanie
								node.set_dir_index_2(i);
								node.increase_number_of_indexes();
								bit_vector[i] = true;
								break;
							}
						}
					}
					else {
						break;
					}
					for (int i = 0; i < 32; i++) { //wypelnienie bloku wskazywanego przez index2
						if (licznik <= data.size()) { //dopisywanie ze sprawdzaniem czy nie probujemy wypelniac wiecej niz jest w data
							data_container[node.get_direct_index2() * 32 + i] = data[licznik];
							licznik++;
						}
						else {
							end = true;
							goto outOfloops;
						}
					}
					for (int j = 0; j < 30; j++) {//petla iterujaca po bloku jednoposrednim
						if (data.size() > licznik) { //jezeli trzeba wpisac wiecej danych niz pomieszcza bloki wskazywane przez index1 i index2 tworzymy blok posredni
							for (std::size_t i=0; i < bit_vector.size(); i++) {
								if (bit_vector[i] == false) { //znaleziono wolny blok + przypisane go do inode
									node.set_indir_index_block(i);
									node.increase_number_of_indexes();
									bit_vector[i] = true;
									break;
								}
							}
						}
						else {
							break;
						}
						for (int i = 0; i < 32; i++) { //wpisywanie danych do kolejnego bloku wskazywanego przez blok jednoposredni
							if (licznik <= data.size()) { //sprawdzanie czy nie wpisujemy wiecej niz jest w data
								data_container[node.get_indirect_block()[j] * 32 + i] = data[licznik];
								licznik++;
							}
							else {
								end = true;
								goto outOfloops; //jesli wpisalismy cale data na dysk uciekamy z petli
							}
						}
					}
				outOfloops:
					write_node = node;
					break;
					
				case 2: //opcja, gdy pierwszy index jest caly zajety i zajety jest drugi
					for (int i = 0; i < 32; i++) { //znalezienie pierwszego wolnego indeksu w bloku wskazywanym przez index2
						if (data_container[node.get_direct_index2() * 32 + i] == NULL) {
							pierwszy_wolny_index = i;
							break;
						}
					}
					for (int i = 0; i < 32 - pierwszy_wolny_index; i++) { //wypelnienie do konca 2-go indeksu
						if (i <= data.size()) { //sprawdzenie czy nie wypelniamy za duzo, gdy data jest male
							licznik++;
							data_container[node.get_direct_index2() * 32 + pierwszy_wolny_index + i] = data[licznik-1];
						}
						else {
							end = true;
							goto outOfloops2;
						}
					}
					for (int j = 0; j < 30; j++) {//petla iterujaca po bloku jednoposrednim
						if (data.size() > licznik) { //jezeli trzeba wpisac wiecej danych niz pomieszcza bloki wskazywane przez index1 i index2 tworzymy blok posredni
							for (std::size_t i=0; i < bit_vector.size(); i++) {
								if (bit_vector[i] == false) { //znaleziono wolny blok + przypisane go do inode
									node.set_indir_index_block(i);
									node.increase_number_of_indexes();
									bit_vector[i] = true;
									break;
								}
							}
						}
						else {
							break;
						}
						for (int i = 0; i < 32; i++) { //wpisywanie danych do kolejnego bloku wskazywanego przez blok jednoposredni
							if (licznik <= data.size()) { //sprawdzanie czy nie wpisujemy wiecej niz jest w data
								data_container[node.get_indirect_block()[j] * 32 + i] = data[licznik];
								licznik++;
							}
							else {
								end = true;
								goto outOfloops2; //jesli wpisalismy cale data na dysk uciekamy z petli
							}
						}
					}
				outOfloops2:
					write_node = node;
					break;

				default: //jezeli inode posiada 3 lub wiecej wskaznikow (oba bezposrednie wskazuja na bloki w pelni zajete

					for (int j = node.get_number_of_indexes()-3; j < 30; j++) {//petla iterujaca po bloku jednoposrednim
						if (node.get_indirect_block()[j+1] == -1) {
							for (int i = 0; i < 32; i++) { //znalezienie pierwszego wolnego indeksu w bloku wskazywanym przez index w bloku posrednim
								if (data_container[node.get_indirect_block()[j] * 32 + i] == NULL) {
									pierwszy_wolny_index = i;
									break;
								}
							}
							for (int i = 0; i < (32 - pierwszy_wolny_index); i++) { //wypelnienie do konca bloku wskazywanego przez j-ty indeks w bloku posrednim
								if (i <= data.size()) { //sprawdzenie czy nie wypelniamy za duzo, gdy data jest male
									licznik++;
									data_container[node.get_indirect_block()[j] * 32 + pierwszy_wolny_index + i] = data[licznik - 1];
								}
								else {
									end = true;
									goto outOfloops3;
								}
							}
							if (data.size() > licznik) { //jezeli musimy zarezerwowac kolejny blok pamieci na dane
								for (std::size_t i = 0; i < bit_vector.size(); i++) { //znalezienie kolejnego wolnego bloku i przypisanie go do node'a
									if (bit_vector[i] == false) {
										node.set_indir_index_block(i);
										node.increase_number_of_indexes();
										bit_vector[i] = true;
										break;
									}
								}
							}
							for (int i = 0; i < 32; i++) { //wpisywanie danych do kolejnego bloku wskazywanego przez blok jednoposredni
								if (licznik <= data.size()) { //sprawdzanie czy nie wpisujemy wiecej niz jest w data
									data_container[node.get_indirect_block()[j+1] * 32 + i] = data[licznik];
									licznik++;
								}
								else {
									end = true;
									goto outOfloops3;
								}
							}
						}
						if (licznik == data.size()) {
							end = true;
							break;
						}						
					}
				outOfloops3:
					write_node = node;
					break;
				}
				
			}

		}
		if (end) {
			break;
		}
	}
	if (exists == false) {
		std::cout << "Couldn't write to file " << file_name <<".\n";
		file_synchronization.signal();
		return false;
	}
	else {
		//zamiana node na ten, w ktorym sa wpisane dane
		directory.erase(file_name);
		directory.emplace(file_name, write_node);
		std::cout << "Writing to file " << file_name << " finished successfully.\n";
		file_synchronization.signal();
		return true;
	}
}

std::string dysk::Read_From_File(std::string file_name)
{
	file_synchronization.wait();
	bool exists = false;
	std::string return_string = "";
	for (const auto& e : directory) {
		if (e.first == file_name) {
			exists = true; //znaleziono plik o podanej nazwie w katalogu
			inode node = e.second;
			if (node.get_direct_index1() != -1) {
				for (int i = 0; i < 32; i++) {
					if (data_container[node.get_direct_index1() * 32 + i] != NULL) { //sprawdzenie, czy nie wpisujemy NULLow do stringa
						return_string += data_container[node.get_direct_index1() * 32 + i];
					}
					else {
						break;
					}
					
				}
			}
			if (node.get_direct_index2() != -1) {
				for (int i = 0; i < 32; i++) {
					if (data_container[node.get_direct_index2() * 32 + i] != NULL) { //sprawdzenie, czy nie wpisujemy NULLow do stringa
						return_string += data_container[node.get_direct_index2() * 32 + i];
					}
					else {
						break;
					}

				}
			}
			for (int i = 0; i < 30; i++) { //iteracja po bloku posrednim
				if (node.get_indirect_block()[i] != -1) {
					for (int j = 0; j < 32; j++) { //iteracja po bloku na dysku wskazywanym przez index w bloku jednoposrednim
						if (data_container[node.get_indirect_block()[i] * 32 + j] != NULL) { //sprawdzenie, czy nie wpisujemy NULLow do stringa
							return_string += data_container[node.get_indirect_block()[i] * 32 + j];
						}
						else {
							break;
						}
					}
				}
				else {
					break;
				}
			}
		}
	}
	if (exists) {
		std::cout << "Reading data from file " << file_name << " finished successfully.\n";
		file_synchronization.signal();
		return return_string;
	}
	else {
		std::cout << "Couldn't read data from " << file_name << ".\n";
		file_synchronization.signal();
		return return_string;
	}

}

bool dysk::Change_File_Name(std::string old_file_name, std::string new_file_name)
{
	file_synchronization.wait();
	bool exists = false;
	for (auto& e : directory) {
		if (e.first == old_file_name) {
			exists = true;
			inode copy = e.second;
			directory.erase(old_file_name);
			directory.emplace(new_file_name, copy);
			break;
		}
	}

	if (exists) {
		std::cout << "Changing name "<< old_file_name << " to " << new_file_name << " finished successfully.\n";
		file_synchronization.signal();
		return true;
	}
	else {
		std::cout << "Couldn't find " << old_file_name << " file in directory.\n";
		file_synchronization.signal();
		return false;
	}
}

bool dysk::Copy_File(std::string file_one, std::string new_file)
{
	file_synchronization.wait();
	bool exists = false;
	for (auto& e : directory) {
		if (e.first == file_one) { //znalezienie pliku o podanej nazwie
			exists = true;
			std::string read = Read_From_File(file_one); //skopiowanie zawartosci pliku do nowego stringa
			Create_File(new_file, read); //utworzenie nowego pliku pod inn¹ nazw¹ z t¹ sam¹ zawartoœci¹
		}
	}

	if (exists) {
		std::cout << "Copying from " << file_one << " to " << new_file << " finished successfully.";
		file_synchronization.signal();
		return true;
	}
	else {
		std::cout << "Couldn't copy to new file.\n";
		file_synchronization.signal();
		return false;
	}
}

void dysk::Format_Disc()
{
	file_synchronization.wait();
	for (int i = 0; i < 1024 ; i++) {
		data_container[i] = NULL; //wyzerowanie dysku
	}

	directory.clear(); //wyczyszczenie katalogu
	
	bit_vector = 0; //wyzerowanie bit_vector

	std::cout << "Disc formated and directory cleared.\n";
	file_synchronization.signal();
	
}

void dysk::show_data_container()
{
	std::cout << "\nDisc:\n";
	for (int i = 0; i < 32; i++) {
		if (i <= 9) {
			std::cout << "Blok " << i << " :  [";
		}
		else {
			std::cout << "Blok " << i << " : [";
		}
		for (int j = 0; j < 32; j++) {
			std::cout << data_container[i * 32 + j] << ",";
		}
		std::cout << "]\n";
	}
}

void dysk::show_directory()
{
	std::cout << "\nDirectory:\n";
	for (auto& e : directory) {
		std::cout << e.first << " ";
	}
	std::cout << std::endl;
}

void dysk::show_bit_vector()
{
	std::cout << std::endl << bit_vector << std::endl;
}
