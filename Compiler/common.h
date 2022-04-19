#pragma once

#include <cmath>
#include <iomanip>
#include <list>
#include <sstream>
#include <string>
#include <vector>

typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned long long ulong;

template <class A, class B>
std::list<B>& operator<<(std::list<B>& l, A const& e) {
	l.push_back((B)e);
	return l;
}

template <class A, class B>
std::list<B>* operator<<(std::list<B>* l, A const& e) {
	l->push_back((B)e);
	return l;
}

template <class A, class B>
std::vector<B>& operator<<(std::vector<B>& v, A const& e) {
	v.push_back((B)e);
	return v;
}

template <class A, class B>
std::vector<B>* operator<<(std::vector<B>* v, A const& e) {
	v->push_back((B)e);
	return v;
}

template <class T>
std::string toString(T t, int base, int precision) {
	std::ostringstream s;

	s << std::setbase(base) << std::setprecision(precision) << t;

	return s.str();
}

template <class T>
T fromString(const std::string& str) {
	std::istringstream s(str);

	T t;
	s >> t;

	return t;
}

template <>
inline double fromString<double>(std::string const& str) {
	if (str == "inf")
		return INFINITY;

	if (str == "nan")
		return NAN;

	std::istringstream s(str);

	double t;
	s >> t;

	return t;
}

uint ceilToPowerOf2(uint n);