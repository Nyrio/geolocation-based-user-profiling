// ConsoleApplication1.cpp�: d�finit le point d'entr�e pour l'application console.
//

#include "stdafx.h"
#include <iostream>
#include "Core.h"


int main()
{
	std::cout << "Main called" << std::endl;

	services::Core c;

	std::cin.get();
    return 0;
}

