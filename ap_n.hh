#ifndef AP_N_H
#define AP_N_H

#include <iostream>
#include <vector>

class ap_n {
	public:
		using base_t = unsigned long long;
		static base_t const base {0xffffffffffffffff};

		ap_n() = default;
		ap_n(std::initializer_list<base_t>);

		ap_n& operator<<=(unsigned int);
		ap_n& operator>>=(unsigned int);

		ap_n& operator+=(ap_n const&);
		ap_n& operator-=(ap_n const&);
		ap_n& operator*=(ap_n&&);
		ap_n& operator/=(ap_n const&);
		ap_n& operator%=(ap_n const&);

		ap_n& catch_quotient(ap_n&);
		ap_n& catch_reminder(ap_n&);

		bool operator==(ap_n const&) const;
		bool operator<=(ap_n const&) const;
		bool operator>=(ap_n const&) const;

		std::ostream& out(std::ostream&) const;

		explicit operator bool();

	private:
		static base_t const bits {64};

		std::vector<base_t> index;
		std::vector<base_t> quotient;
		std::vector<base_t> reminder;

		size_t size() const;
		void prune();
		void division(ap_n const&);
};

ap_n operator<<(ap_n const&, unsigned int);
ap_n operator<<(ap_n&&, unsigned int);

ap_n operator>>(ap_n const&, unsigned int);
ap_n operator>>(ap_n&&, unsigned int);

ap_n operator+(ap_n const&, ap_n const&);
ap_n operator+(ap_n&&, ap_n const&);
ap_n operator+(ap_n const&, ap_n&&);

ap_n operator-(ap_n const&, ap_n const&);
ap_n operator-(ap_n&&, ap_n const&);

ap_n& operator*=(ap_n&, ap_n const&);
ap_n operator*(ap_n const&, ap_n const&);
ap_n operator*(ap_n&&, ap_n const&);
ap_n operator*(ap_n const&, ap_n&&);

ap_n operator/(ap_n const&, ap_n const&);
ap_n operator/(ap_n&&, ap_n const&);

ap_n operator%(ap_n const&, ap_n const&);
ap_n operator%(ap_n&&, ap_n const&);

bool operator!=(ap_n const&, ap_n const&);
bool operator<(ap_n const&, ap_n const&);
bool operator>(ap_n const&, ap_n const&);

std::ostream& operator<<(std::ostream&, ap_n const&);

#endif
