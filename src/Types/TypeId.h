#pragma once
#include <stdint.h>
#include<limits>

namespace RefLib
{
	using TypeId = uint32_t;

	namespace Utils
	{
		constexpr TypeId EmptyType = std::numeric_limits<uint32_t>::max();
	}
}