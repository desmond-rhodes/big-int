#include <iostream>
#include "ap_n.hh"

int main(int argc, char *argv[]) {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	ap_n x {0x8, 0x0, 0x0, 0x0, 0x0};
	for (int i = 0; i < 24; ++i)
		std::cout << (x >> i) << '\n';

	ap_n y {0x1};
	for (int i = 0; i < 20; ++i)
		std::cout << (y << i) << '\n';

	std::cout << "Hello, world!\n";
	return 0;
}
