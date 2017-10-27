#ifndef TYPE_MAPPING_HPP
#define TYPE_MAPPING_HPP

namespace TL {
	template <unsigned int num>
	struct Int2Type {
		inline constexpr static int value = num;
	};

	template <typename T>
	struct Type2Type {
		using OriginalType = T;
	};
};

#endif