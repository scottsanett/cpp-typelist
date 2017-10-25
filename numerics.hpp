#ifndef UNSIGNED_HPP
#define UNSIGNED_HPP

#include <limits> 

template <char ch>
struct Character {
	constexpr static inline char value = ch;
};

template <int num>
struct Integer {
	constexpr static inline unsigned int value = num;
};

template <size_t num>
struct Unsigned { 
	constexpr static inline unsigned int value = num;
};

template <typename T1, typename T2>
struct PlusType {};

template <size_t a, size_t b>
struct PlusType<Unsigned<a>, Unsigned<b>> {
	using type = Unsigned<a + b>;
};

template <typename T1, typename T2>
using Plus = typename PlusType<T1, T2>::type; 

template <typename T1, typename T2>
struct MinusType {};

template <size_t a, size_t b>
struct MinusType<Unsigned<a>, Unsigned<b>> {
	using type = Unsigned<a - b>;
};

template <typename T1, typename T2>
using Minus = typename MinusType<T1, T2>::type;


#endif