#include <iostream>
#include <string.h>
#include "Core.h"

using namespace std;

int main(int argc, char* argv[]) {

	// default main arguments
	std::string wp = "";
	// main arguments parsing
	for(int ite = 1 ; ite < argc ; ++ite)
	{
		if(strcmp("-wp",argv[ite]) == 0)
		{
			if(ite+1<argc){
				wp = argv[++ite];
			}else{
				std::cerr<<"-wp parameter must be followed by a string" <<std::endl;
			}
		}
	}
	// end of argument parsings


	services::Core c(wp);

	return 0;
}