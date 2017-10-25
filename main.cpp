#include <iostream>
#include "typelist.hpp"

template <size_t Num>
struct Unsigned { 
	constexpr static inline int value = 1 + Unsigned<Num - 1>::value;
 };

template <>
struct Unsigned<0> { constexpr static inline int value = 0; };

int main() {
	using empty = TL::MakeList_t<>;
	using types = TL::MakeList_t<int, long, double, char>;
	using numbers = TL::MakeList_t<Unsigned<1>, Unsigned<3>, Unsigned<5>, Unsigned<7>>;
	// explicitly ask for an item in the list
	std::cout << numbers::Tail::Tail::Tail::Head::value << std::endl; //expecting 7
	// use TypeAt
	std::cout << TL::TypeAt_t<numbers, 2>::value << std::endl; // expecting 5
	// use IndexOf
	std::cout << TL::IndexOf_v<numbers, Unsigned<7>> << std::endl; // expecting 3
	std::cout << TL::IndexOf_v<numbers, Unsigned<4>> << std::endl; // expecting -1

	using numbers_type2 = TL::Erase_t<numbers, Unsigned<7>>;
	std::cout << TL::IndexOf_v<numbers_type2, Unsigned<7>> << std::endl;

	std::cout << TL::is_nulltype_v<TL::MakeList<>> << std::endl;
	std::cout << TL::is_typelist_v<numbers> << std::endl;
	std::cout << TL::is_typelist_v<int> << std::endl;
	auto numbers3 = TL::Append_t<TL::Append_t<numbers, Unsigned<7>>, Unsigned<7>>();

}