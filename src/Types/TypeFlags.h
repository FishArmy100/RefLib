#pragma once
#include <inttypes.h>
#include <type_traits>

namespace RefLib
{
	enum class TypeFlags : uint8_t
	{
		None		= 0b00000000,
		Const		= 0b00000001,
		LValue		= 0b00000010,
		RValue		= 0b00000100,
		Volatile	= 0b00001000,
		Pointer		= 0b00010000,
		Invalid		= 0b10000000
	};

	TypeFlags operator|(TypeFlags lhs, TypeFlags rhs);
	TypeFlags operator&(TypeFlags lhs, TypeFlags rhs);

	namespace Utils 
	{
		bool IsTypeFlag(TypeFlags flags, TypeFlags flag);
	}
}

