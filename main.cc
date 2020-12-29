#include <iostream>
#include "ap_n.hh"

int main(int argc, char *argv[]) {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	ap_n x {0x1};
	std::cout << (x >> 1) << '\n';

	std::cout << "Hello, world!\n";
	return 0;
}
