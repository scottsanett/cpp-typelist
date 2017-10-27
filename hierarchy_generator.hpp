#ifndef GENERATION_HPP
#define GENERATION_HPP

#include "typelist.hpp"
#include "type_mapping.hpp"

namespace TL {
	template <typename TList, template <typename> class Unit> 
	struct GenerateScatteredHierarchy;

	template <typename Head, typename Tail, template <typename> class Unit>
	struct GenerateScatteredHierarchy<Typelist<Head, Tail>, Unit>:
		public GenerateScatteredHierarchy<Head, Unit>,
		public GenerateScatteredHierarchy<Tail, Unit>
	{
	public:
		using LeftBase = GenerateScatteredHierarchy<Head, Unit>;
		using RightBase = GenerateScatteredHierarchy<Tail, Unit>;

		// making it possible to access a member by its type
		template <typename T>
		struct Rebind {
			using type = Unit<T>;
		};
	};

	template <typename T, template <typename> class Unit>
	struct GenerateScatteredHierarchy: public Unit<T> {
		using LeftBase = Unit<T>;

		template <typename D>
		struct Rebind {
			using type = Unit<D>;
		};
	};

	template <template <typename> class Unit>
	struct GenerateScatteredHierarchy<NullType, Unit> {
		template <typename D>
		struct Rebind {
			using type = Unit<D>;
		};
	};


	// retain a field member
	template <typename T, typename D>
	typename D::template Rebind<T>::type& Field(D& obj) {
		return obj;
	}

	template <typename T, typename D>
	const typename D::template Rebind<T>::type& Field(D& obj) {
		return obj;
	}

	template <typename H, typename R>
	R& FieldHelper(H& obj, Type2Type<R>, Int2Type<0>) {
		typename H::LeftBase& subObj = obj;
		return subObj;
	}

	template <typename H, typename R, int i>
	R& FieldHelper(H& obj, Type2Type<R> tt, Int2Type<i>) {
		typename H::RightBase& subObj = obj;
		return FieldHelper(subObj, tt, Int2Type<i - 1>());
	}
}


#endif