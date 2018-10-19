// ConsoleApplication1.cpp : définit le point d'entrée pour l'application console.
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

