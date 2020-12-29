#include <iostream>
#include "ap_n.hh"

int main(int argc, char *argv[]) {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	ap_n x {0,  0, 1, 2, 3, 4, 5};
	ap_n y {0, 10, 6, 5, 4, 3, 2};
	x += y;
	std::cout << x << '\n';

	std::cout << "Hello, world!\n";
	return 0;
}
