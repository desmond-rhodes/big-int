#include <iostream>
#include "ap_n.hh"

void test();

int main(int argc, char *argv[]) {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	test();

	std::cout << "Hello, world!\n";
	return 0;
}

void test() {

		ap_n x, y;

	std::cout << '\n'; // ------------------------------

		std::cout << "Division test\n";

		x = {0x2, 0xf, 0x4, 0x9, 0xe}; // 193694 or 2f49e
		y = {0x5, 0xf, 0xe, 0x2, 0x6}; // 392742 or 5fe26
		std::cout << x << ' ' << y << '\n';
		std::cout << y/x << ' ' << y%x << '\n'; // 2 or 2 - 5354 or 14ea

		std::cout << '\n';

		x = {0xa};
		y = {0x2};
		std::cout << x/y << ' ' << x%y << '\n';
		std::cout << y/x << ' ' << y%x << '\n';

		x = {0xf, 0xf};
		y = {0x2};
		std::cout << x/y << ' ' << x%y << '\n';
		y = {};
		std::cout << x/y << ' ' << x%y << '\n';
		y = {1};
		std::cout << x/y << ' ' << x%y << '\n';

	std::cout << '\n'; // ------------------------------

}
