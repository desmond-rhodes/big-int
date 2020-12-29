#include "ap_n.hh"
#include <algorithm>
#include <limits>

static_assert(
	std::numeric_limits<ap_n::base_t>::max() >= 0xffffffff,
	"Base storage type is too small."
);

const ap_n::base_t ap_n::base = 9;

ap_n::ap_n() :index{0} {}

ap_n::ap_n(std::initializer_list<base_t> i)
	:index{i}
{
	std::reverse(index.begin(), index.end());
}

ap_n& ap_n::operator+=(const ap_n& x) {
	auto& n =   index;
	auto& m = x.index;

	auto i = m.size() - n.size();
	for (; i > 0; --i)
		n.push_back(0);
	auto s = n.size();

	bool carry {false};
	base_t sum;

	for (i = 0; i < s; ++i) {
		sum = n[i] + m[i];
		if (sum > base) {
			sum -= 1;
			sum -= base;
		}
		bool carry_old {carry};
		carry = sum < n[i] || sum < m[i];

		if (carry_old)
			sum += 1;
		n[i] = sum;
	}
	if (carry)
		n.push_back(1);

	return *this;
}

std::ostream& ap_n::out(std::ostream& os) const {
	os << '[';
	auto i = index.rbegin();
	if (i != index.rend()) for (;;) {
		os << *i;
		if (++i == index.rend())
			break;
		os << ' ';
	}
	return os << ']';
}

std::ostream& operator<<(std::ostream& os, const ap_n& n) {
	return n.out(os);
}
