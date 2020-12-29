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
	decltype(index.size()) remove {0};
	for (auto i = index.rbegin(); i != index.rend(); ++i)
		if (*i) break;
		else ++remove;
	index.resize(index.size()-remove);
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
	index.resize(index.size() + t / bits);

	auto i = index.size();
	auto j = i - t / bits;

	if (j < i) {
		while (j > 0)
			index[--i] = index[--j];
		while (i > 0)
			index[--i] = 0;
	}

	t %= bits;
	if (t)
		index.push_back(0);

	base_t lo = (base >> t) & base;
	base_t hi = ~lo & base;

	i = index.size();
	while (i > 1) {
		--i;
		index[i] = (index[i] & lo) << t | (index[i-1] & hi) >> (bits-t);
	}
	index[0] = (index[0] & lo) << t;

	return prune();
}

ap_n& ap_n::operator>>=(unsigned int t) {
	decltype(index.size()) i, j = t / bits;

	if (j >= index.size()) {
		index = {};
	} else {
		if (j > 0)
			for (i = 0;	j < index.size(); ++i, ++j)
				index[i] = index[j];
		index.resize(index.size() - t / bits);

		t %= bits;
		base_t hi = (base << t) & base;
		base_t lo = ~hi & base;

		for (i = 0; i < index.size()-1; ++i)
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
