#include <iostream>
#include "ap_n.hh"

int main(int argc, char *argv[]) {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	ap_n x {15,  5, 1, 2, 3, 4, 5};
	ap_n y { 3, 10, 6, 5, 4, 3, 2};
	std::cout << x+y << '\n';

	x = {0xf, 0xf, 0xf};
	y = {0xf, 0xf, 0xf};
	std::cout << x+y << '\n';

	ap_n z {1};
	for (int i = 0; i < 12; ++i)
		std::cout << (z << i) << '\n';

	ap_n e {};
	std::cout << e << '\n';

	for (int i = 0; i < 12; ++i)
		std::cout << (x << i) << '\n';

	std::cout << "Hello, world!\n";
	return 0;
}
