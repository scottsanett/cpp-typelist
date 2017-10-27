

namespace TL {
	template <int num>
	struct Int2Type {
		inline constexpr static int value = v;
	};

	template <typename T>
	struct Type2Type {
		using OriginalType = T;
	};
};