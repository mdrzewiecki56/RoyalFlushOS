#include "Help.h"


/// DYSK
void Help::CREATEFILE_H()
{
	std::cout << "Usage: cf [param1] [param2]" << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name of the file." << std::endl;
	std::cout << "\tparam2 - optionally text written into the file." << std::endl << std::endl;
}

void Help::READFILE_H()
{
	std::cout << "Usage: rf [param1]" << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name of the file you want to read." << std::endl << std::endl;
}

void Help::WRITEFILE_H()
{
	std::cout << "Usage: wf [param1][param2]" << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name of the file you want write text in." << std::endl;
	std::cout << "\tparam2 - the text that will be written into  the file." << std::endl << std::endl;
}

void Help::DELETEFILE_H()
{
	std::cout << "Usage: df [param1]" << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name of the file you want to delete." << std::endl << std::endl;
}

void Help::RENAMEFILE_H()
{
	std::cout << "Usage: rn [param1] [param2]" << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name of the file you want to change the name." << std::endl;
	std::cout << "\tparam2 - new name of the file." << std::endl << std::endl;
}

void Help::COPYFILE_H()
{
	std::cout << "Usage: co [param1] [param2]" << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name of the file you want to copy." << std::endl;
	std::cout << "\tparam2 - name of the new file." << std::endl << std::endl;
}

/// MEMORY
void Help::DISPLAYDISK_H()
{
	std::cout << "Usage: dd " << std::endl << std::endl;
	std::cout << "Function: Displays the content of the drive." << std::endl;
}

void Help::FORMATDISK_H()
{
	std::cout << "Usage: fd " << std::endl << std::endl;
	std::cout << "Function: Deletes the content of the drive." << std::endl;
}

void Help::SHOW_DIR_H()
{
	std::cout << "Usage: sd " << std::endl << std::endl;
	std::cout << "Function: Displays the content of the calatog." << std::endl;
}

void Help::SHOWMEMORY_H()
{
	std::cout << "Usage: dmemory " << std::endl << std::endl;
	std::cout << "Function: Displays the content of the physical memory." << std::endl;
}

/// PROCESS
void Help::CREATEPROCESS_H()
{
	std::cout << "Usage: cp [param1] [param2] [param3]" << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name for the new process." << std::endl;
	std::cout << "\tparam2 - name of the .txt file with the program." << std::endl;
	std::cout << "\tparam3 - ID of the father process." << std::endl << std::endl;
}

void Help::DELETEPROCESS_H()
{
	std::cout << "Usage: dprocess [param1]" << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name for the process you want to delete." << std::endl;
}

void Help::RUNPROCESS_H()
{
	std::cout << "Usage: rp [param1]" << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - name for the process you want to run." << std::endl;
}

void Help::SHOWPCB_H()
{
	std::cout << "Usage: showpcb [param1] " << std::endl << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\tparam1 - ID of the process that you want to check the pcb." << std::endl << std::endl;
}

void Help::SHOWLIST_H()
{
	std::cout << "Usage: dlist " << std::endl << std::endl;
	std::cout << "Usage: displays the list of processes." << std::endl << std::endl;
}

void Help::QUEUE_H()
{
	std::cout << "Usage: pq " << std::endl << std::endl;
	std::cout << "Usage: displays the queue of processes." << std::endl << std::endl;
}

///MY
void Help::HELP_F()
{
	std::cout << "You've typed wrong input." << std::endl;
	std::cout << "If you need help, just type the name of the command or the name of the command and \"/?\"" << std::endl;
	std::cout << "ask the developers of this Operation System." << std::endl << std::endl;
}


Help::Help()
{
}


Help::~Help()
{
}
