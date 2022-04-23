#pragma once
#include "vendor/rttr/type_name.h"
#include <string>

namespace RefLib
{
	namespace Utils
	{
		template<typename T>
		std::string GetTypeName()
		{
			const char* name = rttr::detail::f<T>();
			size_t length = rttr::detail::get_size(name);
			return std::move(std::string(name, length));
		}
	}
}