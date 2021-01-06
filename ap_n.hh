#ifndef AP_N_H
#define AP_N_H

#include <iostream>
#include <vector>

class ap_n {
	public:
		using base_t = unsigned int;
		static const base_t base {0xf};
		static const base_t bits {4};

		using size_type = std::vector<base_t>::size_type;
		size_type size() const;

		ap_n() = default;
		ap_n(std::initializer_list<base_t>);

		ap_n& operator<<=(unsigned int);
		ap_n& operator>>=(unsigned int);

		ap_n& operator+=(const ap_n&);
		ap_n& operator-=(const ap_n&);
		ap_n& operator*=(ap_n&&);
		ap_n& operator/=(const ap_n&);
		ap_n& operator%=(const ap_n&);

		ap_n& catch_quotient(ap_n&);
		ap_n& catch_reminder(ap_n&);

		bool operator==(const ap_n&) const;
		bool operator<=(const ap_n&) const;
		bool operator>=(const ap_n&) const;

		std::ostream& out(std::ostream&) const;

	private:
		std::vector<base_t> index;
		std::vector<base_t> quotient;
		std::vector<base_t> reminder;

		ap_n& prune();
		void division(const ap_n&);
};

ap_n operator<<(const ap_n&, unsigned int);
ap_n operator<<(ap_n&&, unsigned int);

ap_n operator>>(const ap_n&, unsigned int);
ap_n operator>>(ap_n&&, unsigned int);

ap_n operator+(const ap_n&, const ap_n&);
ap_n operator+(ap_n&&, const ap_n&);
ap_n operator+(const ap_n&, ap_n&&);

ap_n operator-(const ap_n&, const ap_n&);
ap_n operator-(ap_n&&, const ap_n&);

ap_n& operator*=(ap_n&, const ap_n&);
ap_n operator*(const ap_n&, const ap_n&);
ap_n operator*(ap_n&&, const ap_n&);
ap_n operator*(const ap_n&, ap_n&&);

ap_n operator/(const ap_n&, const ap_n&);
ap_n operator/(ap_n&&, const ap_n&);

ap_n operator%(const ap_n&, const ap_n&);
ap_n operator%(ap_n&&, const ap_n&);

bool operator!=(const ap_n&, const ap_n&);
bool operator<(const ap_n&, const ap_n&);
bool operator>(const ap_n&, const ap_n&);

std::ostream& operator<<(std::ostream&, const ap_n&);

#endif
