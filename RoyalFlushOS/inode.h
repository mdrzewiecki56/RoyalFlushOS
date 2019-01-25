#pragma once
#include <iostream>
#include <array>

class inode
{
private:
	int direct_index1 = -1, direct_index2 = -1;
	std::array<int, 30> indirect_block;
	unsigned int size_of_file = 0;
	unsigned int number_of_occupied_blocks = 0;
	bool is_opened = false;
	int number_of_indexes = 0;
public:
	inode();
	~inode();
	//settery
	void set_dir_index_1(int value);
	void set_dir_index_2(int value);
	void set_indir_index_block(int value);
	void set_flag_is_opened(bool flag);
	void set_size_of_file(int value);
	void set_number_of_occupied_blocks(int value);
	void increase_number_of_indexes();
	//gettery
	int get_direct_index1();
	int get_direct_index2();
	std::array<int, 30> get_indirect_block();
	int get_number_of_indexes();
};

