#include <stddef.h>

namespace RefLib
{
	namespace Utils
	{
		template<size_t index, typename... Types>
		struct TypeGetter;

		template<size_t index, typename First, typename... Types>
		struct TypeGetter<index, First, Types...> {
			using Type = typename TypeGetter<index - 1, Types...>::Type;
		};

		template<typename First, typename... Types>
		struct TypeGetter<0, First, Types...> {
			using Type = First;
		};
	}
}