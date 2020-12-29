#include <iostream>
#include "ap_n.hh"

int main(int argc, char *argv[]) {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	ap_n x {0x2, 0xf, 0x4, 0x9, 0xe}; // 193694 or 2f49e
	ap_n y {0x5, 0xf, 0xe, 0x2, 0x6}; // 392742 or 5fe26
	std::cout << (x*y) << '\n'; // 76071768948 or 11b63b1374

	std::cout << "Hello, world!\n";
	return 0;
}
