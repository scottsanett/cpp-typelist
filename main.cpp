#include <iostream>
#include "typelist.hpp"

template <size_t Num>
struct Unsigned { 
	constexpr static inline int value = 1 + Unsigned<Num - 1>::value;
 };

template <>
struct Unsigned<0> { constexpr static inline int value = 0; };

int main() {
	using numbers = List<Unsigned<1>, Unsigned<3>, Unsigned<5>, Unsigned<7>>;

	std::cout << Length<numbers> << std::endl; // expecting 4

	std::cout << numbers::Tail::Tail::Tail::Head::value << std::endl; //expecting 7
	std::cout << TypeAt<numbers, 2>::value << std::endl; // expecting 5

	std::cout << IndexOf<numbers, Unsigned<7>> << std::endl; // expecting 3
	std::cout << IndexOf<numbers, Unsigned<4>> << std::endl; // expecting -1

	using numbers2 = Erase<numbers, Unsigned<7>>;
	std::cout << IndexOf<numbers2, Unsigned<7>> << std::endl; // expecting -1

	std::cout << IsTypelist<List<>> << std::endl; // expecting true
	std::cout << IsTypelist<numbers> << std::endl; // expecting true
	std::cout << IsTypelist<int> << std::endl; // expecting false
	
	using numbers3 = Append<Append<numbers2, Unsigned<7>>, Unsigned<7>>;
	std::cout << Length<numbers3> << std::endl; // expecting 5
	std::cout << Length<EraseDuplicates<Append<numbers3, Unsigned<7>>>> << std::endl; //expecting 4
}
