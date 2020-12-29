#include "ap_n.hh"
#include <algorithm>
#include <limits>

/* Class Properties */

static_assert(
	std::numeric_limits<ap_n::base_t>::max() >= 0xffffffff,
	"Base storage type is too small."
);

/* Member Functions */

ap_n& ap_n::prune() {
	while (!index.back())
		index.pop_back();
	return *this;
}

ap_n::ap_n(std::initializer_list<base_t> i)
	:index{i}
{
	std::reverse(index.begin(), index.end());
}

ap_n& ap_n::operator+=(const ap_n& x) {
	auto& n =   index;
	auto& m = x.index;

	if (m.size() > n.size())
		n.resize(m.size(), 0);

	base_t carry {0};
	for (decltype(n.size()) i = 0; i < n.size(); ++i) {
		base_t s = (n[i] + m[i]) & base;
		base_t c = s < n[i] || s < m[i];
		n[i] = s + carry;
		carry = c;
	}
	if (carry)
		n.push_back(1);

	return prune();
}

ap_n& ap_n::operator<<=(unsigned int t) {
	auto s = t / bits;
	t %= bits;

	if (t) {
		base_t lo = (base >> t) & base;
		base_t hi = ~lo & base;
		index.push_back(0);
		for (auto i = index.size()-1; i > 0; --i)
			index[i] = (index[i] & lo) << t | (index[i-1] & hi) >> (bits-t);
		index[0] = (index[0] & lo) << t;
	}

	if (s) {
		auto j = index.size();
		index.resize(index.size() + s);
		auto i = index.size();
		while (j > 0) index[--i] = index[--j];
		while (i > 0) index[--i] = 0;
	}

	return prune();
}

ap_n& ap_n::operator>>=(unsigned int t) {
	auto s = t / bits;
	t %= bits;

	if (s >= index.size()) {
		index = {};
		return *this;
	}

	if (s) {
		for (decltype(index.size()) i = s; i < index.size(); ++i)
			index[i-s] = index[i];
		index.resize(index.size() - s);
	}

	if (t) {
		base_t hi = (base << t) & base;
		base_t lo = ~hi & base;
		for (decltype(index.size()) i = 0; i < index.size()-1; ++i)
			index[i] = (index[i] & hi) >> t | (index[i+1] & lo) << (bits-t);
		index.back() = (index.back() & hi) >> t;
	}

	return prune();
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

/* Helper Functions */

ap_n operator+(const ap_n& n, const ap_n& m) {
	ap_n tmp {n};
	return tmp += m;
}
ap_n operator+(ap_n&& n, const ap_n& m) { return n += m; }
ap_n operator+(const ap_n& n, ap_n&& m) { return m += n; }
ap_n operator+(ap_n&& n, ap_n&& m) { return n += m; }

ap_n operator<<(const ap_n& n, unsigned int t) {
	ap_n tmp {n};
	return tmp <<= t;
}
ap_n operator<<(ap_n&& n, unsigned int t) { return n <<= t; }

ap_n operator>>(const ap_n& n, unsigned int t) {
	ap_n tmp {n};
	return tmp >>= t;
}
ap_n operator>>(ap_n&& n, unsigned int t) { return n >>= t; }

std::ostream& operator<<(std::ostream& os, const ap_n& n) {
	return n.out(os);
}
