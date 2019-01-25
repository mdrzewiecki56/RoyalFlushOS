#include "inode.h"


inode::inode()
{
	for (int i = 0; i < indirect_block.size(); i++) {
		indirect_block[i] = -1;
	}
}


inode::~inode()
{
}

void inode::set_dir_index_1(int value)
{
	this->direct_index1 = value;
}

void inode::set_dir_index_2(int value)
{
	this->direct_index2 = value;
}

void inode::set_indir_index_block(int value)
{
	for (int i = 0; i < indirect_block.size(); i++) {
		if (indirect_block[i] == -1) {
			this->indirect_block[i] = value;
			break;
		}
	}
}

void inode::set_flag_is_opened(bool flag)
{
	is_opened = flag;
}

void inode::set_size_of_file(int value)
{
	size_of_file = value;
}

void inode::set_number_of_occupied_blocks(int value)
{
	number_of_occupied_blocks = value;
}

void inode::increase_number_of_indexes()
{
	number_of_indexes++;
}

int inode::get_direct_index1()
{
	return direct_index1;
}

int inode::get_direct_index2()
{
	return direct_index2;
}

std::array<int, 30> inode::get_indirect_block()
{
	return indirect_block;
}

int inode::get_number_of_indexes()
{
	return number_of_indexes;
}
