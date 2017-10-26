#ifndef UNSIGNED_HPP
#define UNSIGNED_HPP

#include <limits> 
#include <iostream>

template <char ch>
struct Character {
	constexpr static inline char value = ch;
};

template <char ch>
std::ostream& operator<< (std::ostream & os, Character<ch> t) {
	return (os << ch);
}

template <int num>
struct Integer {
	constexpr static inline int value = num;
};

template <int num>
std::ostream& operator<< (std::ostream & os, Integer<num> t) {
	return (os << num);
}

template <size_t num>
struct Unsigned { 
	constexpr static inline unsigned int value = num;
};

template <size_t num>
std::ostream& operator<< (std::ostream & os, Unsigned<num> t) {
	return (os << num);
}

///////////////////////////////
//////////////PLUS/////////////
///////////////////////////////

template <typename T1, typename T2>
struct PlusType {};

template <size_t a, size_t b>
struct PlusType<Unsigned<a>, Unsigned<b>> {
	using type = Unsigned<a + b>;
};

template <size_t a>
struct PlusType<Unsigned<a>, TL::NullType> {
	using type = Unsigned<a>;
};

template <int a, int b>
struct PlusType<Integer<a>, Integer<b>> {
	using type = Integer<a + b>;
};

template <int a>
struct PlusType<Integer<a>, TL::NullType> {
	using type = Integer<a>;
};


template <typename... T>
struct PlusImpl {};

template <size_t a, size_t... b>
struct PlusImpl<Unsigned<a>, Unsigned<b>...> {
	using type = typename PlusType<Unsigned<a>, typename PlusImpl<Unsigned<b>...>::type>::type;
};

template <size_t a>
struct PlusImpl<Unsigned<a>> {
	using type = Unsigned<a>;
};

template <int a, int... b>
struct PlusImpl<Integer<a>, Integer<b>...> {
	using type = typename PlusType<Integer<a>, typename PlusImpl<Integer<b>...>::type>::type;
};

template <int a>
struct PlusImpl<Integer<a>> {
	using type = Integer<a>;
};

template <typename... T>
using Plus = typename PlusImpl<T...>::type;


///////////////////////////////
/////////////MINUS/////////////
///////////////////////////////

template <typename T1, typename T2>
struct MinusType {};

template <size_t a, size_t b>
struct MinusType<Unsigned<a>, Unsigned<b>> {
	using type = Unsigned<a - b>;
};

template <size_t a>
struct MinusType<Unsigned<a>, TL::NullType> {
	using type = Unsigned<0>;
};

template <int a, int b>
struct MinusType<Integer<a>, Integer<b>> {
	using type = Integer<a - b>;
};

template <int a>
struct MinusType<Integer<a>, TL::NullType> {
	using type = Integer<0>;
};

template <typename... T>
struct MinusImpl {};

template <size_t a, size_t... b>
struct MinusImpl<Unsigned<a>, Unsigned<b>...> {
	using type = typename MinusType<Unsigned<a>, typename MinusImpl<Unsigned<b>...>::type>::type;
};

template <size_t a>
struct MinusImpl<Unsigned<a>> {
	using type = Unsigned<a>;
};

template <int a, int... b>
struct MinusImpl<Integer<a>, Integer<b>...> {
	using type = typename MinusType<Integer<a>, typename MinusImpl<Integer<b>...>::type>::type;
};

template <int a>
struct MinusImpl<Integer<a>> {
	using type = Integer<a>;
};

template <typename... T>
using Minus = typename MinusImpl<T...>::type;


#endif