#pragma once
#include <limits>

namespace RefLib
{
	using TypeId = size_t;
	constexpr TypeId NoneType = std::numeric_limits<size_t>::max();
}