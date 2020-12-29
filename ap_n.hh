#ifndef AP_N_H
#define AP_N_H

#include <iostream>
#include <vector>

class ap_n {
	public:
		using base_t = unsigned int;
		static const base_t base {0xf};

		ap_n() = default;
		ap_n(std::initializer_list<base_t>);

		ap_n& operator+=(const ap_n&);
		ap_n& operator<<=(unsigned int);
		ap_n& operator>>=(unsigned int);

		std::ostream& out(std::ostream&) const;

	private:
		std::vector<base_t> index;
		static const base_t bits {4};
		ap_n& prune();
};

ap_n operator+(const ap_n&, const ap_n&);
ap_n operator+(ap_n&&, const ap_n&);
ap_n operator+(const ap_n&, ap_n&&);
ap_n operator+(ap_n&&, ap_n&&);

ap_n operator<<(const ap_n&, unsigned int);
ap_n operator<<(ap_n&&, unsigned int);

ap_n operator>>(const ap_n&, unsigned int);
ap_n operator>>(ap_n&&, unsigned int);

std::ostream& operator<<(std::ostream&, const ap_n&);

#endif
