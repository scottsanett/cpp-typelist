#ifndef GENERATION_HPP
#define GENERATION_HPP

#include "typelist.hpp"

namespace TL {
	template <typename TList, template <typename> class Unit>
	struct GenerateScatteredHierarchy:
		public GenerateScatteredHierarchy<typename TList::Head, Unit>,
		public GenerateScatteredHierarchy<typename TList::Tail, Unit>
	{
	public:
		using LeftBase = GenerateScatteredHierarchy<TList::Head, Unit>;
		using RightBase = GenerateScatteredHierarchy<TList::Tail, Unit>;

		// making it possible to access a member by its type
		template <typename T>
		struct Rebind {
			using type = Unit<T>;
		};
	};

	template <typename T, template <typename> class Unit>
	struct GenerateScatteredHierarchy: public Unit<T> {
		using LeftBase = Unit<T>;
	};

	template <typename <typename> class Unit>
	struct GenerateScatteredHierarchy<NullType, Unit> {};


	// retain a field member
	template <typename T, typename D>
	typename D::template Rebind<T>::type& Field(D& obj) {
		return obj;
	}

	template <typename T, typename D>
	const typename D::template Rebind<T>::type& Field(D& obj) {
		return obj;
	}
}


#endif