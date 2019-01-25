#pragma once
#include<iostream>
class Help
{
public:
	/// DYSK
	void CREATEFILE_H();
	void READFILE_H();
	void WRITEFILE_H();
	void DELETEFILE_H();
	void RENAMEFILE_H();
	void COPYFILE_H();

	///MEMORY
	void DISPLAYDISK_H();
	void FORMATDISK_H();
	void SHOW_DIR_H();
	void SHOWMEMORY_H();

	/// PROCESS
	void CREATEPROCESS_H();
	void DELETEPROCESS_H();
	void RUNPROCESS_H();
	void SHOWPCB_H();
	void SHOWLIST_H();
	void QUEUE_H();

	/// MY
	void HELP_F();

	Help();
	~Help();
};

