#include <iostream>
#include "Core.h"

using namespace std;

void hello_word() {
	services::Core c;
	c.do_stuff();
}

int main() {
	hello_word();
	return 0;
}