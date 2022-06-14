#pragma once
#include <inttypes.h>
#include <type_traits>

namespace RefLib
{
	enum class TypeFlags : uint8_t
	{
		None				= 0b00000000,
		Const				= 0b00000001,
		Reference			= 0b00000010,
		RValueReference		= 0b00000100,
		Volatile			= 0b00001000,
		ConstReference		= Const | Reference
	};

	TypeFlags operator|(TypeFlags lhs, TypeFlags rhs);
	TypeFlags operator&(TypeFlags lhs, TypeFlags rhs);

	namespace Utils 
	{
		bool IsTypeFlag(TypeFlags flags, TypeFlags flag);

		TypeFlags ApplyFlagIfTrue(bool condition, TypeFlags initial, TypeFlags applied);

		template<typename TBase>
		TypeFlags GetFlagsFromType()
		{
			TypeFlags flags = TypeFlags::None;
			flags = ApplyFlagIfTrue(std::is_const_v<TBase>, flags, TypeFlags::Const);
			flags = ApplyFlagIfTrue(std::is_volatile_v<TBase>, flags, TypeFlags::Volatile);
			flags = ApplyFlagIfTrue(std::is_lvalue_reference_v<TBase>, flags, TypeFlags::Reference);
			flags = ApplyFlagIfTrue(std::is_rvalue_reference_v<TBase>, flags, TypeFlags::RValueReference);
			return flags;
		}
	}
}

