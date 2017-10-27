

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