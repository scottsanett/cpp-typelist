#ifndef TYPELIST_HPP
#define TYPELIST_HPP

#include <type_traits>
#include <utility>

// NullType serves as a null marker for types
class NullType {};

// this is a legal type to inherit from
struct EmptyType {};

template <class T, class U>
struct Typelist {
	using Head = T;
	using Tail = U;
};

// everything related to typelists except the definition of Typelist itself lives in the TL namespace
namespace TL {
	template <typename T, typename = std::void_t<>, typename = std::void_t<>>
	struct has_head_and_tail {
		inline constexpr static bool value = false;
	};
	
	template <typename T, typename T1 = std::void_t<>, typename T2 = std::void_t<>>
	inline constexpr static bool has_head_and_tail_v = has_head_and_tail<T, T1, T2>::value;
	
	template <typename T>
	struct has_head_and_tail<T,
		std::void_t<typename T::Head>, 
		std::void_t<typename T::Tail>> {
		inline constexpr static bool value = true;
	};
	
	template <typename T>
	struct is_nulltype {
		inline constexpr static bool value = false;
	};
	
	template <typename T>
	inline constexpr static bool is_nulltype_v = is_nulltype<T>::value;
	
	template <>
	struct is_nulltype<NullType>: std::true_type {};
	
	template <typename T>
	struct is_typelist {
		private:
			constexpr static bool get() {
				if constexpr (is_nulltype_v<T>) return true;
				else if constexpr (!has_head_and_tail_v<T>) return false;
				else if constexpr (is_nulltype_v<typename T::Head>) return false;
				else return is_typelist<typename T::Tail>::value;
			}
	
		public:
			inline constexpr static bool value = get();
	};

	template <typename T>
	inline constexpr static bool is_typelist_v = is_typelist<T>::value;


	// a template I wrote myself to make a list, seems to be working fine
	/**
	 * Args... take 0 or more arguments, so an empty list is now possible
	 */
	template <typename... Args>
	struct MakeList {
		using type = NullType;
	};

	template <typename... Args>
	using MakeList_t = typename MakeList<Args...>::type;

	template <typename T, typename... Args>
	struct MakeList<T, Args...> {
		using type = Typelist<T, typename MakeList<Args...>::type>;
	};

	template <class TList> 
	struct Length {
		inline constexpr static int value = 1 + Length<typename TList::Tail>::value;
	};

	template <class TList>
	inline constexpr static int Length_v = Length<TList>::value;
	
	template <> 
	struct Length<NullType> {
		inline constexpr static int value = 0;
	};


	template <class TList, unsigned int index>
	struct TypeAt {
		using type = typename TypeAt<typename TList::Tail, index - 1>::type;
	};

	template <class TList, unsigned int index>
	using TypeAt_t = typename TypeAt<TList, index>::type;

	template <class Head, class Tail>
	struct TypeAt<Typelist<Head, Tail>, 0> {
		using type = Head;
	};


	// T is a typelist and has a tail member type
	template <typename T, typename D>
	struct IndexOf {
	private:
		inline constexpr static int temp = IndexOf<typename T::Tail, D>::value;
	public:
		inline constexpr static int value = (temp == -1) ? -1 : temp + 1;
	};

	template <typename T, typename D>
	inline constexpr static int IndexOf_v = IndexOf<T, D>::value;

	template <typename T>
	struct IndexOf<NullType, T> {
		inline constexpr static int value = -1;
	};

	// T doesn't have a Tail, as T is not a typelist
	template <typename T, typename Tail>
	struct IndexOf<Typelist<T, Tail>, T> {
		inline constexpr static int value = 0;
	};

	template <typename List, typename T>
	struct Append {
		using type = Typelist<typename List::Head, typename Append<typename List::Tail, T>::type>;
	};

	template <typename List, typename T>
	using Append_t = typename Append<List, T>::type;

	template <>
	struct Append<NullType, NullType> {
		using type = NullType;
	};

	template <typename T>
	struct Append<NullType, T> {
		using type = Typelist<T, NullType>;
	};

	template <typename Head, typename Tail>
	struct Append<NullType, Typelist<Head, Tail>> {
		using type = Typelist<Head, Tail>;
	};

	template <typename TList, typename T>
	struct Erase {
		using type = Typelist<typename TList::Head, typename Erase<typename TList::Tail, T>::type>;
	};

	template <typename TList, typename T>
	using Erase_t = typename Erase<TList, T>::type;

	template <typename T>
	struct Erase<NullType, T> {
		using type = NullType;
	};

	template <typename T, typename Tail>
	struct Erase<Typelist<T, Tail>, T> {
		using type = Tail;
	};

	template <typename TList, typename T>
	struct EraseAll {
		using type = Typelist<typename TList::Head, typename EraseAll<typename TList::Tail, T>::type>;
	};

	template <typename TList, typename T>
	using EraseAll_t = typename EraseAll<TList, T>::type;

	template <typename T>
	struct EraseAll<NullType, T> {
		using type = NullType;
	};

	// go all the way down the list removing the type
	template <typename T, typename Tail>
	struct EraseAll<Typelist<T, Tail>, T> {
		using type = EraseAll<Tail, T>;
	};

}



#endif //TYPELIST_HPP
