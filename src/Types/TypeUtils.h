#pragma once
#include "vendor/rttr/type_name.h"
#include <string>

namespace RefLib
{
	namespace Utils
	{
		template<typename T>
		using decay_t = typename std::decay<T>::type;

		template< bool B, class T = void >
		using enable_if_t = typename std::enable_if<B, T>::type;

		template<typename T>
		std::string GetTypeName()
		{
			const char* name = rttr::detail::f<T>();
			size_t length = rttr::detail::get_size(name);
			return std::move(std::string(name, length));
		}
	}
}