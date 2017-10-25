#include <iostream>
#include "typelist.hpp"
#include "numerics.hpp"

template <typename T>
struct plus_100 {};

template <size_t num>
struct plus_100<Unsigned<num>> {
	using type = Unsigned<num + 100>;
};

template <int num>
struct plus_100<Integer<num>> {
	using type = Integer<num + 100>;
};


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

	using num = Minus<Plus<Unsigned<10>, Unsigned<20>>, Unsigned<5>>;
	std::cout << num::value << std::endl; // expecting 25

	using temp = ForEach<numbers, plus_100>;
	std::cout << temp::Tail::Head::value << std::endl; // expecting 103

	using intlist = List<Integer<-100>, Integer<-1>, Integer<100>>;
	std::cout << ForEach<intlist, plus_100>::Head::value << std::endl; // expecting 0;

}
