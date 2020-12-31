#include "ap_n.hh"
#include <algorithm>
#include <limits>

/* Class Properties */

static_assert(
	std::numeric_limits<ap_n::base_t>::max() >= 0xffffffff,
	"Base storage type is too small."
);

/* Private Member Functions */

ap_n::size_type ap_n::size() const {
	auto s = index.size();
	for (; s > 0; --s)
		if (index[s-1])
			break;
	return s;
}

ap_n& ap_n::prune() {
	index.resize(size());
	return *this;
}

/* Public Member Functions */

ap_n::ap_n(std::initializer_list<base_t> i)
	:index{i}
{
	std::reverse(index.begin(), index.end());
}

ap_n& ap_n::operator+=(const ap_n& x) {
	auto& n =   index;
	auto& m = x.index;
	auto ms = x.size();
	if (size() < ms) n.resize(ms+1, 0);
	else n.push_back(0);

	base_t carry {0};
	size_type i {0};

	for (; i < ms; ++i) {
		n[i] = (n[i] + m[i] + carry) & base;
		carry = (carry) ? n[i] <= m[i] : n[i] < m[i];
	}
	for (; carry; ++i) {
		n[i] = (n[i] + carry) & base;
		carry = !n[i];
	}

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
		index.clear();
		return *this;
	}

	if (s) {
		for (size_type i = s; i < index.size(); ++i)
			index[i-s] = index[i];
		index.resize(index.size() - s);
	}

	if (t) {
		base_t hi = (base << t) & base;
		base_t lo = ~hi & base;
		for (size_type i = 0; i < index.size()-1; ++i)
			index[i] = (index[i] & hi) >> t | (index[i+1] & lo) << (bits-t);
		index.back() = (index.back() & hi) >> t;
	}

	return prune();
}

ap_n& ap_n::operator*=(const ap_n& x) {
	ap_n m {x};
	ap_n p {};
	while (index.size()) {
		if (index[0] & 1)
			p += m;
		*this >>= 1;
		m <<= 1;
	}
	*this = std::move(p);
	return *this;
}

bool ap_n::operator==(const ap_n& x) const {
	auto ns =   size();
	auto ms = x.size();
	if (ns != ms) return false;
	while (ns) if (index[--ns] != x.index[--ms]) return false;
	return true;
}

bool ap_n::operator<=(const ap_n& x) const {
	auto ns =   size();
	auto ms = x.size();
	if (ns > ms) return false;
	if (ns < ms) return true;
	while (ns) {
		--ns, --ms;
		if (index[ns] > x.index[ms]) return false;
		if (index[ns] < x.index[ms]) return true;
	}
	return true;
}

bool ap_n::operator>=(const ap_n& x) const {
	auto ns =   size();
	auto ms = x.size();
	if (ns < ms) return false;
	if (ns > ms) return true;
	while (ns) {
		--ns, --ms;
		if (index[ns] < x.index[ms]) return false;
		if (index[ns] > x.index[ms]) return true;
	}
	return true;
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

/* Overload Operators */

ap_n operator+(const ap_n& n, const ap_n& m) {
	ap_n tmp {n};
	tmp += m;
	return tmp;
}
ap_n operator+(ap_n&& n, const ap_n& m) {
	n += m;
	return std::move(n);
}
ap_n operator+(const ap_n& n, ap_n&& m) {
	m += n;
	return std::move(m);
}

ap_n operator<<(const ap_n& n, unsigned int t) {
	ap_n tmp {n};
	tmp <<= t;
	return tmp;
}
ap_n operator<<(ap_n&& n, unsigned int t) {
	n <<= t;
	return std::move(n);
}

ap_n operator>>(const ap_n& n, unsigned int t) {
	ap_n tmp {n};
	tmp >>= t;
	return tmp;
}
ap_n operator>>(ap_n&& n, unsigned int t) {
	n >>= t;
	return std::move(n);
}

ap_n operator*(const ap_n& n, const ap_n& m) {
	ap_n tmp {n};
	tmp *= m;
	return tmp;
}
ap_n operator*(ap_n& n, const ap_n& m) {
	n *= m;
	return std::move(n);
}
ap_n operator*(const ap_n& n, ap_n& m) {
	m *= n;
	return std::move(m);
}

bool operator!=(const ap_n& n, const ap_n& m) { return !(n == m); }
bool operator<(const ap_n& n, const ap_n& m) { return !(n >= m); }
bool operator>(const ap_n& n, const ap_n& m) { return !(n <= m); }

std::ostream& operator<<(std::ostream& os, const ap_n& n) { return n.out(os); }
