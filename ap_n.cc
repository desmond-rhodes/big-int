#include "ap_n.hh"
#include <algorithm>
#include <limits>

/* Properties */

static_assert(
	std::numeric_limits<ap_n::base_t>::max() >= ap_n::base,
	"Base storage type is too small."
);

size_t ap_n::size() const {
	auto s {index.size()};
	for (; s > 0; --s)
		if (index[s-1])
			break;
	return s;
}

void ap_n::prune() { index.resize(size()); }

/* Member */

ap_n::ap_n(std::initializer_list<base_t> i)
	:index{i}
{
	std::reverse(index.begin(), index.end());
}

ap_n& ap_n::operator<<=(unsigned int t) {
	auto s {t / bits};
	t %= bits;

	if (t) {
		base_t lo {(base >> t) & base};
		base_t hi {~lo & base};
		index.push_back(0);
		for (auto i {index.size()-1}; i > 0; --i)
			index[i] = (index[i] & lo) << t | (index[i-1] & hi) >> (bits-t);
		index[0] = (index[0] & lo) << t;
	}

	if (s) {
		auto j {index.size()};
		index.resize(index.size() + s);
		auto i {index.size()};
		while (j > 0) index[--i] = index[--j];
		while (i > 0) index[--i] = 0;
	}

	prune();
	return *this;
}

ap_n& ap_n::operator>>=(unsigned int t) {
	auto s {t / bits};
	t %= bits;

	if (s >= index.size()) {
		index.clear();
		return *this;
	}

	if (s) {
		for (size_t i {s}; i < index.size(); ++i)
			index[i-s] = index[i];
		index.resize(index.size() - s);
	}

	if (t) {
		base_t hi {(base << t) & base};
		base_t lo {~hi & base};
		for (size_t i {0}; i < index.size()-1; ++i)
			index[i] = (index[i] & hi) >> t | (index[i+1] & lo) << (bits-t);
		index.back() = (index.back() & hi) >> t;
	}

	prune();
	return *this;
}

ap_n& ap_n::operator+=(ap_n const& x) {
	auto& n {  index };
	auto& m {x.index };
	auto ns {  size()};
	auto ms {x.size()};
	if (ns < ms) n.resize(ms+1, 0);
	else if (index.size() == ns) n.push_back(0);

	base_t carry {0};
	size_t i {0};

	for (; i < ms; ++i) {
		n[i] = (n[i] + m[i] + carry) & base;
		carry = (carry) ? n[i] <= m[i] : n[i] < m[i];
	}
	for (; carry; ++i) {
		n[i] = (n[i] + 1) & base;
		carry = !n[i];
	}

	prune();
	return *this;
}

ap_n& ap_n::operator-=(ap_n const& x) {
	auto& n {  index };
	if (*this <= x) {
		n.clear();
		return *this;
	}
	auto& m {x.index };
	auto ms {x.size()};

	base_t borrow {0};
	size_t i {0};

	for (; i < ms; ++i) {
		base_t s {(n[i] - m[i] - borrow) & base};
		borrow = (borrow) ? s >= n[i] : s > n[i];
		n[i] = s;
	}
	for (; borrow; ++i) {
		borrow = !n[i];
		n[i] = (n[i] - 1) & base;
	}

	prune();
	return *this;
}

ap_n& ap_n::operator*=(ap_n&& m) {
	ap_n p;
	while (index.size()) {
		if (index[0] & 1)
			p += m;
		*this >>= 1;
		m <<= 1;
	}
	*this = std::move(p);
	return *this;
}

ap_n& ap_n::operator/=(ap_n const& m) {
	division(m);
	catch_quotient(*this);
	return *this;
}

ap_n& ap_n::operator%=(ap_n const& m) {
	division(m);
	catch_reminder(*this);
	return *this;
}

ap_n& ap_n::catch_quotient(ap_n& x) {
	index = std::move(x.quotient);
	x.quotient.clear();
	return *this;
}

ap_n& ap_n::catch_reminder(ap_n& x) {
	index = std::move(x.reminder);
	x.reminder.clear();
	return *this;
}

