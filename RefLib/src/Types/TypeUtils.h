#pragma once
#include "vendor/rttr/type_name.h"
#include <string>

namespace RefLib
{
	namespace Utils
	{
		template<typename TBase>
		using decay_t = typename std::decay<TBase>::type;

		template< bool B, class TBase = void >
		using enable_if_t = typename std::enable_if<B, TBase>::type;

		template<typename TBase>
		std::string GetTypeName()
		{
			const char* name = rttr::detail::f<TBase>();
			size_t length = rttr::detail::get_size(name);
			return std::move(std::string(name, length));
		}
	}
}