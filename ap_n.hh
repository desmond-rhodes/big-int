#ifndef AP_N_H
#define AP_N_H

#include <iostream>
#include <vector>

class ap_n {
	public:
		using base_t = unsigned int;
		static const base_t base;
		/* counting from 0, i.e. base-10 is written as 9, base-16 as 15 */

		ap_n();
		ap_n(std::initializer_list<base_t>);

		ap_n& operator+=(const ap_n&);
		ap_n& operator+(const ap_n&) const;

		std::ostream& out(std::ostream&) const;

	private:
		std::vector<base_t> index;
};

std::ostream& operator<<(std::ostream&, const ap_n&);

#endif
