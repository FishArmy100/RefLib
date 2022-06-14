#include "TypeFlags.h"

namespace RefLib
{
	TypeFlags operator|(TypeFlags lhs, TypeFlags rhs)
	{
		return static_cast<TypeFlags>(
			static_cast<std::underlying_type_t<TypeFlags>>(lhs) |
			static_cast<std::underlying_type_t<TypeFlags>>(rhs)
			);
	}

	TypeFlags operator&(TypeFlags lhs, TypeFlags rhs)
	{
		return static_cast<TypeFlags>(
			static_cast<std::underlying_type_t<TypeFlags>>(lhs) &
			static_cast<std::underlying_type_t<TypeFlags>>(rhs)
			);
	}

	bool Utils::IsTypeFlag(TypeFlags flags, TypeFlags flag)
	{
		return (bool)(flags & flag);
	}

	TypeFlags Utils::ApplyFlagIfTrue(bool condition, TypeFlags initial, TypeFlags added)
	{
		return initial | (condition ? added : TypeFlags::None);
	}
}


