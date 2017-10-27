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

struct A{};
struct B: public A {};
struct C: public B {};
struct D: public C {};


int main() {
	using numbers = List<Unsigned<1>, Unsigned<3>, Unsigned<5>, Unsigned<7>>;

	std::cout << Size<numbers> << std::endl; // expecting 4

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
	std::cout << Size<numbers3> << std::endl; // expecting 5
	std::cout << Size<EraseDuplicates<Append<numbers3, Unsigned<7>>>> << std::endl; //expecting 4

	using num = Minus<Plus<Unsigned<10>, Unsigned<20>>, Unsigned<5>>;
	std::cout << num::value << std::endl; // expecting 25

	using temp = ForEach<numbers, plus_100>;
	std::cout << TypeAt<temp, 1>::value << std::endl; // expecting 103

	using intlist = List<Integer<-100>, Integer<-1>, Integer<100>>;
	std::cout << TypeAt<ForEach<intlist, plus_100>, 1>{} << std::endl; // expecting 99;

	std::cout << Plus<Integer<1>, Integer<2>, Integer<3>, Integer<4>>::value << std::endl; // expecting 10
	std::cout << Minus<Integer<1>, Integer<2>, Integer<3>, Integer<4>>::value << std::endl; // expecting -1, from right to left

	std::cout << TypeAt<Replace<List<Integer<0>>, Integer<0>, Integer<100>>, 0>::value << std::endl; // expecting 100

	std::cout << TypeAt<Insert<List<Integer<0>, Integer<1>>, Integer<-1>, 0>, 2>::value << std::endl; // expecting 1

	std::cout << IndexOf<DerivedToFront<List<A, B, C, D>>, A> << std::endl; // expecting 3 

}
