#ifndef TYPELIST_HPP
#define TYPELIST_HPP

#include <type_traits>
#include <utility>

namespace TL {

	// NullType serves as a null marker for types
	class NullType {};
	
	// this is a legal type to inherit from
	struct EmptyType {};
	
	template <class T, class U>
	struct Typelist {
		using Head = T;
		using Tail = U;
	};

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


	template <class TList> 
	struct LengthType {
		inline constexpr static int value = 1 + LengthType<typename TList::Tail>::value;
	};

	template <class TList>
	inline constexpr static int Length = LengthType<TList>::value;
	
	template <> 
	struct LengthType<NullType> {
		inline constexpr static int value = 0;
	};


	template <class TList, unsigned int index>
	struct TypeAtType {
		using type = typename TypeAtType<typename TList::Tail, index - 1>::type;
	};

	template <class Head, class Tail>
	struct TypeAtType<Typelist<Head, Tail>, 0> {
		using type = Head;
	};


	// T is a typelist and has a tail member type
	template <typename T, typename D>
	struct IndexOfType {
	private:
		inline constexpr static int temp = IndexOfType<typename T::Tail, D>::value;
	public:
		inline constexpr static int value = (temp == -1) ? -1 : temp + 1;
	};

	template <typename T>
	struct IndexOfType<NullType, T> {
		inline constexpr static int value = -1;
	};

	// T doesn't have a Tail, as T is not a typelist
	template <typename T, typename Tail>
	struct IndexOfType<Typelist<T, Tail>, T> {
		inline constexpr static int value = 0;
	};

	template <typename List, typename T>
	struct AppendType {
		using type = Typelist<typename List::Head, typename AppendType<typename List::Tail, T>::type>;
	};

	template <>
	struct AppendType<NullType, NullType> {
		using type = NullType;
	};

	template <typename T>
	struct AppendType<NullType, T> {
		using type = Typelist<T, NullType>;
	};

	template <typename Head, typename Tail>
	struct AppendType<NullType, Typelist<Head, Tail>> {
		using type = Typelist<Head, Tail>;
	};

	template <typename TList, typename T>
	struct EraseType {
		using type = Typelist<typename TList::Head, typename EraseType<typename TList::Tail, T>::type>;
	};

	template <typename T>
	struct EraseType<NullType, T> {
		using type = NullType;
	};

	template <typename T, typename Tail>
	struct EraseType<Typelist<T, Tail>, T> {
		using type = Tail;
	};

	template <typename TList, typename T>
	struct EraseAllType {
		using type = Typelist<typename TList::Head, typename EraseAllType<typename TList::Tail, T>::type>;
	};

	template <typename T>
	struct EraseAllType<NullType, T> {
		using type = NullType;
	};

	// go all the way down the list removing the type
	template <typename T, typename Tail>
	struct EraseAllType<Typelist<T, Tail>, T> {
		using type = EraseAllType<Tail, T>;
	};

	template <typename TList>
	struct EraseDuplicatesType {
	private:
		using temp1 = typename EraseDuplicatesType<typename TList::Tail>::type;
		using temp2 = typename EraseType<temp1, typename TList::Head>::type;
	public:
		using type = Typelist<typename TList::Head, temp2>;
	};

	template <>
	struct EraseDuplicatesType<NullType> {
		using type = NullType;
	};

	template <typename TList, template <typename> typename Func>
	struct ForEachType {
		using type = Typelist<
			typename Func<typename TList::Head>::type, 
			typename ForEachType<typename TList::Tail, Func>
		::type>;
	};

	template <typename T, template <typename> typename Func>
	struct ForEachType<Typelist<T, NullType>, Func> {
		using type = Typelist<typename Func<T>::type, NullType>;
	};

	template <template <typename> typename Func>
	struct ForEachType<NullType, Func> {
		using type = NullType;
	};

}

template <typename... Args>	
using List = TL::MakeList_t<Args...>;

template <class TList>
inline constexpr static int Length = TL::LengthType<TList>::value;

template <class TList, unsigned int index>
using TypeAt = typename TL::TypeAtType<TList, index>::type;

template <typename T, typename D>
inline constexpr static int IndexOf = TL::IndexOfType<T, D>::value;

template <typename List, typename T>
using Append = typename TL::AppendType<List, T>::type;

template <typename TList, typename T>
using Erase = typename TL::EraseType<TList, T>::type;

template <typename TList, typename T>
using EraseAll = typename TL::EraseAllType<TList, T>::type;

template <typename TList>
using EraseDuplicates = typename TL::EraseDuplicatesType<TList>::type;

template <typename T>
inline constexpr static bool IsTypelist = TL::is_typelist<T>::value;

template <typename TList, template <typename> typename Func>
using ForEach = typename TL::ForEachType<TList, Func>::type;

#endif //TYPELIST_HPP