void ap_n::division(ap_n const& d) {
	static ap_n const one {1};
	auto i {size()};
	ap_n q;
	ap_n r;
	while (i--) for (auto b {static_cast<base_t>(1) << bits-1}; b; b >>= 1) {
		q <<= 1;
		r <<= 1;
		if (index[i] & b)
			r += one;
		if (r >= d) {
			r -= d;
			q += one;
		}
	}
	quotient = std::move(q.index);
	reminder = std::move(r.index);
}

bool ap_n::operator==(ap_n const& x) const {
	auto ns {  size()};
	auto ms {x.size()};
	if (ns != ms) return false;
	while (ns) if (index[--ns] != x.index[--ms]) return false;
	return true;
}

bool ap_n::operator<=(ap_n const& x) const {
	auto ns {  size()};
	auto ms {x.size()};
	if (ns > ms) return false;
	if (ns < ms) return true;
	while (ns) {
		--ns, --ms;
		if (index[ns] > x.index[ms]) return false;
		if (index[ns] < x.index[ms]) return true;
	}
	return true;
}

bool ap_n::operator>=(ap_n const& x) const {
	auto ns {  size()};
	auto ms {x.size()};
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
	os << index.size() << ": [";
	auto i {index.rbegin()};
	if (i != index.rend()) for (;;) {
		os << *i;
		if (++i == index.rend())
			break;
		os << ' ';
	}
	return os << ']';
}

/* explicit */ ap_n::operator bool() { return size(); }

/* Non-member */

ap_n operator<<(ap_n const& n, unsigned int t) {
	ap_n tmp {n};
	tmp <<= t;
	return tmp;
}
ap_n operator<<(ap_n&& n, unsigned int t) {
	n <<= t;
	return std::move(n);
}

ap_n operator>>(ap_n const& n, unsigned int t) {
	ap_n tmp {n};
	tmp >>= t;
	return tmp;
}
ap_n operator>>(ap_n&& n, unsigned int t) {
	n >>= t;
	return std::move(n);
}

ap_n operator+(ap_n const& n, ap_n const& m) {
	ap_n tmp {n};
	tmp += m;
	return tmp;
}
ap_n operator+(ap_n&& n, ap_n const& m) {
	n += m;
	return std::move(n);
}
ap_n operator+(ap_n const& n, ap_n&& m) {
	m += n;
	return std::move(m);
}

ap_n operator-(ap_n const& n, ap_n const& m) {
	ap_n tmp {n};
	tmp -= m;
	return tmp;
}
ap_n operator-(ap_n&& n, ap_n const& m) {
	n -= m;
	return n;
}

ap_n& operator*=(ap_n& n, ap_n const& m) {
	ap_n tmp {m};
	n *= std::move(tmp);
	return n;
}
ap_n operator*(ap_n const& n, ap_n const& m) {
	ap_n tmp {n};
	tmp *= m;
	return tmp;
}
ap_n operator*(ap_n&& n, ap_n const& m) {
	n *= m;
	return std::move(n);
}
ap_n operator*(ap_n const& n, ap_n&& m) {
	m *= n;
	return std::move(m);
}

ap_n operator/(ap_n const& n, ap_n const& m) {
	ap_n tmp {n};
	tmp /= m;
	return tmp;
}
ap_n operator/(ap_n&& n, ap_n const& m) {
	n /= m;
	return std::move(n);
}

ap_n operator%(ap_n const& n, ap_n const& m) {
	ap_n tmp {n};
	tmp %= m;
	return tmp;
}
ap_n operator%(ap_n&& n, ap_n const& m) {
	n %= m;
	return std::move(n);
}

bool operator!=(ap_n const& n, ap_n const& m) { return !(n == m); }
bool operator<(ap_n const& n, ap_n const& m) { return !(n >= m); }
bool operator>(ap_n const& n, ap_n const& m) { return !(n <= m); }

std::ostream& operator<<(std::ostream& os, ap_n const& n) { return n.out(os); }
