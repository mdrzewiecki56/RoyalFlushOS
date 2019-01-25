#include "dysk.h"
#include "inode.h"
#include "Interpreter.h"
#include "Memory.h"
#include "PCB.h"
#include "ProcessManager.h"
#include "Semaphore.h"
#include "Shell.h"
#include "Scheduler.h"
#include <iostream>
void intro();

int main()
{
	intro();
	
	Shell shell;
	shell.run();
	

	getchar();
	return 0;
}

void intro()
{
	
		Sleep(2000);
		std::cout << R"(
                                        
                                        


								    `-//++o/`                  
								 -oNMMMMMMMMMmyyyso:/-         
							      `+mMMMMMMMMMMMMMMMMMMMMMNmho.    
							     /MMMMMMMMMMMMMMMMMMMMMMMMMMMMNhyyo
							    -MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN+-`
							   `NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM:   
							   `NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMy    
							   
									  
                              
                               



	
	
	
	
	
	


	
	
                                        
											Wczytywanie systemu . . .
										     (C) opOS Operating System 2018
)" << '\n';

		Sleep(1500);
		system("cls");

		std::cout << R"(
                                        
                                        


								    `-//++o/`                  
								 -oNMMMMMMMMMmyyyso:/-         
							      `+mMMMMMMMMMMMMMMMMMMMMMNmho.    
							     /MMMMMMMMMMMMMMMMMMMMMMMMMMMMNhyyo
							    -MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN+-`
							   `NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM:   
							   `NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMy    
							   -MMMMMMMMMMMMMMMMMMMMMNyoyMMMMM.    
							  -yMh+:omMMMo:-.``MMMhNMy-   -hMMm     
							 oMs.    /d/     `oyhhy/+so     `-.     
							sM:      `.          ___           ___         ___           ___                   
							Nh        	    /\  \         /\  \       /\  \         /\__\
							hm`                /::\  \       /::\  \     /::\  \       /:/ _/_
							.Ny`		  /:/\:\  \     /:/\:\__\   /:/\:\  \     /:/ /\  \ 
							 
									  
                              
                               
                                        




	

											Wczytywanie systemu . . .
										     (C) opOS Operating System 2018
)" << '\n';
		Sleep(1500);
		system("cls");

		std::cout << R"(
                                        
                                        


								    `-//++o/`                  
								 -oNMMMMMMMMMmyyyso:/-         
							      `+mMMMMMMMMMMMMMMMMMMMMMNmho.    
							     /MMMMMMMMMMMMMMMMMMMMMMMMMMMMNhyyo
							    -MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN+-`
							   `NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM:   
							   `NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMy    
							   -MMMMMMMMMMMMMMMMMMMMMNyoyMMMMM.    
							  -yMh+:omMMMo:-.``MMMhNMy-   -hMMm     
							 oMs.    /d/     `oyhhy/+so     `-.     
							sM:      `.          ___           ___         ___           ___                   
							Nh        	    /\  \         /\  \       /\  \         /\__\
							hm`                /::\  \       /::\  \     /::\  \       /:/ _/_
							.Ny`		  /:/\:\  \     /:/\:\__\   /:/\:\  \     /:/ /\  \ 
							 `hm/` :`        /:/  \:\  \   /:/ /:/  /  /:/  \:\  \   /:/ /::\  \
								.+sss`  /:/__/ \:\__\ /:/_/:/  /  /:/__/ \:\__\ /:/_/:/\:\__\
								        \:\  \ /:/  / \:\/:/  /   \:\  \ /:/  / \:\/:/ /:/  /
									 \:\  /:/  /   \::/__/     \:\  /:/  /   \::/ /:/  / 
									  \:\/:/  /     \:\  \      \:\/:/  /     \/_/:/  /  
									   \::/  /       \:\__\      \::/  /        /:/  /   
									    \/__/         \/__/       \/__/         \/__/  	
									  
                              
                               
                                        
											Wczytywanie systemu . . .
										     (C) opOS Operating System 2018
)" << '\n';


		Sleep(1500);
		system("cls");
	
}
