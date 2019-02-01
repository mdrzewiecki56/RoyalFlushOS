#include "dysk.h"
#include "inode.h"
#include "Interpreter.h"
#include "Memory.h"
#include "PCB.h"
#include "ProcessManager.h"
#include "Semaphore.h"
#include "BetterShell.h"
#include "Scheduler.h"
#include <iostream>
int main()
{
	//intro();
	
	BetterShell shell;
	shell.initialize();
	

	getchar();
	return 0;
}
